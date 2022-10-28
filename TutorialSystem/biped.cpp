#include "pch.h"
#include "GeomGL.h"
#include "biped.h"
//EX: muscle:
//1. body 1 (proximal ref)
//2. body 2 (proximal ref)
//3. body 3 (proximal ref) (NULL if no body)
//4. Mesh coordinates: vector 1 on body 1, vector 2 on body 2 (or 3 for biarticular) (change the values in 'DummyGeometricProperties.xml' file)
//5. Enum containing the name of the first joint. The name MUST be UNIQUE. The first joint is the proximal (closer to the head)
//6. Enum containing the type of the first joint (Hinge, BallAndSocket, DoubleHinge)
//7. Enum containing the name of the second joint. The name MUST be UNIQUE. Use NOjoint if the muscle is monoarticular
//8. Enum containing the type of the second joint (Hinge, BallAndSocket, DoubleHinge). select NOtype if the muscle is monoarticular
Muscle::Muscle(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* insert1, GeomNewton* insert2, GeomNewton* insert3, dVector ins1, dVector ins2, JointName jname, JointType jtype, JointName jname1, JointType jtype1, Mtuname mname) :
	m_Manager(wMain)
	, body1(insert1)
	, body2(insert2)
	, body3(insert3)
	, aVao(0)
	, aVbo(0)
	, m_Insert1(ins1)
	, m_Insert2(ins2)
	, LDebug_Manager(LManager)
	, LineIndex(0)
	, activation(0.0f)
	, stepSize(1 / 1000.0f)
	, arm(10.f)
	, phi_M(0.0f)
	, phi_R(3.14f)
	, phi1_M(0.0f)
	, phi1_R(3.14f)
	, F_max(2000.0f)
	, rho(0.5f)
	, l_slk(10.f)
	, l_mtu(0.0f)
	, theta_actual(0.0f)
	, m_Fmtu(0.0f)
	, theta_0(0.0f)
	, damp(0.0f)
	, Jname(jname)
	, Jtype(jtype)
	, Jname1(jname1)
	, Jtype1(jtype1)
	, m_name(mname)
{	
	lCE = 0.0f;
	l_opt = 0.0f;
	vm = 0.0f; // [l_opt/s] max fiber contraction velocity 
	v = 0.0f;
	damp = 0.01f;
	kj = 0.3f; // [Nm/rad]
	vj_max = 1; //[rad/s]
	ang_lim = 0;
	ang1_lim = 0;
}

Muscle::~Muscle() {
	if (!m_Manager->IsTerminated)
		for (auto itr = m_Manager->vMuscleList.begin();
			itr != m_Manager->vMuscleList.end(); itr++) {
			if ((Muscle*)*itr == this) {
				m_Manager->vMuscleList.erase(itr);
				break;
			}
			delete body1;
			delete body2;
			delete body3;
		}
}

