#ifndef BIPED_H
#define BIPED_H

#pragma once
#include "pch.h"

struct Muscle
{
	Muscle(NewtonManager* wMain, GeomNewton* insert1, GeomNewton* insert2,dVector ins1, dVector ins2);
	virtual ~Muscle();
	glm::vec3 mColor;
	void Render(Shader* cshd, dFloat steptime);
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

private:
	NewtonManager* m_Manager;
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
	void GenerateMesh();
};

struct Biped
{   // public
	Biped(NewtonManager* wMain, float posx, float posy, float posz);
	virtual ~Biped();
	GeomNewton* GetUp_Leg_L();
	GeomNewton* GetLow_Leg_L();
	GeomNewton* GetPlantar_L();
	GeomNewton* GetToe_L();

private:
	NewtonManager* m_Manager;
	GeomNewton* Up_Leg_L;
	GeomNewton* Low_Leg_L;
	GeomNewton* Plantar_L;
	GeomNewton* Toe_L;	
	dCustomHinge* Knee_L;
	dCustomBallAndSocket* Ankle_L;
	dCustomHinge* Flextoe_L;

	float l_Up_Leg;
	float l_Low_Leg;
	float r_leg;
	float l_foot;
	float w_foot;
	glm::vec3 _Pos;
	float l_toe;
	float Scale;
};



#endif //BIPED_H
