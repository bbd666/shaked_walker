#ifndef MUSCLEV2_H
#define MUSCLEV2_H

#pragma once
#include "pch.h"

#include "Shader.h"
#include "Camera.h"
#include "VertexGL.h"
#include "WindowGL.h"
#include <iostream>
#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>
struct MuscleV2
{
	MuscleV2(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* origin_body, GeomNewton* instertion_body, GeomNewton* sliding_body, dVector origin, dVector insertion, dVector insertion2);
	virtual ~MuscleV2();
	glm::vec3 mColor;
	void UpdateLineCoord(Shader* cshd, dFloat steptime);
	void SetOrigin(float px, float py, float pz);
	void SetInsertion(float px, float py, float pz);
	dVector GetOrigin_GlobalRef();
	dVector GetInsertion_GlobalRef();
	dVector GetInsertion2_GlobalRef();
	void SetLength0(float l);
	float GetLength0();
	void SetStiffness(float kpe, float kbe, float kse);
	void SetLopt(float L);
	float GetLopt();
	void SetLslack(float L);
	void SetinitialVel(double x);
	float GetLslack();
	void SetFmax(float L);
	float GetFmax();
	void SetVmax(float V);
	float GetVmax();
	dVector GetForceMTU_V1();
	GeomNewton* body1;
	GeomNewton* body2;
	GeomNewton* body3;
	void GenerateMesh();

private:
	NewtonManager* m_Manager;
	LineDebugManager* LDebug_Manager;
	GeomBase* aUserData;
	dVector aDiffuseColor;
	float m_Length0;
	float m_Lopt;
	float m_Lslack; // [m] type hill muscle slack length
	float m_PE; // [N/m] type hill parallel stiffness
	float m_BE; // [N/m] type hill buffer stiffness
	float m_SE; // [N/m] type hill series stiffness
	float m_CE; // [N] type hill contractive force
	float m_Stiffness;
	float m_Fmax;
	float m_Vmax;
	double m_vint;
	std::vector<float> m_Arm;
	dVector m_FElas;
	dVector m_origin;
	dVector m_insertion;
	dVector m_insertion2;
	int LineIndex;
	int LineIndex1;
	float m_lce;
	dFloat m_time_prec;

};



#endif //MUSCLEV2_H

