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
	m_vce = 0;
	m_time = 0;
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

//[ rhs_class
/* The rhs of x' = f(x) defined as a class needed for time integration of v_ce (ODEINT)*/
class hill_muscle {
	double lmtu;
	int act;
	double vcet_out;
public:
	hill_muscle(int activation, double lmtuin) : act(activation), lmtu(lmtuin){ }
	double pushv() { return vcet_out; }
	void setv(double vin) { vcet_out = vin; }

	void operator() (const state_type& x, state_type& dxdt, const double /*t*/ )
	{
		// Caso vas
		double F0 = 6000;
		double lopt = 0.075f;
		double lslack = 0.215f;
		double lcet = x[0];
		double lse = lmtu - x[0]*lopt;
		double lset = lse / lslack;
		double fl = exp(log(0.05f) * pow(pow(0.56f, -1) * (lcet - 1), 4)); //length contribution
		double Fse = 0; // series force
		double Fhpe = 0;// high parallel force
		double Flpe = 0;// low parallel force

		if ((lset - 1) >= 0) // if l_se > l_slack tendon force is activated
			Fse = F0 * pow((pow(0.04f, -1) * (lset - 1)), 2);

		if ((lcet - 0.44f) <= 0) // if l_ce < 44 % of l_opt the lower stiffness in // with CE is activated
			Flpe = F0 * pow((pow(0.28f, -1) * (0.44f - lcet)), 2);

		if ((lcet - 1) >= 0) // if l_ce > l_opt the higher stiffness in // with CE is actuated
			Fhpe = F0 * pow((pow(0.56f, -1) * (lcet - 1)), 2);

		// find fv
		double fv;
		if ((Fse + Flpe - Fhpe == 0) | (Fse + Flpe - Fhpe > F0))
			fv = 1; // velocity contribution should be between 0 and 1 instead higher values are obtained starting from l_ce = l_opt
		else
			fv = (Fse + Flpe - Fhpe) / (act * F0 * fl);

		// computation of v_ce from fv
		//if (act > 0.0f)
			dxdt[0] = (10.0 * fv - 10.0) / (1.0 + 5.0 * fv);// for fiber flextion
		//else
		//	dxdt[0] = (-10.0f * fv + 10.0f) / (-57.2 + 37.8 * fv); // For extension (not used now)
		setv(dxdt[0]);
	}
};


//observer for time time integration of v_ce (ODEINT)
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
double Muscle::GetForceMTU_V1(dFloat time, dModelRootNode* model)
{
	// Caso vas
	double a = 1;
	double rho = 0.7f;
	double fimax = M_PI - 2.88f;
	double fir = M_PI - 2.18f;
	double r = 0.056f;
	double F0 = 6000;
	double m_Lmtu = 0.0;
	dRaycastVHModel* controller = (dRaycastVHModel*)model;
	dCustomHinge* Knee_L = (controller->GetKnee_L());
	double theta = Knee_L->GetJointAngle() + M_PI; // need to access joint angle
	double penn = rho * r * (sin(theta - fimax) - sin(fir - fimax));
	m_Lmtu = (double)m_Lopt + (double)m_Lslack - penn; // ATTENTION sign of penn must change according to contraction velocity
	double m_lcet = m_lce / m_Lopt;
	double m_lse = m_Lmtu - m_lce;
	double m_lset = m_lse / m_Lslack;
	double fl = exp(log(0.05f) * pow(pow(0.56f, -1) * (m_lcet - 1), 4)); //length contribution
	double m_Fse = 0; // series force
	double m_Fhpe = 0;// high parallel force
	double m_Flpe = 0;// low parallel force

	if ((m_lset - 1) >= 0) // if l_se > l_slack tendon force is activated
		m_Fse = F0 * pow((pow(0.04f, -1) * (m_lset - 1)), 2);

	if ((m_lcet - 0.44f) <= 0) // if l_ce < 44 % of l_opt the lower stiffness in // with CE is activated
		m_Flpe = F0 * pow((pow(0.28f, -1) * (0.44f - m_lcet)), 2);

	if ((m_lcet - 1) >= 0) // if l_ce > l_opt the higher stiffness in // with CE is actuated
		m_Fhpe = F0 * pow((pow(0.56f, -1) * (m_lcet - 1)), 2);

	// find fv
	double fv;
	double m_vcet = m_vce / m_Lopt;
	//if (m_vcet < 0)
		fv = (-10 - m_vcet) / (-10 + 5 * m_vcet); // for fiber flextion
	//else
	//	fv = 1.5f + 0.5 * ((-10+m_vcet)/(37.8*m_vcet+10)); // For extension (not used now)

		if (fv > 1)
			fv = 1;
	// force computation
	
	double m_Fpe = m_Fhpe - m_Flpe; // parallel force
	double m_Fce = a * F0 * fl * fv; // contractile element force
	double m_Fmtu = m_Fce + m_Fpe; // force of the mtu

	// Integration of actual v_ce to update l_ce

	// time integration
	m_time = m_time + time;
	m_time_prec = m_time;// class variable update for next call

	double dt = double(time);// integration step
	
	// state_initialization
	state_type x(1);
	x[0] = m_lcet; // start at actual l_cet

	vector<state_type> x_vec;
	vector<state_type> x_der;
	vector<double> times;

	hill_muscle hm(a, m_Lmtu);
	size_t steps = integrate(boost::ref(hm), x, (double)m_time - dt, (double)m_time, dt, push_back_state_and_time(x_vec, times));
	//size_t steps = integrate(boost::ref(hm), x, (double)m_time - dt, (double)m_time, dt);

	cout << m_lce << '\t' << m_Lmtu << '\t' << m_vce << '\t' << theta << '\t' << m_time << '\t' << m_Fmtu << endl;

	m_lce = x[0] * m_Lopt; // class variable update for next call
	m_vce = (hm.pushv() * m_Lopt);// class variable update for next call
	
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
// Function to get muscle origin body global reference
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