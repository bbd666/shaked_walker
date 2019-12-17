#include "pch.h"
#include "GeomGL.h"
#include "WindowGL.h"
#include "biped.h"


Muscle::Muscle(NewtonManager* wMain, GeomNewton* insert1, GeomNewton* insert2, dVector ins1, dVector ins2):
	m_Manager(wMain)
,	body1(insert1)
,   body2(insert2)
,	aVao(0)
,	aVbo(0)
,	m_Insert1(ins1)
,	m_Insert2(ins2)
{
	m_Manager->vMuscleList.push_back(this);
	m_Length0 = 0.;
	m_Stiffness = 0.f;
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
	//aVtx = new MainVertexPTN[2];
	//aVtx[0].posit.x = GetInsert1_GlobalRef().m_x;
	//aVtx[0].posit.y = GetInsert1_GlobalRef().m_y;
	//aVtx[0].posit.z = GetInsert1_GlobalRef().m_z;
	//aVtx[1].posit.x = GetInsert2_GlobalRef().m_x;
	//aVtx[1].posit.y = GetInsert2_GlobalRef().m_y;
	//aVtx[1].posit.z = GetInsert2_GlobalRef().m_z;
	//	glGenVertexArrays(1, &aVao);
	//	if (aVao) {
	//		glBindVertexArray(aVao);
	//		glGenBuffers(1, &aVbo);
	//		if (aVbo) {
	//			glBindBuffer(GL_ARRAY_BUFFER, aVbo);
	//			//
	//			glBufferData(GL_ARRAY_BUFFER, sizeof(MainVertexPTN) * 2, aVtx, GL_STATIC_DRAW);
	//			//
	//			glEnableVertexAttribArray(0);
	//			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MainVertexPTN), (void*)0);
	//			//
	//			glEnableVertexAttribArray(1);
	//			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(MainVertexPTN), (void*)(offsetof(MainVertexPTN, MainVertexPTN::uv)));
	//			//
	//			glEnableVertexAttribArray(2);
	//			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(MainVertexPTN), (void*)(offsetof(MainVertexPTN, MainVertexPTN::normal)));
	//			//
	//		}
	//		//
	//		glDisableVertexAttribArray(2);
	//		glDisableVertexAttribArray(1);
	//		glDisableVertexAttribArray(0);
	//		//
	//		glBindVertexArray(0);
	//	}
}

void Muscle::Render(Shader* cshd, dFloat steptime)
{
	//if (aVao) {
	//	//cshd->setNMat4("mat_model", aTrans);
	//	//cshd->setNVec4("diffusecol", aDiffuseColor);
	//	//

	//	//
	//	glBindVertexArray(aVao);
	//	//
	//	glEnableVertexAttribArray(0);
	//	glEnableVertexAttribArray(1);
	//	glEnableVertexAttribArray(2);
	//	//
	//	glDrawElements(GL_LINE_STRIP, 1, GL_UNSIGNED_INT, 0);
	//	//
	//	glBindVertexArray(0);
	//	glBindTexture(GL_TEXTURE_2D, 0);
	//}
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
	dVector pos1(0.0f);
	dVector VTemp(0.0f);
	dMatrix bodymatrix1;
	NewtonBodyGetPosition(NBody1, &pos1[0]);
	NewtonBodyGetMatrix(NBody1, &bodymatrix1[0][0]);
	VTemp = bodymatrix1.TransformVector(m_Insert1);
	return dVector(pos1.m_x+VTemp.m_x, pos1.m_y + VTemp.m_y, pos1.m_z + VTemp.m_z, pos1.m_w + VTemp.m_w);
}

