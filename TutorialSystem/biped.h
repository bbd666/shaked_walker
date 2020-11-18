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
	void SetLength0(float l);
	float GetLength0();
	void SetStiffness(float l);
	float GetStiffness();
	dVector GetForceElas();
	GeomNewton* body1;
	GeomNewton* body2;
	void GenerateMesh();

private:
	NewtonManager* m_Manager;
	LineDebugManager* LDebug_Manager;
	GeomBase* aUserData;
	MainVertexPTN* aVtx;
	dVector aDiffuseColor;
	GLuint aVao;
	GLuint aVbo;
	GLuint aIbo;
	float m_Length0;
	float m_Stiffness;
	dVector m_FElas;
	dVector m_Insert1;
	dVector m_Insert2;
	int LineIndex;

};



#endif //BIPED_H
