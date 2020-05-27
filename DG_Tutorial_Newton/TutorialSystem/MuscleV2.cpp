#include "pch.h"
#include "MuscleV2.h"
#include "GeomGL.h"
#include "OXVehicleRayCast.h"


using namespace std;
using namespace boost::numeric::odeint;
// HILL Mucle as described by Geyer and Herr 2010

MuscleV2::MuscleV2(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* insertion_body, GeomNewton* origin_body, GeomNewton* sliding_body, dVector origin, dVector insertion, dVector insertion2) :
	m_Manager(wMain)
	, body1(insertion_body)
	, body2(origin_body)
	, body3(sliding_body)
	, m_origin(origin)
	, m_insertion(insertion)
	, m_insertion2(insertion2)
	, LDebug_Manager(LManager)
	, LineIndex(0)
	, LineIndex1(0)
{
	m_Manager->vMuscleList.push_back(this);
	m_Length0 = 1.f; // [m] initial length
	m_Stiffness = 1000.0f; // rimuovere
	m_Lopt = 0.075f; // [m] type hill muscle optimal length
	m_Lslack = 0.215f; // [m] type hill muscle slack length
	m_Fmax = 4000.f; // [N] 
	m_Vmax = 12.f; // [lopts^-1]
	m_Arm = { 0, 0 };
	m_lce = m_Lopt;
	m_vint = 0;
	m_time_prec = 0;


}

MuscleV2::~MuscleV2() {
	if (!m_Manager->IsTerminated)
		for (auto itr = m_Manager->vMuscleList.begin();
			itr != m_Manager->vMuscleList.end(); itr++) {
		if ((MuscleV2*)*itr == this) {
			m_Manager->vMuscleList.erase(itr);
			break;
		}
	}
}

float MuscleV2::GetLength0() {
	return m_Length0;
}

void MuscleV2::SetLength0(float l) {
	m_Length0 = l;
}


void MuscleV2::SetStiffness(float kpe, float kbe, float kse)
{
	m_PE = kpe;
	m_BE = kbe;
	m_SE = kse;
}

float MuscleV2::GetFmax() {
	return m_Fmax;
}

void MuscleV2::SetFmax(float F) // modoficare 
{
	m_Fmax = F;
}

float MuscleV2::GetVmax() {
	return m_Vmax;
}

void MuscleV2::SetVmax(float V) // modoficare 
{
	m_Vmax = V;
}

float MuscleV2::GetLopt() {
	return m_Lopt;
}

void MuscleV2::SetLopt(float L) // modoficare 
{
	m_Lopt = L;
}

float MuscleV2::GetLslack() {
	return m_Lslack;
}

void MuscleV2::SetLslack(float L) // modoficare 
{
	m_Lslack = L;
}

void MuscleV2::SetinitialVel(double x) {
	m_vint = x;
}

void MuscleV2::GenerateMesh() {

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

void MuscleV2::UpdateLineCoord(Shader* cshd, dFloat steptime)
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


// this function returns the muscle force computed with measured lce (no integration)
dVector MuscleV2::GetForceMTU_V1()
{
	// Caso vas
	float a = 1;
	float rho = 0.7f;
	float fimax = M_PI - 2.88f;
	float fir = M_PI - 2.18f;
	float r = 0.056f;
	float F0 = 6000;
	dVector m_Lmtu = (this->GetInsertion_GlobalRef() - this->GetOrigin_GlobalRef());// check
	float m_lcet = sqrt(pow(m_Lmtu.m_x,2)+ pow(m_Lmtu.m_y, 2)+ pow(m_Lmtu.m_z, 2))/ 0.4281f; // HP Lce i less than Lmtu, but how much? with 1/100 ?
	float fl, fv;
	fl = exp(log(0.05f) * pow(pow(0.56f, -1) * (m_lce - 1), 4));
	float m_Flpe, m_Fhpe;
	m_Flpe = F0 * pow((pow(0.28f, -1) * (0.44f - m_lcet)), 2);
	m_Fhpe = F0 * pow((pow(0.56f, -1) * (m_lcet - 1)), 2);

	// Get global velocity of bodies
	NewtonBody* NBody1 = (NewtonBody*)body1->GetBody();
	dVector v_b1, v_b12;
	//dMatrix T(body1->GetMatrix());
	//dVector p_c = T.m_posit;// COM point in global coordinates
	//p_c = p_c + m_origin; // origin point in global coord
	//NewtonBodyGetVelocity(NBody1,&v_b12[0]); // velocity of the body origin point
	//NewtonBodyGetPointVelocity(NBody1, &p_c[0], &v_b1[0]);// velocity muscle origin point
	NewtonBody* NBody2 = (NewtonBody*)body2->GetBody();
	dVector v_b2;
	NewtonBodyGetVelocity(NBody1, &v_b2[0]);
	float v_ce = sqrt(pow(v_b2.m_x, 2) + pow(v_b2.m_y, 2) + pow(v_b2.m_z, 2)); // computed numerically
	float v_cet = v_ce / m_Lopt;
	if (v_ce < 0)
	{
		fv = (-10.f - v_cet) / (-10.f + 5.f * v_cet);;
	}
	else {
		fv = 1.5f + 0.5f * ((-10.0f + v_cet) / (37.8f * v_cet+ 10.0f));
	}
	float m_Fpe = m_Fhpe - m_Flpe;
	float m_Fce;
	m_Fce = a * F0 * fl * fv;
	dVector m_Fmtu = { m_Fce ,1.0,1.0,0.0};
	return m_Fmtu;
}

void MuscleV2::SetOrigin(float px, float py, float pz) {
	m_origin.m_x = px;
	m_origin.m_y = py;
	m_origin.m_z = pz;
	m_origin.m_w = 1.0f;
}

void MuscleV2::SetInsertion(float px, float py, float pz) {
	m_insertion.m_x = px;
	m_insertion.m_y = py;
	m_insertion.m_z = pz;
	m_insertion.m_w = 1.0f;
}
// Function to get musclue origin body global reference
dVector MuscleV2::GetOrigin_GlobalRef() {
	NewtonBody* NBody1 = (NewtonBody*)body1->GetBody();
	dVector VTemp(0.0f);
	dMatrix bodymatrix1;
	NewtonBodyGetMatrix(NBody1, &bodymatrix1[0][0]);
	VTemp = bodymatrix1.TransformVector(m_origin);
	return dVector(VTemp.m_x, VTemp.m_y, VTemp.m_z, VTemp.m_w);
}
// Function to get muscle insertion body global reference 
dVector MuscleV2::GetInsertion_GlobalRef() {
	NewtonBody* NBody2 = (NewtonBody*)body2->GetBody();
	dVector VTemp(0.0f);
	dMatrix bodymatrix2;
	NewtonBodyGetMatrix(NBody2, &bodymatrix2[0][0]);
	VTemp = bodymatrix2.TransformVector(m_insertion);
	return dVector(VTemp.m_x, VTemp.m_y, VTemp.m_z, VTemp.m_w);
}
// Function to get sliding body global reference
dVector MuscleV2::GetInsertion2_GlobalRef() {
	NewtonBody* NBody3 = (NewtonBody*)body3->GetBody();
	dVector VTemp(0.0f);
	dMatrix bodymatrix3;
	NewtonBodyGetMatrix(NBody3, &bodymatrix3[0][0]);
	VTemp = bodymatrix3.TransformVector(m_insertion2);
	return dVector(VTemp.m_x, VTemp.m_y, VTemp.m_z, VTemp.m_w);
}