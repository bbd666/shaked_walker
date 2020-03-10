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
#include "biped.h"

class dRaycastVHModel: public dModelRootNode
{
public:
	dRaycastVHModel(WindowMain* const winctx, const char* const modelName, const dMatrix& location, int linkMaterilID);
	~dRaycastVHModel();

	GeomNewton* GetThigh_L();
	GeomNewton* GetShank_L();
	GeomNewton* GetPlantarC_L();
	GeomNewton* GetToesC_L();
	GeomNewton* GetThigh_R();
	GeomNewton* GetShank_R();
	GeomNewton* GetPlantarC_R();
	GeomNewton* GetToesC_R();
	GeomNewton* GetTrunk();
	GeomNewton* GetHead();
	GeomNewton* GetArm_R();
	GeomNewton* GetForearm_R();
	GeomNewton* GetHand_R();
	GeomNewton* GetArm_L();
	GeomNewton* GetForearm_L();
	GeomNewton* GetHand_L();
	float GetFoot2Floor_L();
	void CastFoot_L();
	void CreateFootScanLine();
	GeomNewton* GetTrunkCompound();
	
private:
	WindowMain* m_winManager;
	NewtonManager* aManager;

	GeomNewton* Thigh_L;
	GeomNewton* Shank_L;
	GeomNewton* PlantarC_L;
	GeomNewton* ToesC_L;
	dCustomHinge* Knee_L;
	dCustomBallAndSocket* Ankle_L;
	dCustomHinge* Toes_L;
	GeomNewton* Thigh_R;
	GeomNewton* Shank_R;
	GeomNewton* PlantarC_R;
	GeomNewton* ToesC_R;
	dCustomHinge* Knee_R;
	dCustomBallAndSocket* Ankle_R;
	dCustomHinge* Toes_R;
	GeomNewton* Trunk;
	GeomNewton* Head;
	dCustomBallAndSocket* Head_trunk;
	GeomNewton* Arm_R;
	dCustomBallAndSocket* Shoulders_arm_R;
	GeomNewton* Forearm_R;
	dCustomHinge* Farm_arm_R;
	GeomNewton* Hand_R;
	dCustomBallAndSocket* Hand_arm_R;
	GeomNewton* Arm_L;
	dCustomBallAndSocket* Shoulders_arm_L;
	GeomNewton* Forearm_L;
	dCustomHinge* Farm_arm_L;
	GeomNewton* Hand_L;
	dCustomBallAndSocket* Hand_arm_L;
	GeomNewton* Hip_L;
	GeomNewton* Hip_R;
	dCustomHinge* Hip_Trunk_R;
	dCustomHinge* Hip_Trunk_L;
	GeomNewton* TrunkCompound;

	dModelNode* ToesC_LNode;
	dModelNode* PlantarC_LNode;
	dModelNode* Shank_LNode;
	dModelNode* ToesC_RNode;
	dModelNode* PlantarC_RNode;
	dModelNode* Shank_RNode;
	dModelNode* Thigh_RNode;
	dModelNode* Hip_Node;
	dModelNode* Trunk_Node;
	dModelNode* Head_Node;
	dModelNode* Arm_RNode;
	dModelNode* Forearm_RNode;
	dModelNode* Hand_RNode;
	dModelNode* Arm_LNode;
	dModelNode* Forearm_LNode;
	dModelNode* Hand_LNode;
	dModelNode* Hip_R_Node;
	dModelNode* Hip_L_Node;
	dModelNode* TrunkCompoundNode;

	float l_Shank;
	float l_Thigh;
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

	Muscle* m1;
	dVector ins11;
	dVector ins12;

	dVector  ContactFoot_L;
	dVector  NormalFoot_L;
	dVector  ContactGround_L;
	float Foot2Floor_L;
	int FootLineIndex_L;

	MainVertexPTN* aVtx;
	float aTexTileU;
	float aTexTileV;
	unsigned int* aIndices;
	int aIndiceCount;
};

class DGVehicleRCManager: public dModelManager
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
};


#endif //OXVEHICLE_RAYCAST__H_
