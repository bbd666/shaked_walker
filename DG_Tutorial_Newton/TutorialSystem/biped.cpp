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
{
	m_Manager->vMuscleList.push_back(this);
	m_Length0 = 1.f;
	m_Stiffness = 1000.f;
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
		glm::vec3 linecolor;
		linepos1.x = 0; linepos1.y = 0; linepos1.z = 0; 
		linepos2.x = 2.0f; linepos2.y = 2.0f; linepos2.z = 2.0f;
		linecolor.x = 1.0f; linecolor.y = 0.f; linecolor.z = 0.f;

		LineIndex=LDebug_Manager->AddLine(linepos1, linepos2, linecolor, true);
		
}

void Muscle::UpdateLineCoord(Shader* cshd, dFloat steptime)
{
	LDebug_Manager->aLineBuffer[LineIndex-1].posit.x = this->GetInsert1_GlobalRef().m_x;
	LDebug_Manager->aLineBuffer[LineIndex-1].posit.y = this->GetInsert1_GlobalRef().m_y;
	LDebug_Manager->aLineBuffer[LineIndex-1].posit.z = this->GetInsert1_GlobalRef().m_z;
	//aLineBuffer[LineIndex].color = newlinecolor;
	LDebug_Manager->aLineBuffer[LineIndex-2].posit.x = this->GetInsert2_GlobalRef().m_x;
	LDebug_Manager->aLineBuffer[LineIndex-2].posit.y = this->GetInsert2_GlobalRef().m_y;
	LDebug_Manager->aLineBuffer[LineIndex-2].posit.z = this->GetInsert2_GlobalRef().m_z;

}

dVector Muscle::GetForceElas() {
	dVector Elongation;
	Elongation = this->GetInsert2_GlobalRef() - this->GetInsert1_GlobalRef();
	m_FElas = Elongation.Scale(m_Stiffness / m_Length0);
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

 


;