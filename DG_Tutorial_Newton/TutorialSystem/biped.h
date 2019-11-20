#ifndef BIPED_H
#define BIPED_H

#pragma once
#include "pch.h"

struct Biped
{   // public
	Biped(NewtonManager* wMain, float posx, float posy, float posz);
	virtual ~Biped();

private:
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