dVector Muscle::GetInsert2_GlobalRef() {
	NewtonBody* NBody2 = (NewtonBody*)body2->GetBody();
	dVector pos2(0.0f);
	dVector VTemp(0.0f);
	dMatrix bodymatrix2;
	NewtonBodyGetPosition(NBody2, &pos2[0]);
	NewtonBodyGetMatrix(NBody2, &bodymatrix2[0][0]);
	VTemp = bodymatrix2.TransformVector(m_Insert2);
	return dVector(pos2.m_x + VTemp.m_x, pos2.m_y + VTemp.m_y, pos2.m_z + VTemp.m_z, pos2.m_w + VTemp.m_w);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Biped::Biped(NewtonManager* wMain, float posx, float posy, float posz) : 
m_Manager(wMain)
, Up_Leg_L(NULL)
, Low_Leg_L(NULL)
, Plantar_L(NULL)
, Toe_L(NULL)
, Knee_L(NULL)
, Ankle_L(NULL)
, Flextoe_L(NULL)
, _Pos(glm::vec3(posx,posy,posz))
, Scale(1.0f)
{
	string tex("Textures//wood6.png");
	l_Up_Leg = 1.0f;
	l_Low_Leg = 1.0f;
	r_leg = 0.1f;
	l_foot = 0.4f;	
	w_foot = 0.2f;
	l_toe = 0.2f;

	// create one leg
	GeomNewton* Up_Leg_L = new GeomNewton(m_Manager);
	Up_Leg_L->SetBodyType(adtDynamic);
	Up_Leg_L->SetTexture0(&tex[0], "Tex0");
	Up_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Leg_L->SetPosition(_Pos.x, _Pos.y, _Pos.z);
	Up_Leg_L->InitNewton(atCapsule, Scale*r_leg, Scale*r_leg, Scale*l_Up_Leg, 10.0f*Scale);
	GeomNewton* Low_Leg_L = new GeomNewton(m_Manager);
	Low_Leg_L->SetBodyType(adtDynamic);
	Low_Leg_L->SetTexture0(&tex[0], "Tex0");
	Low_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Leg_L->SetPosition(_Pos.x+ Scale * (l_Up_Leg+ l_Low_Leg)/2, _Pos.y, _Pos.z);
	Low_Leg_L->InitNewton(atCapsule, Scale* r_leg, Scale*  r_leg, Scale* l_Low_Leg, 10.0f);
	GeomNewton* Plantar_L = new GeomNewton(m_Manager);
	Plantar_L->SetBodyType(adtDynamic);
	Plantar_L->SetTexture0(&tex[0], "Tex0");
	Plantar_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_L->SetPosition(_Pos.x + Scale * (l_Up_Leg/2 + l_Low_Leg), _Pos.y + Scale * (l_foot/4), _Pos.z);
	Plantar_L->InitNewton(atBox, Scale*  0.1f, Scale*  l_foot, Scale* w_foot, 10.0f * Scale);
	GeomNewton* Toe_L = new GeomNewton(m_Manager);
	Toe_L->SetBodyType(adtDynamic);
	Toe_L->SetTexture0(&tex[0], "Tex0");
	Toe_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Toe_L->SetPosition(_Pos.x + Scale * (l_Up_Leg / 2 + l_Low_Leg), _Pos.y + Scale * (3*l_foot/4+l_toe/2), _Pos.z);
	Toe_L->InitNewton(atBox, Scale* 0.1f, Scale*  0.2f, Scale*  0.2f, 10.0f*Scale);
	//
	// create knee joint.
	dMatrix Knee_LPinMatrix(dGetIdentityMatrix());
	Knee_LPinMatrix = Knee_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Knee_LPinMatrix.m_posit = dVector(_Pos.x + Scale * l_Up_Leg / 2, _Pos.y, _Pos.z);
	dCustomHinge* Knee_L = new dCustomHinge(Knee_LPinMatrix, Low_Leg_L->GetBody(), Up_Leg_L->GetBody());
	m_Manager->vJointList.push_back(Knee_L);

	// create ankle joint.
	dMatrix Ankle_LPinMatrix(dGetIdentityMatrix());
	Ankle_LPinMatrix = Ankle_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Ankle_LPinMatrix.m_posit = dVector(_Pos.x + Scale * (l_Up_Leg / 2 + l_Low_Leg), _Pos.y, _Pos.z);
	dCustomBallAndSocket* Ankle_L = new dCustomBallAndSocket(Ankle_LPinMatrix, Low_Leg_L->GetBody(), Plantar_L->GetBody());
	m_Manager->vJointList.push_back(Ankle_L);

	// create toe joint.
	dMatrix Toe_LPinMatrix(dGetIdentityMatrix());
	Toe_LPinMatrix = Toe_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Toe_LPinMatrix.m_posit = dVector(_Pos.x + Scale * (l_Up_Leg / 2 + l_Low_Leg), _Pos.y + 3 * Scale*  l_foot / 4, _Pos.z);
	dCustomHinge* Flextoe_L = new dCustomHinge(Toe_LPinMatrix, Plantar_L->GetBody(), Toe_L->GetBody());
	m_Manager->vJointList.push_back(Flextoe_L);

	if (Up_Leg_L == NULL) { fprintf(stderr, "1OK1 "); }
	if (this->GetUp_Leg_L() == NULL) { fprintf(stderr, "1OK2 "); }
}


GeomNewton* Biped::GetUp_Leg_L() {
	return Up_Leg_L;
}

GeomNewton* Biped::GetLow_Leg_L() {
	return Low_Leg_L;
}

GeomNewton* Biped::GetPlantar_L() {
	return Plantar_L;
}

GeomNewton* Biped::GetToe_L() {
	return Toe_L;
}

Biped::~Biped() {
	delete Flextoe_L;
	delete Ankle_L;
	delete Knee_L;
	delete Toe_L;
	delete Plantar_L;
	delete Low_Leg_L;
	delete Up_Leg_L;
}
;