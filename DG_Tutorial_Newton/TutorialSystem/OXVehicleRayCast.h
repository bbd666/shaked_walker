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
#include "tinyxml.h"
#include "biped.h"
#include "NewtonUtil.h"

class dRaycastVHModel: public dModelRootNode
{
public:
	dRaycastVHModel(WindowMain* const winctx, const char* const modelName, const dMatrix& location, int linkMaterilID);
	~dRaycastVHModel();

	GeomNewton* GetUp_Leg_L();
	GeomNewton* GetLow_Leg_L();
	GeomNewton* GetPlantar_L();
	float GetFoot2Floor_L();
	void CastFoot(const char* const Laterality);
	float GetFoot2Floor_R();

	int CreateFootScanLine();


private:
	WindowMain* m_winManager;
	GeomNewton* Sacrum;
	GeomNewton* Spine;
	GeomNewton* Hip_L;
	GeomNewton* Up_Leg_L;
	GeomNewton* Low_Leg_L;
	GeomNewton* Plantar_L;
	GeomNewton* Toe_L;
	GeomNewton* Hip_R;
	GeomNewton* Up_Leg_R;
	GeomNewton* Low_Leg_R;
	GeomNewton* Plantar_R;
	GeomNewton* Toe_R;
	GeomNewton* Clav_L;
	GeomNewton* Up_Arm_L;
	GeomNewton* Low_Arm_L;
	GeomNewton* Hand_L;
	GeomNewton* Clav_R;
	GeomNewton* Up_Arm_R;
	GeomNewton* Low_Arm_R;
	GeomNewton* Hand_R;
	GeomNewton* Neck;
	GeomNewton* Head;
	GeomNewton* Heel_L;
	GeomNewton* Pad1_L;
	GeomNewton* Pad2_L;
	GeomNewton* Heel_R;
	GeomNewton* Pad1_R;
	GeomNewton* Pad2_R;


//	dModelNode* SacrumNode;
	dModelNode* Hip_LNode;
	dModelNode* Up_Leg_LNode;
	dModelNode* Low_Leg_LNode;
	dModelNode* Plantar_LNode;
	dModelNode* Toe_LNode;
	dModelNode* Hip_RNode;
	dModelNode* Up_Leg_RNode;
	dModelNode* Low_Leg_RNode;
	dModelNode* Plantar_RNode;
	dModelNode* Toe_RNode;
	dModelNode* Lumbar_Node;
	dModelNode* Clav_LNode;
	dModelNode* Shoulder_LNode;
	dModelNode* Elbow_LNode;
	dModelNode* Wrist_LNode;
	dModelNode* Clav_RNode;
	dModelNode* Shoulder_RNode;
	dModelNode* Elbow_RNode;
	dModelNode* Wrist_RNode;
	dModelNode* Neck_Node;
	dModelNode* Head_Node;
	dModelNode* Heel_L_Node;
	dModelNode* Pad1_L_Node;
	dModelNode* Pad2_L_Node;
	dModelNode* Heel_R_Node;
	dModelNode* Pad1_R_Node;
	dModelNode* Pad2_R_Node;


	dCustomDoubleHinge* Disk1_L;
	dCustomDoubleHinge* Rotule_L;
	dCustomHinge* Knee_L;
	dCustomBallAndSocket* Ankle_L;
	dCustomHinge* Flextoe_L;
	dCustomDoubleHinge* Disk1_R;
	dCustomDoubleHinge* Rotule_R;
	dCustomHinge* Knee_R;
	dCustomBallAndSocket* Ankle_R;
	dCustomHinge* Flextoe_R;
	dCustomDoubleHinge* Disk2;
	dCustomDoubleHinge* Strn_L;
	dCustomHinge* Elb_L;
	dCustomBallAndSocket* shld_L;
	dCustomBallAndSocket* Wr_L;
	dCustomDoubleHinge* Strn_R;
	dCustomHinge* Elb_R;
	dCustomBallAndSocket* shld_R;
	dCustomBallAndSocket* Wr_R;
	dCustomDoubleHinge* Hd;
	dCustomDoubleHinge* Nck;
	dCustomDoubleHinge* Hl_L;
	dCustomDoubleHinge* Pd1_L;
	dCustomDoubleHinge* Pd2_L;
	dCustomDoubleHinge* Hl_R;
	dCustomDoubleHinge* Pd1_R;
	dCustomDoubleHinge* Pd2_R;

	void dump_to_stdout(const char* pFilename);
	void dump_to_stdout(TiXmlNode* pParent, unsigned int indent = 0);
	int dump_attribs_to_stdout(TiXmlElement* pElement, std::vector<float> &vector, unsigned int indent);
	std::vector<float> v_scale, v_lengths, v_weight, v_masses, v_ixx, v_iyy, v_izz, v_com, v_angles, v_x1, v_y1, v_z1, v_x2, v_y2, v_z2;


	float l_Hip;
	float l_Sacrum;
	float l_Spine;
	float l_Up_Leg;
	float l_Low_Leg;
	float r_bones;
	float l_plantar;
	float w_foot;
	glm::vec3 _Pos;
	float l_toe;
	float h_foot;
	float scale_mass, scale_length;
	float l_Clav;
	float l_Up_Arm;
	float l_Low_Arm;
	float l_Hand;
	float l_Head;
	float l_Neck;
	float r_Pad;
	float h_sphere;

	Muscle* m_sol_L, * m_ta_L, * m_gas_L, * m_vas_L, * m_ham_L, * m_rf_L, * m_glu_L, * m_hfl_L;
	Muscle* m_sol_R, * m_ta_R, * m_gas_R, * m_vas_R, * m_ham_R, * m_rf_R, * m_glu_R, * m_hfl_R;

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

class DGVehicleRCManager: public dModelManager
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
