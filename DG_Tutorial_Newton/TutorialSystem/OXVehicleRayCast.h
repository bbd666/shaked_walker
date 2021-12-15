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
#include "dCustomJoint.h"
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

	map<std::string, GeomNewton*> rigid_element;//  geom newton as map: use mass_keys as string
	map<std::string, dModelNode*> nodes;// nodes as map: use mass_keys as string
	map<std::string, dVector*> body_pos;// vector of body position
	map<std::string, dVector*> body_dim;// vector of body dimension
	map<std::string, std::string> child_father;// bodies tree
		 
	map<std::string, Muscle*> muscles;//  muscle as map: use muscle_keys as string
	std::vector<std::string> muscle_keys = { "hfl_r","glu_r","ham_r", "rf_r", "vas_r","sol_r", "ta_r", "gas_r", "hfl_l","glu_l","ham_l", "rf_l", "vas_l", "sol_l", "ta_l", "gas_l"};
	map<std::string, std::string> m_body1;// body one of muscle
	map<std::string, std::string> m_body2;// body 2 of muscle
	map<std::string, std::string> m_body3;// body 3 of muscle
	map<std::string, dVector> m_point1;// point 1 line muscle
	map<std::string, dVector> m_point2;// point 2 line muscle
	map<std::string, JointName> m_joint1;// joint 1 muscle
	map<std::string, JointName> m_joint2;// joint 2 muscle
	map<std::string, JointType> m_joint_type1;// joint type 1 muscle
	map<std::string, JointType> m_joint_type2;// joint type 2 muscle
	map<std::string, Mtuname> m_muscle_name;// name of muscle
	map<std::string, map<std::string, float>> m_list_prop;// list containing the list of muscle properties (crazy ;))

	map<std::string, float> m_theta0;// initial angle 1 of muscle
	map<std::string, float> m_theta10;// initial angle 2 of muscle
	map<std::string, float> m_lim1;// limit angle 1 of muscle 
	map<std::string, float> m_lim2;// limit angle 2 of muscle 

	std::vector<std::string> muscle_p_keys = { "Fmax", "v_max", "lopt","lslk", "rho", "r", "r1", "phiM", "phiR", "phi1M", "phi1R" };
	map<std::string, float> sol, ta, gas, vas, hfl, glu, ham, rf;

	dCustomDoubleHinge* Hip_r;
	dCustomHinge* Knee_r;
	//dCustomBallAndSocket* Ankle;
	dCustomDoubleHinge* Ankle_r;

	dCustomDoubleHinge* Hip_l;
	dCustomHinge* Knee_l;
	//dCustomBallAndSocket* Ankle;
	dCustomDoubleHinge* Ankle_l;

	GeomNewton* Plantar_L;
	GeomNewton* Plantar_R;

	void dump_to_stdout(const char* pFilename);
	void dump_to_stdout(TiXmlNode* pParent, unsigned int indent = 0);
	const char* getIndent(unsigned int numIndents);
	const char* getIndentAlt(unsigned int numIndents);
	int dump_attribs_to_stdout(TiXmlElement* pElement, std::vector<float> &vector, unsigned int indent);
	int dump_attribs_to_stdout2(TiXmlElement* pElement, std::map<std::string, float>& l, unsigned int indent);

	std::vector<float> v_scale, v_lengths, v_total_weight, v_masses, v_ixx, v_iyy, v_izz, v_com, v_angles, v_x1, v_y1, v_z1, v_x2, v_y2, v_z2, v_maxForce;
	
	map<std::string, float> lengths;
	map<std::string, float> mass_distrib,delta_cm;
	map<std::string, float> Ixx, Iyy, Izz, body_rot_ang;
	std::vector<std::string> lengths_keys = { "Head","UpArm","ForeArm","Hand","Trunk","Thigh","Shank","Foot","Toes","UPT","MPT","LPT","Hip","Shoulder","FootBreadth","AnkleHeight","Neck", "RadBones","RadSpheres" };
	std::vector<std::string> mass_keys = { "Head","UpArm","ForeArm","Hand","Trunk","Thigh","Shank","Foot","Toes","UPT","MPT","LPT"};
	std::vector<std::string> body_keys = { "LPT","Thigh_r","Shank_r", "Foot_r","Thigh_l","Shank_l", "Foot_l" };

	float l_Hip;
	float l_Up_Leg;
	float l_Low_Leg;
	float r_bones;
	float l_foot;
	float w_foot;
	
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
	float MTU_excitation_signal(const Mtuname m_name) const;// the ouput is the excitation signal for muscle m_name
	//
	dRaycastVHModel* m_player;
	dCustomJoint::dAngularIntegration* m_curJointAngle;

private:
	WindowMain* m_winManager;
};


#endif //OXVEHICLE_RAYCAST__H_
