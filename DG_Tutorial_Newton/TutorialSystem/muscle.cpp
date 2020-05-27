#include "pch.h"
#include "GeomGL.h"
#include "muscle.h"
#include "OXVehicleRayCast.h"


using namespace std;
using namespace boost::numeric::odeint;
// HILL Mucle as described by Geyer and Herr 2010
/* The type of container used to hold the state vector */
typedef std::vector< double > state_type;

Muscle::Muscle(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* insertion_body, GeomNewton* origin_body, dVector origin, dVector insertion, dVector insertion2) :
	m_Manager(wMain)
,	body1(insertion_body)
,   body2(origin_body)
,	m_origin(origin)
,	m_insertion(insertion)
,	m_insertion2(insertion2)
,   LDebug_Manager(LManager)
,	LineIndex(0)
,	LineIndex1(0)
{
	m_Manager->vMuscleList.push_back(this);
	m_Length0 = 1.f; // [m] initial length
	m_Stiffness = 1000.0f; // rimuovere
	m_Lopt = 0.075f; // [m] type hill muscle optimal length
	m_Lslack = 0.215f; // [m] type hill muscle slack length
	m_Fmax = 4000.f; // [N] 
	m_Vmax = 12.f; // [lopts^-1]
	m_Arm = {0, 0};
	m_lce = m_Lopt;
	m_vint = 0;
	m_time_prec = 0;
	

}

Muscle::~Muscle() {
	if (!m_Manager->IsTerminated)
		for (auto itr = m_Manager->vMuscleList.begin();
			itr != m_Manager->vMuscleList.end(); itr++) {
		if ((Muscle*)*itr == this) {
			m_Manager->vMuscleList.erase(itr);
			break;
		}
	}
}

float Muscle::GetLength0() {
	return m_Length0;
}

void Muscle::SetLength0(float l) {
	m_Length0 = l;
}


void Muscle::SetStiffness(float kpe, float kbe, float kse) 
{
	m_PE = kpe;
	m_BE = kbe;
	m_SE = kse;
}

float Muscle::GetFmax() {
	return m_Fmax;
}

void Muscle::SetFmax(float F) // modoficare 
{
	m_Fmax = F;
}

float Muscle::GetVmax() {
	return m_Vmax;
}

void Muscle::SetVmax(float V) // modoficare 
{
	m_Vmax = V;
}

float Muscle::GetLopt() {
	return m_Lopt;
}

void Muscle::SetLopt(float L) // modoficare 
{
	m_Lopt = L;
}

float Muscle::GetLslack() {
	return m_Lslack;
}

void Muscle::SetLslack(float L) // modoficare 
{
	m_Lslack = L;
}

void Muscle::SetinitialVel(double x) {
	m_vint = x;
}

void Muscle::GenerateMesh() {

		glm::vec3 linepos1; //  origin
		glm::vec3 linepos2; //  insertion
		glm::vec3 linepos3; // point between insertion and origin
		glm::vec3 linecolor; // color line
		linepos1.x = 0; linepos1.y = 0; linepos1.z = 0; 
		linepos2.x = 2.0f; linepos2.y = 2.0f; linepos2.z = 2.0f;
		linepos3.x = 3.0f; linepos3.y = 3.0f; linepos3.z = 3.0f;
		linecolor.x = 1.0f; linecolor.y = 0.f; linecolor.z = 0.f;

		LineIndex = LDebug_Manager->AddLine(linepos1, linepos3, linecolor);
		LineIndex1 = LDebug_Manager->AddLine(linepos3, linepos2, linecolor);
}