Mtuname Muscle::GetMuscleName() {
	return m_name;
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

float Muscle::GetAngle() {
	return Jang;
}

void Muscle::SetAngle(float ang) {
	// change sign 
	switch (m_name) {
	case HFL: {Jang = -ang;break;}
	case RF: {Jang = -ang;break;}
	case VAS: {Jang = -ang;break;}
	case TA: {Jang = -ang;break;}
	default: Jang = ang;
	}
}

float Muscle::GetAngle1() {
	return Jang1;
}

void Muscle::SetAngle1(float ang) {
	// change sign
	switch (m_name) {
	case RF: {Jang1 = -ang;break;}// CHECK
	default: Jang1 = ang;
	}
}

void Muscle::SetLimits(float a_lim, float a1_lim) {
	ang_lim = a_lim; // limit joint angle
	ang1_lim = a1_lim; // limitl joint angle 1
}

char Muscle::GetLaterality()
{
	return laterality;
}

void Muscle::SetLaterality(char letter)
{
	laterality = letter;
}

void Muscle::GenerateMesh() {

		glm::vec3 linepos1;
		glm::vec3 linepos2;
		linepos1.x = 0; linepos1.y = 0; linepos1.z = 0; 
		linepos2.x = 2.0f; linepos2.y = 2.0f; linepos2.z = 2.0f;
		lineColor.x = 1.0f; lineColor.y = 0.f; lineColor.z = 0.f;

		if (LDebug_Manager != nullptr)
			LineIndex=LDebug_Manager->AddLine(linepos1, linepos2, lineColor);
		
}

void Muscle::UpdateLineCoord(Shader* cshd, dFloat steptime)
{
	if (LDebug_Manager != nullptr) {
		LDebug_Manager->aLineBuffer[LineIndex - 1].posit.x = this->GetInsert1_GlobalRef().m_x;
		LDebug_Manager->aLineBuffer[LineIndex - 1].posit.y = this->GetInsert1_GlobalRef().m_y;
		LDebug_Manager->aLineBuffer[LineIndex - 1].posit.z = this->GetInsert1_GlobalRef().m_z;
		LDebug_Manager->aLineBuffer[LineIndex - 1].color = lineColor;
		LDebug_Manager->aLineBuffer[LineIndex - 2].posit.x = this->GetInsert2_GlobalRef().m_x;
		LDebug_Manager->aLineBuffer[LineIndex - 2].posit.y = this->GetInsert2_GlobalRef().m_y;
		LDebug_Manager->aLineBuffer[LineIndex - 2].posit.z = this->GetInsert2_GlobalRef().m_z;
		LDebug_Manager->aLineBuffer[LineIndex - 2].color = lineColor;
	}
}

float Muscle::dresidu(const float l, const float t) {
		float l_tild,v_tild,ls_tild, a_tild;
		float f1, f2, df1, df2;
		float l_mtu = Compute_muscle_length(theta_actual, theta1_actual);
		l_tild = (this->lCE + l) / this->l_opt;
		v_tild = l / t / this->l_opt; // [-/s] muscle velocity contraction

		if ((v_tild < 0) & (l_tild < 0.44)) // eq 7 millard 2013
			v_tild = 0;

		//if (v_tild < -vm / this->l_opt)
		//	v_tild = -vm / this->l_opt;

		f1 = exp(log(0.05f) * pow((l_tild - 1) / 0.56f, 4)); //fl calculation
		df1 = exp(log(0.05f) * pow((l_tild - 1) / 0.56f, 4)) * (log(0.05f) * 4 * pow((l_tild - 1) / 0.56f, 3)) / 0.56f / this->l_opt; // fl derivative
		//fv calculation for contracting and lengthening muscle cases
		if (v_tild < 0.f) {
			f2 = -(this->vm + v_tild) / (5.0f*v_tild-this->vm); 
			df2 = 6.0f * this->vm / t / this->l_opt / pow(5.0f * v_tild - this->vm, 2); // fv derivative
			//df2 = -1 / this->l_opt / t * pow(-10 + 5 * l / this->l_opt / t, -1) + 5 / this->l_opt / t * (10 + l / this->l_opt / t) * pow(-10 + 5 * l / this->l_opt / t, -2);
		}
		else
		{
			f2 = 1.5f + 0.5f * (-this->vm + v_tild) / (37.8f * v_tild + this->vm);
			df2 = 38.8 * this->vm * 0.5f / t / this->l_opt / pow(37.8f * v_tild + this->vm, 2); // fv derivative
			//df2 = 0.5 / this->l_opt / t * pow(37.8 * l / this->l_opt * t + 10, -1) - 0.5 * 37.8 / this->l_opt / t * (-10 + l / this->l_opt / t) * pow(37.8 * l / this->l_opt * t + 10, -2);
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
		f1 = f1 - df2;
		ls_tild = (-(lCE + l) + l_mtu) / this->l_slk;
		//Fse component
		if (ls_tild > 1.0f) {
			f2 = -2.0f * (ls_tild - 1.0f) / pow(0.04f, 2) / this->l_slk;
		}
		else { f2 = 0.0f; }
		f1 = f1 - f2;
		// Fdamp component
		f2 = this->damp / t / this->l_opt;
		return (f1 + f2);
	}
// compute Err(delta) = Fce + Fpe + Fse as a function of ce increment
float Muscle::residu(const float l, const float t) {
	float l_tild, v_tild, ls_tild;
	float f1, f2;
	float l_mtu = Compute_muscle_length(theta_actual, theta1_actual);
	l_tild = (this->lCE + l) / l_opt; // [-] l_ce+delta
	v_tild = l / t / this->l_opt; // [-/s] muscle velocity contraction

	if ((v_tild < 0) & (l_tild < 0.44)) // eq 7 millard 2013
		v_tild = 0;

	//if (v_tild < -vm / this->l_opt)
	//	v_tild = -vm / this->l_opt;

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
	f1 = f1 - f2;
	// Fdamp component
	f2 = this->damp * l / this->l_opt / t;
	return (f1 + f2); // Err = Fce + Fhpe + Flpe - Fse + Fdamp[-]
}
// series element normalized force [-]
float Muscle::fSE(const float l) {
	float f, ls_tild;
	float l_mtu = Compute_muscle_length(theta_actual, theta1_actual);
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
// parallel damper normalized force [-]
float Muscle::fDE(const float l, const float t) {
	float l_tild;
	float v_tild, f;
	l_tild = (lCE + l) / this->l_opt;
	v_tild =  l / t / this->l_opt;
	if ((v_tild < 0.0f) & (l_tild < 0.44f)){// limit eq 7 millard 2013
		f = 0;
	}
	else { f = this->damp * v_tild; } // damper force according to millard 2013 
	return (f);
}
// Compute muscle torque with implicit integration method (Newton-Rhaphson). T.m_x = torque joint 1, T.m_y = torque joint 2
dVector Muscle::Compute_muscle_Torque(dFloat time)
{
	dVector T(0, 0, 0);
	theta_actual = theta_0 - this->GetAngle(); //[rad]
	theta1_actual = theta1_0 - this->GetAngle1(); //[rad]
	l_mtu = Compute_muscle_length(theta_actual, theta1_actual); // [cm] compute the muscle length

	// apply Newton-Rapshon method
	float dl(1.0e-006); // [m] delta_lce initialization
	float tol(1.0e-003); // [-] error tolerance
	float nm(100); // max iterations
	int n(0);
	//m_nmax = nm;
	//while (m_nmax >= nm - 1) {
	//	dl = dl / 10.0f;
	while ((abs(residu(dl, time)) > tol) && (n < nm)) {
		dl = dl - residu(dl, time) / dresidu(dl, time); // [cm] delta_lce - (Err/dErr)
		n++;
	}
	m_nmax = n;
	//}
	// update variable for next call
	m_Delta_l = dl;
	v = dl / time;
	m_Fmtu = F_max * (this->fSE(dl));
	if (m_Fmtu < 0) // formula 15 millard 2012
		m_Fmtu = 0;
	l_mtu = l_mtu + dl;

	// Compute reaction torque if theta_max is reached (according to Geyer)
	float delta_theta = 0, delta_theta1 = 0;
	float delta_theta_dot = 0, delta_theta1_dot = 0;
	float T_reac=0, T1_reac = 0;

	if (ang_lim !=0  && theta_actual < ang_lim)
		delta_theta = ang_lim - theta_actual;

	if (delta_theta != 0)
	{
		delta_theta_dot = delta_theta / time;
		T_reac = kj * delta_theta * (1 + delta_theta_dot / vj_max);
	}

	if (ang1_lim != 0 && theta1_actual < ang1_lim)
		delta_theta1 = ang1_lim - theta1_actual;

	if (delta_theta1 != 0)
	{
		delta_theta1_dot = delta_theta1 / time;
		T1_reac = kj * delta_theta1 * (1 + delta_theta1_dot / vj_max);
	}
	
	// Compute torque for each joint actuated by the muscle. m_x = torque joint, m_y = torque joint 1
	if (m_name == GLU || m_name ==HFL || m_name == HAM || m_name == RF)
		T.m_x = m_Fmtu * arm * 0.01 - T_reac;
	else
		T.m_x = m_Fmtu * arm * 0.01 * cos(theta_actual- phi_M) - T_reac;

	if (T.m_x < 0)// prevents unrealistic negative values due to T reaction torque
		T.m_x = 0;
	switch (Jname) 
	{
	case HIP_R:
	{
		 // GLU HAM
		switch (m_name) {
		case GLU: {T.m_x = T.m_x * (-1);break;}
		case HAM: {T.m_x = T.m_x * (-1);break;}}
		break;
	}
	case KNEE_R:
	{
		 // GAS 
		if (m_name == GAS) // VAS
			T.m_x = T.m_x * (-1);
		break;
	}
	case ANKLE_R:
	{
		//  SOL
		if (m_name == SOL) // TA
			T.m_x = T.m_x * (-1);
		break;
	}
	case HIP_L:
	{
		// GLU HAM
		switch (m_name) {
		case GLU: {T.m_x = T.m_x * (-1);break;}
		case HAM: {T.m_x = T.m_x * (-1);break;}
		}
		break;
	}
	case KNEE_L:
	{
		 // GAS 
		if (m_name == GAS) // VAS
			T.m_x = T.m_x * (-1);
		break;
	}
	case ANKLE_L:
	{
		 //  SOL
		if (m_name == SOL) // TA
			T.m_x = T.m_x * (-1);
		break;
	}
	case NOjoint: {
		T.m_x = 0;
		break;}
	}
	

	T.m_y = m_Fmtu * arm1 * 0.01 * cos(theta1_actual - phi1_M) - T1_reac;

	if (T.m_y < 0)// prevents unrealistic negative values due to T1 reaction torque
		T.m_y = 0;
	switch (Jname1)
	{
	case KNEE_R:
	{
		// HAM
		if (m_name == HAM) // RF
			T.m_y = T.m_y * (-1);
		break;
	}
	case ANKLE_R:
	{
		 // GAS
		if (m_name == GAS)
			T.m_y = T.m_y * (-1);
		break;
	}
	case KNEE_L:
	{
		// HAM
		if (m_name == HAM) // RF
			T.m_y = T.m_y * (-1);
		break;
	}
	case ANKLE_L:
	{
		 // GAS
		if (m_name == GAS)
			T.m_y = T.m_y * (-1);
		break;
	}
	case NOjoint: {
		T.m_y = 0;
		break;}
	}

	// Assign colors to muscle mesh
	lineColor.x = this->fSE(dl); lineColor.z = 1.0f - this->fSE(dl); lineColor.y = 0;
	if (m_nmax >= nm - 1)
	{
		lineColor.x = 1.0f; lineColor.z = 1.0f; lineColor.y = 1.0f;
	}
	return T;
}
// compute muscle length according to formula 11 in Optimizing locomotion controllers using biologically-based actuators and objectives - Supplemental material Wang 2012 
float Muscle::Compute_muscle_length(float jointangle, float jointangle1)
{
	float delta(0), delta1(0), l_mtu(0);
	l_mtu = l_opt + l_slk;
	if (m_name == VAS) {
		delta = rho * arm * (sin(jointangle - phi_M) - sin(phi_R - phi_M)); // knee
		l_mtu = l_mtu + delta;
	}
	else if (m_name == RF || m_name == HAM) {
		delta = rho * arm * (jointangle - phi_R); // hip
		delta1 = rho * arm1 * (sin(jointangle1 - phi1_M) - sin(phi1_R - phi1_M)); // knee
		l_mtu = l_mtu + delta + delta1;
	}
	else if (m_name == GAS) {
		delta = rho * arm * (sin(jointangle - phi_M) - sin(phi_R - phi_M)); // knee
		delta1 = rho * arm1 * (sin(jointangle1 - phi1_M) - sin(phi1_R - phi1_M)); // ankle
		l_mtu = l_mtu + delta + delta1;
	}
	else if (m_name == TA || m_name == SOL) {
		delta = rho * arm * (sin(jointangle - phi_M) - sin(phi_R - phi_M)); // knee
		l_mtu = l_mtu + delta;
	}
	else {// HFL GLU
		delta = rho * arm * (jointangle - phi_R); 
		l_mtu = l_mtu + delta;
	}
	return l_mtu;
}

void Muscle::SetThetazero(float angle)
{
	theta_0 = angle;
}

void Muscle::SetTheta1zero(float angle)
{
	theta1_0 = angle;
}

void Muscle::SetMuscleParams(const float Fmax, const float v_max, const float opt, const float slk, const float rhoin, const float r, const float r1, const float phiM, const float phiR, const float phi1M, const float phi1R)
{
	arm = r;
	arm1 = r1;
	phi_M = phiM;
	phi_R = phiR;
	phi1_M = phi1M;
	phi1_R = phi1R;
	F_max = Fmax;
	rho = rhoin;
	l_opt = opt;
	l_slk = slk;
	vm = v_max;
	lCE = l_opt; // lce initialized to optimum length
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

void Muscle::SetNeuralDelay(const float iStepSize)
{
	stepSize = iStepSize;
}

void Muscle::SetActivation(const float iExcitation)
{
	activation = 100* stepSize *(iExcitation- activation) + activation; 
}

float Muscle::GetActivation()
{
	return activation;
}

void Muscle::GetMuscleParams(float& angle, float& angle1, float& lce, float& lopt, float& lmtu,float& Fmuscle, float& Fmax,float& V)
{
	angle = theta_actual;
	angle1 = theta1_actual;
	lce = lCE;
	Fmuscle = m_Fmtu;
	V= v;
	lmtu = l_mtu;
	Fmax = F_max;
	lopt = l_opt;
}