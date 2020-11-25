#ifndef MUSCLEV2_H
#define MUSCLEV2_H

#pragma once
#include "pch.h"

#include "Shader.h"
#include "Camera.h"
#include "VertexGL.h"
#include "WindowGL.h"
#include <iostream>

struct MuscleV2
{
	MuscleV2(NewtonManager* wMain, GeomNewton* first_body, GeomNewton* second_body);
	virtual ~MuscleV2();
	float GetForceMTU_V2(dFloat t);
	GeomNewton* body1;
	GeomNewton* body2;
private:
	NewtonManager* m_Manager;
	float m_Fmax;
	float m_Vmax;
	float p_initial;
	dFloat m_time;
};

#endif //MUSCLEV2_H