void Muscle::UpdateLineCoord(Shader* cshd, dFloat steptime)
{
	// Origin
	LDebug_Manager->aLineBuffer[LineIndex1 - 4.0].posit.x = this->GetOrigin_GlobalRef().m_x;
	LDebug_Manager->aLineBuffer[LineIndex1 - 4.0].posit.y = this->GetOrigin_GlobalRef().m_y;
	LDebug_Manager->aLineBuffer[LineIndex1 - 4.0].posit.z = this->GetOrigin_GlobalRef().m_z;
	//aLineBuffer[LineIndex].color = newlinecolor;
	// Insertion
	LDebug_Manager->aLineBuffer[LineIndex1 - 1.0].posit.x = this->GetInsertion_GlobalRef().m_x;
	LDebug_Manager->aLineBuffer[LineIndex1 - 1.0].posit.y = this->GetInsertion_GlobalRef().m_y;
	LDebug_Manager->aLineBuffer[LineIndex1 - 1.0].posit.z = this->GetInsertion_GlobalRef().m_z;
	// external point
	LDebug_Manager->aLineBuffer[LineIndex1 - 3.0].posit.x = this->GetInsertion2_GlobalRef().m_x;
	LDebug_Manager->aLineBuffer[LineIndex1 - 3.0].posit.y = this->GetInsertion2_GlobalRef().m_y;
	LDebug_Manager->aLineBuffer[LineIndex1 - 3.0].posit.z = this->GetInsertion2_GlobalRef().m_z;
	// external point
	LDebug_Manager->aLineBuffer[LineIndex1 - 2.0].posit.x = this->GetInsertion2_GlobalRef().m_x;
	LDebug_Manager->aLineBuffer[LineIndex1 - 2.0].posit.y = this->GetInsertion2_GlobalRef().m_y;
	LDebug_Manager->aLineBuffer[LineIndex1 - 2.0].posit.z = this->GetInsertion2_GlobalRef().m_z;

}
// Function to compute the elastic force of muscle
dVector Muscle::GetForceElas() 
{
	dVector Elongation;
	Elongation = ( this->GetInsertion_GlobalRef()) - this->GetOrigin_GlobalRef() ;// check
	cout << Elongation[0] << '\t' << Elongation[1] << '\t' << Elongation[2] << endl;
	m_FElas = Elongation.Scale(m_Stiffness / m_Length0);

	return m_FElas;
}

//[ rhs_class
/* The rhs of x' = f(x) defined as a class */
class hill_muscle {

	double l_opt;
	int act;
	double l_mtu;
	double F_max;
	double l_slack;
public:
	hill_muscle(int activation, double loptin, double m_lmtu, double lslack, double Fmax) : act(activation), l_opt(loptin), l_mtu(m_lmtu), l_slack(lslack), F_max(Fmax) { }

	void operator() (const state_type& x, state_type& dxdt, const double /* t */)
	{
		double m_lcet = x[0] / l_opt;
		double m_lse = l_mtu - x[0];
		double m_lset = m_lse / l_slack;
		double fl = exp(log(0.05f) * pow(pow(0.56f, -1) * (m_lcet - 1), 4));
		double m_Fse = F_max * pow((pow(0.04f, -1) * (m_lset - 1)), 2);
		double m_Flpe = F_max * pow((pow(0.28f, -1) * (0.44f - m_lcet)), 2);
		double m_Fhpe = F_max * pow((pow(0.56f, -1) * (m_lcet - 1)), 2);
		double fv = (m_Fse + m_Flpe - m_Fhpe) / (act * F_max * fl);

		//dxdt = (10.0 * fv - 10.0) * l_opt / (1.0 + 5.0 * fv); // muscle lengthening
		dxdt[0] = (-10.0f * fv + 10.0f) * l_opt / (-57.2 + 37.8 * fv); // muscle shortening
	}
};
//]


//observer
struct push_back_state_and_time
{
	std::vector< state_type >& m_states;
	std::vector< double >& m_times;

	push_back_state_and_time(std::vector< state_type >& states, std::vector< double >& times)
		: m_states(states), m_times(times) { }

	void operator()(const state_type& x, double t)
	{
		m_states.push_back(x);
		m_times.push_back(t);
	}
};

 //Optimization locomotion controller additional material 2012
