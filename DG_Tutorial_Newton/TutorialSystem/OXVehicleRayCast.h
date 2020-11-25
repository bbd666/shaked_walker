/*
* Copyright(c) < 1999 - 2020 > <Dave Gravel, Free Tutorial - Newton Dynamics and more.>
*
* This software is provided 'as-is', without any express or implied
* warranty.In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
*including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions :
*
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software.If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
*
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
*
* 3. This notice may not be removed or altered from any source distribution.
*/
//
#ifndef OXVEHICLE_RAYCAST__H_
#define OXVEHICLE_RAYCAST__H_
#pragma once
#include "pch.h"
#include "NewtonManager.h"
#include "WindowGL.h"
#include "muscle.h"
#include "MuscleV2.h"

class dRaycastVHModel : public dModelRootNode
{
public:
	dRaycastVHModel(WindowMain* const winctx, const char* const modelName, const dMatrix& location, int linkMaterilID);
	~dRaycastVHModel();
	GeomNewton* GetThigh_L();
	GeomNewton* GetShank_L();
	dCustomHinge* GetKnee_L();
	GeomNewton* GetCE_KL();
	void AddMuscleV2_Element(GeomNewton* body1, GeomNewton* body2);
	
private:
	WindowMain* m_winManager;
	NewtonManager* aManager;
	GeomNewton* Thigh_L;
	GeomNewton* Shank_L;
	dModelNode* Shank_LNode;
	dCustomHinge* Knee_L;
	GeomNewton* CE_KL;
	dModelNode* CE_KLNode;
	GeomNewton* link;
	dModelNode* linkNode;
	dCustomSlider* slider;

	float l_Thigh;
	float l_Shank;
	float r_leg;
	float l_foot;
	float w_foot;
	glm::vec3 _Pos;
	float l_toe;
	float Scale;
	float l_hip;
	float l_trunk;
	float l_shoulders;
	float l_head;
	float l_arm;
	float l_farm;
	float l_hand;

	std::vector<float> masses;
	std::vector<float> Ixx;
	std::vector<float> Iyy;
	std::vector<float> Izz;
	float tot_w;
	float h_foot;

	Muscle* VAS_L;
	MuscleV2* VAS_L2;
	dVector insVAS_L11;
	dVector insVAS_L12;

	MainVertexPTN* aVtx;
	float aTexTileU;
	float aTexTileV;
	unsigned int* aIndices;
	int aIndiceCount;
};

class DGVehicleRCManager : public dModelManager
{
public:
	DGVehicleRCManager(WindowMain* winctx);
	virtual ~DGVehicleRCManager();
	//
	virtual void OnPreUpdate(dModelRootNode* const model, dFloat timestep) const;
	virtual void OnPostUpdate(dModelRootNode* const model, dFloat timestep) const;
	virtual void OnUpdateTransform(const dModelNode* const bone, const dMatrix& localMatrix) const;
	virtual void OnDebug(dModelRootNode* const model, dCustomJoint::dDebugDisplay* const debugContext);
	//
	dModelRootNode* CreateWalkerPlayer(const char* const modelName, const dMatrix& location);
	//
	dRaycastVHModel* m_player;
private:
	WindowMain* m_winManager;
	GeomNewton* temp;
};


#endif //OXVEHICLE_RAYCAST__H_
