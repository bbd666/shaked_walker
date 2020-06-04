#ifndef MUSCLE_H
#define MUSCLE_H

#pragma once
#include "pch.h"

#include "Shader.h"
#include "Camera.h"
#include "VertexGL.h"
#include "WindowGL.h"
#include <iostream>
//#include <boost/array.hpp>

#include <boost/numeric/odeint.hpp>
//#include <boost/numeric/odeint/stepper/XYZ.hpp> //- the include path for all steppers, XYZ is a placeholder for a stepper. 
//#include <boost/numeric/odeint/algebra/XYZ.hpp>// - all algebras. 
//#include <boost/numeric/odeint/util/XYZ.hpp> //- the utility functions like is_resizeable, same_size, or resize. 
//#include <boost/numeric/odeint/integrate/XYZ.hpp>//- the integrate routines. 
//#include <boost/numeric/odeint/iterator/XYZ.hpp> //- the range and iterator functions. 
//#include <boost/numeric/odeint/external/XYZ.hpp>// - any binders to external libraries. 
struct Muscle
{
	Muscle(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* origin_body, GeomNewton* instertion_body, dVector origin, dVector insertion, dVector insertion2);
	virtual ~Muscle();
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
	dVector GetForceElas();
	dVector GetForceMTU_V2(dFloat t, dModelRootNode* model);
	GeomNewton* body1;
	GeomNewton* body2;
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
	dFloat m_time;
	double m_vce;
};



#endif //MUSCLE_H
