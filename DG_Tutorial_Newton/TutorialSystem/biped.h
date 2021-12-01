#ifndef BIPED_H
#define BIPED_H

#pragma once
#include "pch.h"

#include "Shader.h"
#include "Camera.h"
#include "VertexGL.h"
#include "WindowGL.h"

enum JointType {Hinge, BallAndSocket, DoubleHinge, NOtype};
enum Mtuname {GLU, HAM, HFL, RF, VAS, GAS, SOL, TA};

struct Muscle
{
	Muscle(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* insert1, GeomNewton* insert2, GeomNewton* insert3, dVector ins1, dVector ins2, JointName jname, JointType jtype, JointName jname1, JointType jtype1, Mtuname mname);
	virtual ~Muscle();
	Mtuname GetMuscleName();
	glm::vec3 mColor;
	void UpdateLineCoord(Shader* cshd, dFloat steptime);
	void SetInsert1(float px, float py, float pz);
	void SetInsert2(float px, float py, float pz);
	dVector GetInsert1_GlobalRef();
	dVector GetInsert2_GlobalRef();
	void GetOriginAndInsertion(dVector &vOrigin, dVector &vInsert);
	void SetActivation(const float iExcitation);
	float GetActivation();
	void GetMuscleParams(float& angle, float& angle1, float& lce, float& Fmuscle, float& V, float& lmtu);
	void SetMaxJointAngle(const float max);
	float GetMaxJointAngle();
	void SetNeuralDelay(const float iStepSize);
	
	void SetLength0(float l);
	float GetAngle();
	void SetAngle(float ang);
	float GetAngle1();
	void SetAngle1(float ang);
	float GetLength0();
	float dresidu(const float l,const float t);
	float residu(const float l,const float t);
	float fSE(const float l);
	float fCE(const float l, const float t);
	float fPE(const float l);
	float fDE(const float l, const float t);
	dVector Compute_muscle_Torque(dFloat time);
	float Compute_muscle_length(float jointangle, float jointangle1);
	void SetThetazero(float angle);
	void SetTheta1zero(float angle);
	void SetMuscleParams(const float Fmax, const float v_max, const float opt, const float slk, const float rhoin, const float r, const float r1, const float phiM, const float phiR, const float phi1M, const float phi1R);
	GeomNewton* body1;
	GeomNewton* body2;
	GeomNewton* body3;
	void GenerateMesh();
	float GetNmax();
	float GetDelta_l();
	void SetDelta_l(const float l);
	float GetLCE();
	void SetLCE(const float l);
	JointName Jname;
	JointType Jtype;
	JointName Jname1;
	JointType Jtype1;
	Mtuname m_name;

private:
	NewtonManager* m_Manager;
	LineDebugManager* LDebug_Manager;
	GeomBase* aUserData;
	MainVertexPTN* aVtx;
	glm::vec3 lineColor;
	GLuint aVao;
	GLuint aVbo;
	GLuint aIbo;
	float m_Length0;
	float m_Stiffness;
	dVector m_Insert1;
	dVector m_Insert2;
	int LineIndex;
	float l_opt; // [cm] optimum muscle length
	float v; // [cm/s]  muscle velocity
	float vm; // [cm/s] max muscle velocity
	float lCE; // [cm] contractile element length
	float activation; // activation level of the muscle [0-1]
	double stepSize;
	int m_nmax;
	float m_Delta_l; // [cm] muscle increment
	float arm; // [cm] maximum moment arm of the muscle
	float arm1; // [cm] maximum moment arm of the muscle. arm 1 refers to the lowest joint of the muscle (proximal ref system)
	float phi_M; // [rad] joint angle with maximum moment arm
	float phi_R; // [rad] joint angle in which delta = 0 (l_mtu = l_opt + l_slk)
	float phi1_M; // [rad] joint 1 angle with maximum moment arm. Joint 1 is the lowest joint of the muscle (proximal ref system)
	float phi1_R; // [rad] joint 1 angle in which delta = 0 (l_mtu = l_opt + l_slk)
	float F_max; // max isometric force [N]
	float m_Fmtu;
	float rho; // accounts for pennation angle
	float l_slk; // slack length
	float l_mtu;
	float theta_actual; // actual angle of the joint
	float theta1_actual; // actual angle of the joint 1
	float theta_0; // initial angle of the joint
	float theta1_0; // initial angle of the joint 1
	float theta_min; // max joint angle
	float damp;
	float Jang; // angle of the joint
	float Jang1; // angle of the joint 1
};
#endif //BIPED_H
