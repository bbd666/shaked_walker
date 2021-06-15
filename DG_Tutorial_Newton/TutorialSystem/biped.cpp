#include "pch.h"
#include "GeomGL.h"
#include "biped.h"

Muscle::Muscle(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* insert1, GeomNewton* insert2, dVector ins1, dVector ins2) :
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
{
	//m_Manager->vMuscleList.push_back(this);
	
	lCE = 0.5f;
	l_opt = 0.25f;
	l_slack = 0.3f;
	vm = 10.0f;
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


float Muscle::GetStiffness() {
	return m_Stiffness;
}

void Muscle::SetStiffness(float l) {
	m_Stiffness = l;
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
		float dist_insers;
		float f1, f2, df1, df2;
		dVector v;
		v = this->GetInsert2_GlobalRef() - this->GetInsert1_GlobalRef();
		dist_insers =sqrt(v.DotProduct3(v));
		l_tild = (this->lCE + l) / this->l_opt;
		v_tild = l / t / this->l_opt;
		f1 = exp(log(0.05f) * pow((l_tild - 1) / 0.56f, 4)); //fl calculation
		df1 = exp(log(0.05f) * pow((l_tild - 1) / 0.56f, 4)) * (log(0.05f) * 4 * pow((l_tild - 1) / 0.56f, 3)) / 0.56f / this->l_opt;
		//fv calculation for contracting and extending muscle cases
		if (v_tild < 0.f) {
			f2 = -(this->vm + v_tild) / (5.0f*v_tild-this->vm); 
			df2 = 6.0f * this->vm / t / this->l_opt / pow(5.0f * v_tild - this->vm, 2);
		}
		else
		{
			f2 = 1.5f + 0.5f * (-this->vm + v_tild) / (37.8f * v_tild + this->vm);
			df2 = (0.5f * 38.8f * this->vm-1.5f*37.8f) / t / this->l_opt / pow(37.8f * v_tild + this->vm, 2);
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
		ls_tild = (-(lCE + l) + dist_insers) / this->l_slack;
		//Fse component
		if (ls_tild > 1.0f) {
			f2 = -2.0f * (ls_tild - 1.0f) / pow(0.04f, 2) / this->l_slack;
		}
		else { f2 = 0.0f; }
		return (f1-f2);
	}

float Muscle::residu(const float l, const float t) {
	float l_tild, v_tild, ls_tild;
	float dist_insers;
	float f1, f2;
	dVector v;
	v = this->GetInsert2_GlobalRef() - this->GetInsert1_GlobalRef();
	dist_insers = sqrt(v.DotProduct3(v));
	l_tild = (this->lCE + l) / l_opt;
	v_tild = l / t / this->l_opt;
	f1 = exp(log(0.05f) * pow((l_tild - 1) / 0.56f, 4)); //fl calculation
	//fv calculation for contracting and extending muscle cases
	if (v_tild < 0.f) {
		f2 = -(this->vm + v_tild) / (5.0f * v_tild - this->vm);
	}
	else
	{
		f2 = 1.5f + 0.5f * (-this->vm + v_tild) / (37.8f * v_tild + this->vm);
	}
	f1 = f1 * f2;
	//Fhpe component
	if (l_tild > 1.0f) {
		f2 = pow((l_tild - 1.0f)/0.56f,2);
	}
	else { f2 = 0.f; }
	//Flpe component 
	if (l_tild < 0.44f) {
		f2 = -pow((0.44f - l_tild) / 0.28f, 2);
	}
	f1 = f1 + f2;
	ls_tild = (-(lCE + l) + dist_insers) / this->l_slack;
	//Fse component
	if (ls_tild > 1.0f) {
		f2 = pow((ls_tild - 1.0f) / 0.04f, 2);
	}
	else { f2 = 0.f; }
	return (f1 - f2);
}

float Muscle::fSE(const float l) {
	float f, ls_tild;
	float dist_insers;
	dVector v;
	v = this->GetInsert2_GlobalRef() - this->GetInsert1_GlobalRef();
	dist_insers = sqrt(v.DotProduct3(v));
	ls_tild = (-(this->lCE + l) + dist_insers) / this->l_slack;
	if (ls_tild > 1.0f) {
		f = pow((ls_tild - 1.0f) / 0.04f, 2);
	}
	else { f = 0.0f; }
	return (f);
}

float Muscle::fCE(const float l, const float t) {
	float l_tild, v_tild;
	float dist_insers;
	float f1, f2;
	dVector v;
	v = this->GetInsert2_GlobalRef() - this->GetInsert1_GlobalRef();
	dist_insers = sqrt(v.DotProduct3(v));
	v_tild = l / t / this->l_opt;
	l_tild = (this->lCE + l) / this->l_opt;
	f1 = exp(log(0.05f) * pow((l_tild - 1) / 0.56f, 4));
	if (v_tild < 0.0f) {
		f2 = -(this->vm + v_tild) / (5.0f * v_tild - this->vm);
	}
	else { 
		f2 = 1.5f + 0.5f * (-this->vm + v_tild) / (37.8f * v_tild + this->vm);
	}
	return (f1 * f2);
}

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

dVector Muscle::GetForceElas(const float t) {
	dVector Elongation;
	float dl(1.0e-003);
	float tol(1.0e-003);
	float nm(100);
	int n(0);
	dl = this->lCE / 10000.f;
	
	//m_nmax = nm;
	//while (m_nmax >= nm - 1) {
	//	dl = dl / 10.0f;
		while ((abs(residu(dl, t)) > tol) && (n < nm)) {
			dl = dl - residu(dl, t) / dresidu(dl, t);
			n++;
		}
		m_nmax = n;
	//}


	Elongation = this->GetInsert2_GlobalRef() - this->GetInsert1_GlobalRef();
	Elongation = Elongation.Normalize();

	m_FElas = Elongation.Scale(this->fSE(dl));
	m_FElas = m_FElas.Scale(this->activation);
	lineColor.x = this->fSE(dl); lineColor.z = 1.0f- this->fSE(dl); lineColor.y = 0;
	m_Delta_l= dl;
	if (m_nmax >= nm-1) 
	{
		lineColor.x = 1.0f; lineColor.z = 1.0f; lineColor.y = 1.0f;
	}

	return m_FElas;
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

MuscleV2::MuscleV2(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* bodyins, dCustomHinge* jointact) :
	m_Manager(wMain)
	, body(bodyins)
	, LDebug_Manager(LManager)
	, joint(jointact)
	, activation(1.0f)
	, stepSize(1 / 1000.0f)
	, arm(10.f)
	, phi_M(0.0f)
	, phi_R(3.14f)
	, F_max(2000.0f)
	, rho(0.5f)
	, l_opt(11.f)
	, l_slk(10.f)
	, l_ce(11.f)
	, v_ce(0.0f)
	, theta_actual(0.0f)
	, m_Fmtu(0.0f)
	, theta_0(0.0f)
{

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

void MuscleV2::SetExcitation(const double iExcitation)
{
	activation = 100 * stepSize * (iExcitation - activation) + activation;
}

void MuscleV2::SetThetazero(double angle)
{
	theta_0 = angle;
}

double MuscleV2::Compute_muscle_length(double jointangle)
{
	double penn, l_mtu;
	penn = rho * arm * (jointangle - phi_R);
	l_mtu = l_opt + l_slk + penn;
	return l_mtu;
}

void MuscleV2::SetMuscleParams(const double r, const double phiM, const double phiR, const double Fmax, const double rhoin, const double opt, const double slk)
{
	arm = r;
	phi_M = phiM;
	phi_R = phiR;
	F_max = Fmax;
	rho = rhoin;
	l_opt = opt;
	l_slk = slk;
}


float MuscleV2::Compute_muscle_Torque(dFloat time) {

	theta_actual = theta_0 - joint->GetJointAngle(); // need to access joint angle
	double l_mtu = Compute_muscle_length(theta_actual);
	double m_lcet = l_ce / l_opt;
	double m_lse = l_mtu - l_ce;
	double m_lset = m_lse / l_slk;
	double fl = exp(log(0.05f) * pow(pow(0.56f, -1) * (m_lcet - 1), 4)); //length contribution
	double m_Fse = 0; // series force
	double m_Fhpe = 0;// high parallel force
	double m_Flpe = 0;// low parallel force

	if ((m_lset - 1) > 0) // if l_se > l_slack tendon force is activated
		m_Fse = F_max * pow((pow(0.04f, -1) * (m_lset - 1)), 2);

	if ((m_lcet - 0.44f) < 0) // if l_ce < 44 % of l_opt the lower stiffness in // with CE is activated
		m_Flpe = F_max * pow((pow(0.28f, -1) * (0.44f - m_lcet)), 2);

	if ((m_lcet - 1) > 0) // if l_ce > l_opt the higher stiffness in // with CE is actuated
		m_Fhpe = F_max * pow((pow(0.56f, -1) * (m_lcet - 1)), 2);

	// find fv
	double fv;
	//if (m_lset <= 1) // to be checked
	//	fv = 1;
	//else
		fv = (m_Fse - m_Fhpe + m_Flpe) / F_max / activation / fl;

	if (fv > 1.5f)
		fv = 1.5f; // max value for fv given a v_max = 10 length/s

	// force computation
	double m_Fpe = m_Fhpe - m_Flpe; // parallel force
	double m_Fce = activation * F_max * fl * fv; // contractile element force
	m_Fmtu = m_Fce + m_Fpe; // force of the mtu

	// Integration of actual fv to update l_ce
	double dt = double(time);// integration step

	// computation of actual v_ce
	double m_vcet;
	if (theta_actual - theta_prec >= 0) // lengthening
		m_vcet = ((-10 * fv) + 10) / (-57.2 + (37.8 * fv));
	else // shortening
		m_vcet = (-10 + (10 * fv)) / (1 + (5 * fv));

	l_ce = l_ce + m_vcet * dt * l_opt;
	theta_prec = theta_actual;
	//cout << l_mtu << '\t' << m_vcet << '\t' << m_lcet << '\t' << theta << '\t' << m_Fmtu << endl;

	double T = m_Fmtu * arm * 0.01f; // in Nm
	//cout << theta << '\t' << l_ce << '\t'<<  m_Fmtu << '\t' << m_Fce << '\t' << m_Fhpe << '\t' << m_Flpe << '\t' << fl  << '\t' << fv << '\n';
	return T;
}

void MuscleV2::GetMuscleParams(double& angle, double& lce, double& Fmuscle)
{
	angle = theta_actual;
	lce = l_ce;
	Fmuscle = m_Fmtu;
}