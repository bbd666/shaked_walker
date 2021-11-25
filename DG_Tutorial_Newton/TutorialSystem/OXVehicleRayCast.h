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

	float GetFoot2Floor_L();
	void CastFoot(const char* const Laterality);
	float GetFoot2Floor_R();

	int CreateFootScanLine();


private:
	WindowMain* m_winManager;
	GeomNewton* UP_leg;
	GeomNewton* LPT;
	GeomNewton* Plantar_L;
	GeomNewton* Plantar_R;

	dModelNode* N1;

	dCustomHinge* Hip;

	void dump_to_stdout(const char* pFilename);
	void dump_to_stdout(TiXmlNode* pParent, unsigned int indent = 0);
	int dump_attribs_to_stdout(TiXmlElement* pElement, std::vector<float> &vector, unsigned int indent);
	int dump_attribs_to_stdout2(TiXmlElement* pElement, std::map<std::string, float>& l, unsigned int indent);
	std::vector<float> v_scale, v_lengths, v_total_weight, v_masses, v_ixx, v_iyy, v_izz, v_com, v_angles, v_x1, v_y1, v_z1, v_x2, v_y2, v_z2, v_maxForce;
	
	map<std::string, float> lengths;
	map<std::string, float> mass_distrib,delta_cm;
	map<std::string, float> Ixx,Iyy,Izz;
	std::vector<std::string> lengths_keys = { "Head","UpArm","ForeArm","Hand","Trunk","Thigh","Shank","Foot","Toes","UPT","MPT","LPT","Hip","Shoulder","FootBreadth","AnkleHeight","Neck", "RadBones","RadSpheres" };
	std::vector<std::string> mass_keys = { "Head","UpArm","ForeArm","Hand","Trunk","Thigh","Shank","Foot","Toes","UPT","MPT","LPT"};

	float l_Hip;
	float l_Up_Leg;
	float l_Low_Leg;
	float r_bones;
	float l_foot;
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
	float r_Pad;
	float h_sphere;
	float 	l_UPT, l_MPT, l_LPT, l_trunk, l_neck, l_delta, ankle_j;
	Muscle* m_sol_L, * m_ta_L, * m_gas_L, * m_vas_L, * m_ham_L, * m_rf_L, * m_glu_L, * hfl;
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
