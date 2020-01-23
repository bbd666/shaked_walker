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
	GeomNewton* GetPlantar_L();
	GeomNewton* GetToe_L();
	GeomNewton* GetThigh_R();
	GeomNewton* GetShank_R();
	GeomNewton* GetPlantar_R();
	GeomNewton* GetToe_R();
	GeomNewton* GetHip();
	GeomNewton* GetSpine();
	GeomNewton* GetShoulders();
	GeomNewton* GetNeck();
	GeomNewton* GetArm_R();
	GeomNewton* GetForearm_R();
	GeomNewton* GetHead();
	GeomNewton* GetArm_L();
	GeomNewton* GetForearm_L();

	float GetFoot2Floor_L();
	void CastFoot_L();
	void CreateFootScanLine();


private:
	WindowMain* m_winManager;

	GeomNewton* Thigh_L;
	GeomNewton* Shank_L;
	GeomNewton* Plantar_L;
	GeomNewton* Toe_L;
	dCustomHinge* Knee_L;
	dCustomBallAndSocket* Ankle_L;
	dCustomHinge* Flextoe_L;
	GeomNewton* Thigh_R;
	GeomNewton* Shank_R;
	GeomNewton* Plantar_R;
	GeomNewton* Toe_R;
	dCustomHinge* Knee_R;
	dCustomBallAndSocket* Ankle_R;
	dCustomHinge* Flextoe_R;
	GeomNewton* Hip;
	GeomNewton* Spine;
	dCustomBallAndSocket* Hip_spine;
	GeomNewton* Shoulders;
	dCustomBallAndSocket* Spine_shoulders;
	GeomNewton* Neck;
	dCustomBallAndSocket* Shoulders_neck;
	GeomNewton* Head;
	dCustomBallAndSocket* Neck_head;
	GeomNewton* Arm_R;
	dCustomBallAndSocket* Shoulders_arm_R;
	GeomNewton* Forearm_R;
	dCustomHinge* Farm_arm_R;
	GeomNewton* Arm_L;
	dCustomBallAndSocket* Shoulders_arm_L;
	GeomNewton* Forearm_L;
	dCustomHinge* Farm_arm_L;


	dModelNode* Toe_LNode;
	dModelNode* Plantar_LNode;
	dModelNode* Shank_LNode;
	dModelNode* Toe_RNode;
	dModelNode* Plantar_RNode;
	dModelNode* Shank_RNode;
	dModelNode* Thigh_RNode;
	dModelNode* Hip_Node;
	dModelNode* Spine_Node;
	dModelNode* Shoulders_Node;
	dModelNode* Neck_Node;
	dModelNode* Head_Node;
	dModelNode* Arm_RNode;
	dModelNode* Forearm_RNode;
	dModelNode* Arm_LNode;
	dModelNode* Forearm_LNode;

	float l_Thigh;
	float l_Shank;
	float r_leg;
	float l_foot;
	float w_foot;
	glm::vec3 _Pos;
	float l_toe;
	float Scale;
	float l_hip;
	float l_spine;
	float l_shoulders;
	float l_neck;
	float l_arm;
	float l_farm;
	float d_head;
	std::vector<float> masses;
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
