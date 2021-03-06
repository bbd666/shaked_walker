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
	Muscle(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* insert1, GeomNewton* insert2, dVector ins1, dVector ins2);
	virtual ~Muscle();
	glm::vec3 mColor;
	void UpdateLineCoord(Shader* cshd, dFloat steptime);
	void SetInsert1(float px, float py, float pz);
	void SetInsert2(float px, float py, float pz);
	dVector GetInsert1_GlobalRef();
	dVector GetInsert2_GlobalRef();
	void GetOriginAndInsertion(dVector &vOrigin, dVector &vInsert);
	void SetExcitation(const float iExcitation);
	void SetStepSize(const float iStepSize);
	
	void SetLength0(float l);
	float GetLength0();
	float dresidu(const float l,const float t);
	float residu(const float l,const float t);
	float fSE(const float l);
	float fCE(const float l, const float t);
	float fPE(const float l);
	void SetStiffness(float l);
	float GetStiffness();
	dVector GetForceElas(const float t);
	GeomNewton* body1;
	GeomNewton* body2;
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
	dVector m_FElas;
	dVector m_Insert1;
	dVector m_Insert2;
	int LineIndex;
	float l_opt;
	float l_slack;
	float vm;
	float lCE;
	float activation;
	float stepSize;
	int m_nmax;
	float m_Delta_l;
};


#endif //BIPED_H
