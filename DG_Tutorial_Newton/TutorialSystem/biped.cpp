#include "pch.h"
#include "GeomGL.h"
#include "biped.h"

Muscle::Muscle(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* insert1, GeomNewton* insert2, dCustomHinge* jointact, dVector ins1, dVector ins2) :
	m_Manager(wMain)
,	body1(insert1)
,   body2(insert2)
,	aVao(0)
,	aVbo(0)
,	m_Insert1(ins1)
,	m_Insert2(ins2)
,   LDebug_Manager(LManager)
,	LineIndex(0)
,	activation(0.0f)
,	stepSize(1/1000.0f)
,	joint(jointact)
,	arm(10.f)
,	phi_M(0.0f)
,	phi_R(3.14f)
,	F_max(2000.0f)
,	rho(0.5f)
,	l_slk(10.f)
,	theta_actual(0.0f)
,	m_Fmtu(0.0f)
,	theta_0(0.0f)
{	
	lCE = 0.5f;
	l_opt = 0.5f;
	vm = 10.0f; // [cm/s] max fiber contraction velocity 
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

float Muscle::GetNmax() {
	return m_nmax;
}

float Muscle::GetDelta_l() {
	return m_Delta_l;
}

float Muscle::GetLCE() {
	return lCE;
}

void Muscle::SetLCE(const float l) {
	lCE = l;
}

void Muscle::SetDelta_l(const float l) {
	m_Delta_l = l;
}

void Muscle::SetLength0(float l) {
	m_Length0 = l;
}

void Muscle::GenerateMesh() {

		glm::vec3 linepos1;
		glm::vec3 linepos2;
		linepos1.x = 0; linepos1.y = 0; linepos1.z = 0; 
		linepos2.x = 2.0f; linepos2.y = 2.0f; linepos2.z = 2.0f;
		lineColor.x = 1.0f; lineColor.y = 0.f; lineColor.z = 0.f;

		LineIndex=LDebug_Manager->AddLine(linepos1, linepos2, lineColor);
		
}

void Muscle::UpdateLineCoord(Shader* cshd, dFloat steptime)
{
	LDebug_Manager->aLineBuffer[LineIndex-1].posit.x = this->GetInsert1_GlobalRef().m_x;
	LDebug_Manager->aLineBuffer[LineIndex-1].posit.y = this->GetInsert1_GlobalRef().m_y;
	LDebug_Manager->aLineBuffer[LineIndex-1].posit.z = this->GetInsert1_GlobalRef().m_z;
	LDebug_Manager->aLineBuffer[LineIndex - 1].color = lineColor;
	LDebug_Manager->aLineBuffer[LineIndex-2].posit.x = this->GetInsert2_GlobalRef().m_x;
	LDebug_Manager->aLineBuffer[LineIndex-2].posit.y = this->GetInsert2_GlobalRef().m_y;
	LDebug_Manager->aLineBuffer[LineIndex-2].posit.z = this->GetInsert2_GlobalRef().m_z;
	LDebug_Manager->aLineBuffer[LineIndex - 2].color = lineColor;
}

float Muscle::dresidu(const float l, const float t) {
		float l_tild,v_tild,ls_tild;
		float f1, f2, df1, df2;
		float l_mtu = Compute_muscle_length(theta_actual);
		l_tild = (this->lCE + l) / this->l_opt;
		v_tild = l / t / this->l_opt;
		f1 = exp(log(0.05f) * pow((l_tild - 1) / 0.56f, 4)); //fl calculation
		df1 = exp(log(0.05f) * pow((l_tild - 1) / 0.56f, 4)) * (log(0.05f) * 4 * pow((l_tild - 1) / 0.56f, 3)) / 0.56f / this->l_opt; // fl derivative
		//fv calculation for contracting and extending muscle cases
		if (v_tild < 0.f) {
			f2 = -(this->vm + v_tild) / (5.0f*v_tild-this->vm); 
			df2 = 6.0f * this->vm / t / this->l_opt / pow(5.0f * v_tild - this->vm, 2); // fv derivative
		}
		else
		{
			f2 = 1.5f + 0.5f * (-this->vm + v_tild) / (37.8f * v_tild + this->vm);
			//df2 = (0.5f * 38.8f * this->vm-1.5f*37.8f) / t / this->l_opt / pow(37.8f * v_tild + this->vm, 2); // check
			df2 = (0.5f * 38.8f * this->vm) / t / this->l_opt / pow(37.8f * v_tild + this->vm, 2); // fv derivative
		}
		f1 = f1 * df2 + f2 * df1;
		//Fhpe component
		if (l_tild > 1.0f) {	
			df2 = 2.0f * (l_tild - 1.0f) / pow(0.56f, 2) / this->l_opt;
		}
		else { df2 = 0.f; }
		f1 = f1 + df2;
		//Flpe component 
		if (l_tild < 0.44f) {
			df2 = -2.0f * (0.44f - l_tild) / pow(0.28f, 2) / this->l_opt;
		}
		f1 = f1 + df2;
		ls_tild = (-(lCE + l) + l_mtu) / this->l_slk;
		//Fse component
		if (ls_tild > 1.0f) {
			f2 = -2.0f * (ls_tild - 1.0f) / pow(0.04f, 2) / this->l_slk;
		}
		else { f2 = 0.0f; }
		return (f1-f2);
	}
// compute Err(delta) = Fce + Fpe + Fse as a function of ce increment
float Muscle::residu(const float l, const float t) {
	float l_tild, v_tild, ls_tild;
	float f1, f2;
	float l_mtu = Compute_muscle_length(theta_actual);
	l_tild = (this->lCE + l) / l_opt; // [cm] l_ce+delta
	v_tild = l / t / this->l_opt; // [cm/s] muscle velocity contraction
	f1 = exp(log(0.05f) * pow((l_tild - 1) / 0.56f, 4)); //fl calculation formula 9 wang 2012 supplemental material 
	//fv calculation for contracting and extending muscle cases. formula 10 wang 2021 supplemental material
	if (v_tild < 0.f) {
		f2 = -(this->vm + v_tild) / (5.0f * v_tild - this->vm);
	}
	else
	{
		f2 = 1.5f + 0.5f * (-this->vm + v_tild) / (37.8f * v_tild + this->vm);
	}
	f1 = activation * f1 * f2; // [-] Fce = activation * fl * fv
	//Fhpe component
	if (l_tild > 1.0f) {
		f2 = pow((l_tild - 1.0f)/0.56f,2); // formula 6
	}
	else { f2 = 0.f; }
	//Flpe component 
	if (l_tild < 0.44f) {
		f2 = -pow((0.44f - l_tild) / 0.28f, 2); // formula 7
	}
	f1 = f1 + f2; // Fce + Fhpe + Flpe
	ls_tild = (l_mtu -(lCE + l)) / this->l_slk; // 
	//Fse component
	if (ls_tild > 1.0f) {
		f2 = pow((ls_tild - 1.0f) / 0.04f, 2); // formula 8
	}
	else { f2 = 0.f; }
	return (f1 - f2); // Err = Fce + Fhpe + Flpe - Fse [-]
}
// series element normalized force [-]
float Muscle::fSE(const float l) {
	float f, ls_tild;
	float l_mtu = Compute_muscle_length(theta_actual);
	ls_tild = (-(this->lCE + l) + l_mtu) / this->l_slk;
	if (ls_tild > 1.0f) {
		f = pow((ls_tild - 1.0f) / 0.04f, 2);
	}
	else { f = 0.0f; }
	return (f);
}
// contractile element normalized force [-]
float Muscle::fCE(const float l, const float t) {
	float l_tild, v_tild;
	float dist_insers;
	float f1, f2;
	v_tild = l / t / this->l_opt;
	l_tild = (this->lCE + l) / this->l_opt;
	f1 = exp(log(0.05f) * pow((l_tild - 1) / 0.56f, 4));
	if (v_tild < 0.0f) {
		f2 = -(this->vm + v_tild) / (5.0f * v_tild - this->vm);
	}
	else { 
		f2 = 1.5f + 0.5f * (-this->vm + v_tild) / (37.8f * v_tild + this->vm);
	}
	return (activation * f1 * f2);
}
// parallel element normalized force [-]
float Muscle::fPE(const float l) {
	float l_tild;
	float f1, f2;
	l_tild = (lCE + l) / this->l_opt;
	if (l_tild > 1.0f) {
		f1 = pow((l_tild - 1.0f) / 0.56f, 2);
	}
	else { f1 = 0.f; }
	if (l_tild < 0.44f) {
		f2 = -pow((0.44f - l_tild) / 0.28f, 2);
	}
	else { f2 = 0.0f; }
	return (f1 + f2);
}
// Compute muscle torque with implicit integration method (Newton-Rhaphson)
double Muscle::Compute_muscle_Torque(dFloat time)
{
	float dl(1.0e-003);
	float tol(1.0e-003);
	float nm(100); // max iterations
	int n(0);
	dl = this->lCE / 10000.f; // [cm] muscle increment initialization

	theta_actual = theta_0 - joint->GetJointAngle(); // [rad] need to access joint angle 
	double l_mtu = Compute_muscle_length(theta_actual); // [cm] compute the muscle length
	// apply newton-Rapshon method
	// 
	//m_nmax = nm;
	//while (m_nmax >= nm - 1) {
	//	dl = dl / 10.0f;
	while ((abs(residu(dl, time)) > tol) && (n < nm)) {
		dl = dl - residu(dl, time) / dresidu(dl, time); // [cm] delta - (Err/dErr)
		n++;
	}
	m_nmax = n;
	//}
	float v_tild = dl / time / this->l_opt;
	m_Fmtu = F_max * (this->fSE(dl)+0.1*v_tild); // [N] added damping as formula 14  in flexing computational muscle: modeling and simulation of musculotendon dynamics 2013 millard
	double T = m_Fmtu * arm * 0.01f; // [Nm]
	m_Delta_l = dl;

	// Assign colors to muscle mesh
	lineColor.x = this->fSE(dl); lineColor.z = 1.0f - this->fSE(dl); lineColor.y = 0;
	if (m_nmax >= nm - 1)
	{
		lineColor.x = 1.0f; lineColor.z = 1.0f; lineColor.y = 1.0f;
	}
	return T;
}
// compute muscle length according to formula 11 in Optimizing locomotion controllers using biologically-based actuators and objectives - Supplemental material Wang 2012 
double Muscle::Compute_muscle_length(double jointangle)
{
	double delta, l_mtu;
	delta = rho * arm * (jointangle - phi_R); // monoarticular muscles
	l_mtu = l_opt + l_slk + delta;
	return l_mtu;
}

void Muscle::SetThetazero(double angle)
{
	theta_0 = angle;
}

void Muscle::SetMuscleParams(const double r, const double phiM, const double phiR, const double Fmax, const double rhoin, const double opt, const double slk)
{
	arm = r;
	phi_M = phiM;
	phi_R = phiR;
	F_max = Fmax;
	rho = rhoin;
	l_opt = opt;
	l_slk = slk;
}

void Muscle::SetInsert1(float px, float py, float pz) {
	m_Insert1.m_x = px;
	m_Insert1.m_y = py;
	m_Insert1.m_z = pz;
	m_Insert1.m_w = 1.0f;
}

void Muscle::SetInsert2(float px, float py, float pz) {
	m_Insert2.m_x = px;
	m_Insert2.m_y = py;
	m_Insert2.m_z = pz;
	m_Insert2.m_w = 1.0f;
}

dVector Muscle::GetInsert1_GlobalRef() {
	NewtonBody* NBody1 = (NewtonBody*)body1->GetBody();
	dVector VTemp(0.0f);
	dMatrix bodymatrix1;
	NewtonBodyGetMatrix(NBody1, &bodymatrix1[0][0]);
	VTemp = bodymatrix1.TransformVector(m_Insert1);
	return dVector( VTemp.m_x,  VTemp.m_y,  VTemp.m_z,  VTemp.m_w);
}

dVector Muscle::GetInsert2_GlobalRef() {
	NewtonBody* NBody2 = (NewtonBody*)body2->GetBody();
	dVector VTemp(0.0f);
	dMatrix bodymatrix2;
	NewtonBodyGetMatrix(NBody2, &bodymatrix2[0][0]);
	VTemp = bodymatrix2.TransformVector(m_Insert2);
	return dVector(VTemp.m_x, VTemp.m_y, VTemp.m_z, VTemp.m_w);
}

void Muscle::GetOriginAndInsertion(dVector& vOrigin, dVector& vInsert) {
	vOrigin = GetInsert1_GlobalRef();
	vInsert = GetInsert2_GlobalRef();
}

void Muscle::SetStepSize(const float iStepSize)
{
	stepSize = iStepSize;
}

void Muscle::SetExcitation(const float iExcitation)
{
	activation = 100* stepSize *(iExcitation- activation) + activation;
}

void Muscle::GetMuscleParams(double& angle, double& lce, double& Fmuscle)
{
	angle = theta_actual;
	lce = lCE;
	Fmuscle = m_Fmtu;
}