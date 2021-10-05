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
	void SetExcitation(const float iExcitation);
	void GetMuscleParams(double& angle, double& lce, double& Fmuscle);
	void SetStepSize(const float iStepSize);
	
	void SetLength0(float l);
	float GetLength0();
	float dresidu(const float l,const float t);
	float residu(const float l,const float t);
	float fSE(const float l);
	float fCE(const float l, const float t);
	float fPE(const float l);
	double Compute_muscle_Torque(dFloat time);
	double Compute_muscle_length(double jointangle);
	void SetThetazero(double angle);
	void SetMuscleParams(const double r, const double phiM, const double phiR, const double Fmax, const double rhoin, const double opt, const double slk);
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
	float vm; // [cm/s] muscle velocity
	float lCE; // [cm] contractile element length
	float activation; // activation level of the muscle [0-1]
	float stepSize;
	int m_nmax;
	float m_Delta_l; // [cm] muscle increment
	double arm; // [cm] maximum moment arm of the muscle
	double phi_M; // [rad] joint angle with maximum moment arm
	double phi_R; // [rad] joint angle in which delta = 0 (l_mtu = l_opt + l_slk)
	double F_max; // max isometric force [N]
	double m_Fmtu;
	double rho; // accounts for pennation angle
	double l_slk; // slack length
	double theta_actual; // actual angle of the joint
	double theta_0; // initial angle of the joint
};
#endif //BIPED_H
