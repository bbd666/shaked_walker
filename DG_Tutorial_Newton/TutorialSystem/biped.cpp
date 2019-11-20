#include "pch.h"
#include "GeomGL.h"
#include "WindowGL.h"
#include "biped.h"

Biped::Biped(NewtonManager* wMain, float posx, float posy, float posz) : Up_Leg_L(NULL)
, Low_Leg_L(NULL)
, Plantar_L(NULL)
, Toe_L(NULL)
, Knee_L(NULL)
, Ankle_L(NULL)
, Flextoe_L(NULL)
, _Pos(glm::vec3(4.0f,3.0f,0.0f))
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
	GeomNewton* Up_Leg_L = new GeomNewton(wMain);
	Up_Leg_L->SetBodyType(adtDynamic);
	Up_Leg_L->SetTexture0(&tex[0], "Tex0");
	Up_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Up_Leg_L->SetPosition(_Pos.x, _Pos.y, _Pos.z);
	Up_Leg_L->InitNewton(atCapsule, Scale*r_leg, Scale*r_leg, Scale*l_Up_Leg, 10.0f*Scale);
	GeomNewton* Low_Leg_L = new GeomNewton(wMain);
	Low_Leg_L->SetBodyType(adtDynamic);
	Low_Leg_L->SetTexture0(&tex[0], "Tex0");
	Low_Leg_L->SetDiffuseColor(0.7f, 0.7f, 0.7f);
	Low_Leg_L->SetPosition(_Pos.x+ Scale * (l_Up_Leg+ l_Low_Leg)/2, _Pos.y, _Pos.z);
	Low_Leg_L->InitNewton(atCapsule, Scale* r_leg, Scale*  r_leg, Scale* l_Low_Leg, 10.0f);
	GeomNewton* Plantar_L = new GeomNewton(wMain);
	Plantar_L->SetBodyType(adtDynamic);
	Plantar_L->SetTexture0(&tex[0], "Tex0");
	Plantar_L->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	Plantar_L->SetPosition(_Pos.x + Scale * (l_Up_Leg/2 + l_Low_Leg), _Pos.y + Scale * (l_foot/4), _Pos.z);
	Plantar_L->InitNewton(atBox, Scale*  0.1f, Scale*  l_foot, Scale* w_foot, 10.0f * Scale);
	GeomNewton* Toe_L = new GeomNewton(wMain);
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
	wMain->vJointList.push_back(Knee_L);

	// create ankle joint.
	dMatrix Ankle_LPinMatrix(dGetIdentityMatrix());
	Ankle_LPinMatrix = Ankle_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Ankle_LPinMatrix.m_posit = dVector(_Pos.x + Scale * (l_Up_Leg / 2 + l_Low_Leg), _Pos.y, _Pos.z);
	dCustomBallAndSocket* Ankle_L = new dCustomBallAndSocket(Ankle_LPinMatrix, Low_Leg_L->GetBody(), Plantar_L->GetBody());
	wMain->vJointList.push_back(Ankle_L);

	// create knee joint.
	dMatrix Toe_LPinMatrix(dGetIdentityMatrix());
	Toe_LPinMatrix = Toe_LPinMatrix * dYawMatrix(90.0f * dDegreeToRad);
	Toe_LPinMatrix.m_posit = dVector(_Pos.x + Scale * (l_Up_Leg / 2 + l_Low_Leg), _Pos.y + 3 * Scale*  l_foot / 4, _Pos.z);
	dCustomHinge* Flextoe_L = new dCustomHinge(Toe_LPinMatrix, Plantar_L->GetBody(), Toe_L->GetBody());
	wMain->vJointList.push_back(Flextoe_L);
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