dVector Muscle::GetForceMTU_V2(dFloat time, dModelRootNode* model)
{
	// m_lce is and input from AI 
// Caso vas
	float a = 1;
	float rho = 0.7f;
	float fimax = M_PI - 2.88f;
	float fir = M_PI - 2.18f;
	float r = 0.056f;
	float F0 = 6;
	dRaycastVHModel* controller = (dRaycastVHModel*)model;
	dCustomHinge* Knee_L = (controller->GetKnee_L());
	float theta = Knee_L->GetJointAngle() + M_PI; // need to access joint angle
	//float m_Lmtu = m_Lopt + m_Lslack + rho * r * (sin(theta - fimax) - sin(fir - fimax));  // lengthening 
	float m_Lmtu = m_Lopt + m_Lslack - rho * r * (sin(theta - fimax) - sin(fir - fimax));  // shortening
	float m_lcet = m_lce / m_Lopt;
	float m_lse = m_Lmtu - m_lce;
	float m_lset = m_lse / m_Lslack;
	float fl = exp(log(0.05f) * pow(pow(0.56f, -1) * (m_lcet - 1), 4));
	float m_Fse = F0 * pow((pow(0.04f, -1) * (m_lset - 1)), 2);
	float m_Flpe = F0 * pow((pow(0.28f, -1) * (0.44f - m_lcet)), 2);
	float m_Fhpe = F0 * pow((pow(0.56f, -1) * (m_lcet - 1)), 2);
	float fv = (m_Fse - m_Flpe - m_Fhpe) / (a * F0 * fl);
	float m_Fpe = m_Fhpe - m_Flpe;
	float m_Fce = a * F0 * fl * fv;
	dVector m_Fmtu;
	float F = m_Fce + m_Fpe;
	m_Fmtu.m_x = F * cos(30 * dDegreeToRad);// X component FIX
	m_Fmtu.m_y = F * sin(30 * dDegreeToRad);// Y component FIX
	m_Fmtu.m_z = F * sin(00 * dDegreeToRad);// Z component FIX

	// Integration of actual v_ce to update l_ce
	double dt = (double)time - (double)m_time_prec;// find integration step
	if (dt > 0.0) // since muscle is called 3 times at same time instant (why?) integration is done only the first call
	{
		m_time_prec = time;// class variable update for next call
		//[ state_initialization
		state_type x(1);
		x[0] = m_lce; // start at actual l_ce

		vector<state_type> x_vec;
		vector<double> times;

		hill_muscle hm(a, m_Lopt, m_Lmtu, m_Lslack, F0);
		//size_t steps = integrate(hm, x, 0.0, dt, dt);
		size_t steps = integrate(hm, x, 0.0, dt, dt, push_back_state_and_time(x_vec, times));

		m_lce = x[0]; // class variable update for next call
	}
	
	return m_Fmtu;
}

void Muscle::SetOrigin(float px, float py, float pz) {
	m_origin.m_x = px;
	m_origin.m_y = py;
	m_origin.m_z = pz;
	m_origin.m_w = 1.0f;
}

void Muscle::SetInsertion(float px, float py, float pz) {
	m_insertion.m_x = px;
	m_insertion.m_y = py;
	m_insertion.m_z = pz;
	m_insertion.m_w = 1.0f;
}
// Function to get musclue origin body global reference
dVector Muscle::GetOrigin_GlobalRef() {
	NewtonBody* NBody1 = (NewtonBody*)body1->GetBody();
	dVector VTemp(0.0f);
	dMatrix bodymatrix1;
	NewtonBodyGetMatrix(NBody1, &bodymatrix1[0][0]);
	VTemp = bodymatrix1.TransformVector(m_origin);
	return dVector( VTemp.m_x,  VTemp.m_y,  VTemp.m_z,  VTemp.m_w);
}
// Function to get muscle insertion body global reference 
dVector Muscle::GetInsertion_GlobalRef() {
	NewtonBody* NBody2 = (NewtonBody*)body2->GetBody();
	dVector VTemp(0.0f);
	dMatrix bodymatrix2;
	NewtonBodyGetMatrix(NBody2, &bodymatrix2[0][0]);
	VTemp = bodymatrix2.TransformVector(m_insertion);
	return dVector(VTemp.m_x, VTemp.m_y, VTemp.m_z, VTemp.m_w);
}
// Function to get muscle insertion body global reference
dVector Muscle::GetInsertion2_GlobalRef() {
	NewtonBody* NBody2 = (NewtonBody*)body2->GetBody();
	dVector VTemp(0.0f);
	dMatrix bodymatrix2;
	NewtonBodyGetMatrix(NBody2, &bodymatrix2[0][0]);
	VTemp = bodymatrix2.TransformVector(m_insertion2);
	return dVector(VTemp.m_x, VTemp.m_y, VTemp.m_z, VTemp.m_w);
}