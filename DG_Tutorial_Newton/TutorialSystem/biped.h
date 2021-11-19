#ifndef BIPED_H
#define BIPED_H

#pragma once
#include "pch.h"

#include "Shader.h"
#include "Camera.h"
#include "VertexGL.h"
#include "WindowGL.h"



struct Muscle
{
	Muscle(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* insert1, GeomNewton* insert2, dCustomHinge* jointact, dVector ins1, dVector ins2);
	virtual ~Muscle();
	glm::vec3 mColor;
	void UpdateLineCoord(Shader* cshd, dFloat steptime);
	void SetInsert1(float px, float py, float pz);
	void SetInsert2(float px, float py, float pz);
	dVector GetInsert1_GlobalRef();
	dVector GetInsert2_GlobalRef();
	void GetOriginAndInsertion(dVector &vOrigin, dVector &vInsert);
	void SetActivation(const float iExcitation);
	float GetActivation();
	void GetMuscleParams(float& angle, float& lce, float& Fmuscle, float& V, float& lmtu);
	void SetMaxJointAngle(const float max);
	float GetMaxJointAngle();
	void SetNeuralDelay(const float iStepSize);
	
	void SetLength0(float l);
	float GetLength0();
	float dresidu(const float l,const float t);
	float residu(const float l,const float t);
	float fSE(const float l);
	float fCE(const float l, const float t);
	float fPE(const float l);
	float fDE(const float l, const float t);
	float Compute_muscle_Torque(dFloat time);
	float Compute_muscle_length(float jointangle);
	void SetThetazero(float angle);
	void SetMuscleParams(const float r, const float phiM, const float phiR, const float Fmax, const float rhoin, const float opt, const float slk, const float v_max);
	GeomNewton* body1;
	GeomNewton* body2;
	dCustomHinge* joint;
	void GenerateMesh();
	float GetNmax();
	float GetDelta_l();
	void SetDelta_l(const float l);
	float GetLCE();
	void SetLCE(const float l);

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
	float phi_M; // [rad] joint angle with maximum moment arm
	float phi_R; // [rad] joint angle in which delta = 0 (l_mtu = l_opt + l_slk)
	float F_max; // max isometric force [N]
	float m_Fmtu;
	float rho; // accounts for pennation angle
	float l_slk; // slack length
	float l_mtu;
	float theta_actual; // actual angle of the joint
	float theta_0; // initial angle of the joint
	float theta_min; // max joint angle
};
#endif //BIPED_H
