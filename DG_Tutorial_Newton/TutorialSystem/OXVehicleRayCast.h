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
#ifndef _OXVEHICLE_RAYCAST__H_
#define _OXVEHICLE_RAYCAST__H_

#include "pch.h"
#include "NewtonManager.h"
#include "WindowGL.h"
#include "biped.h"
#include "NewtonUtil.h"

class dRaycastVHModel : public dModelRootNode
{
public:
	dRaycastVHModel(WindowMain* const winctx, const char* const modelName, const dMatrix& location, int linkMaterilID);
	~dRaycastVHModel();

	GeomNewton* GetUp_Leg_L();
	GeomNewton* GetLow_Leg_L();
	GeomNewton* GetPlantar_L();
	float GetFoot2Floor_L();
	GeomNewton* GetUp_Leg_R();
	GeomNewton* GetLow_Leg_R();
	GeomNewton* GetPlantar_R();
	dCustomHingeActuator* GetAnkle_L();
	float GetFoot2Floor_R();




private:
	WindowMain* m_winManager;
	GeomNewton* Low_Leg_L;
	GeomNewton* Plantar_L;
	dModelNode* Plantar_LNode;



	dCustomHingeActuator* Ankle_L;



	float l_Hip;
	float l_Sacrum;
	float l_Spine;
	float l_Up_Leg;
	float l_Low_Leg;
	float r_bones;
	float l_foot;
	float w_foot;
	glm::vec3 _Pos;
	float l_toe;
	float Scale;
	float l_Clav;
	float l_Up_Arm;
	float l_Low_Arm;
	float l_Hand;
	float l_Head;
	float l_Neck;

	Muscle* m1;
	dVector ins11;
	dVector ins12;

	dVector  ContactFoot_L;
	dVector  NormalFoot_L;
	dVector  ContactGround_L;
	float Foot2Floor_L;
	int FootLineIndex_L, FootLineIndex_R;

	dVector  ContactFoot;
	dVector  NormalFoot;
	dVector  ContactGround;
	float Foot2Floor_R;
};

class DGVehicleRCManager : public dModelManager
{
public:
	DGVehicleRCManager(WindowMain* winctx);
	virtual ~DGVehicleRCManager();
	//
	virtual void OnPreUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const;
	virtual void OnPostUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const;
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
