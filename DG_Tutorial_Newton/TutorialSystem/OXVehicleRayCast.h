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
#include "ControlAlgorithm.h"

class dRaycastVHModel: public dModelRootNode
{
public:
	dRaycastVHModel(WindowMain* const winctx, const char* const modelName, const dMatrix& location, int linkMaterilID);
	~dRaycastVHModel();

	void DrawFrame(const dVector& posit, dFloat scale);
	//virtual void DrawFrame(const dMatrix& matrix, dFloat scale);
	int CreateLine();
	void CreateDescreteContactFootCollision(NewtonBody* const footBody) const;
	vector<dFloat> GetTrunkSagittalState();
	vector<dFloat> GetTrunkCoronalState();
	vector<dFloat> GetFootCoronalState(string body);
	float ComputeAngleBWVectors(dVector const& dir, dVector const& ref, dVector const& crossref);
	dVector ComputePlayerCOM();
	dVector ComputePlayerCOMvelocity();
	GeomNewton* Get_Floor();
	GeomNewton* Get_box();
	map<std::string, GeomNewton*> Get_RigidElemetList();
	map<std::string, dCustomHinge*> Get_HingeJointList();
	map<std::string, dCustomDoubleHinge*> Get_DoubleHingeJointList();
	float GetLegLength();
	ControlAlgorithm controller;
	void AddActionReactionTorque(float Torque, dVector pin, GeomNewton* b1, GeomNewton* b2);

private:
	WindowMain* m_winManager;
	void AssemblyCreation();
	void TrunkCreation();
	void LegCreation(string trunk, string thigh, string shank, string foot, string toes, string hip, string knee, string ankle, string toe_hinge);
	void RigidBodyCreation(string body);
	void HingeJoint(string jname, string body1, string body2, dVector pos, float minAng, float maxAng);
	void DoubleHingeJoint(string jname, string body1, string body2, dVector pos, float minAng1, float maxAng1, float minAng2, float maxAng2);
	
	float LPT_A ,ThighR_A,ThighL_A,Shank_A ,Foot_A;

	/// <RIGID ELEMENTS>
	GeomNewton* box;
	GeomNewton* Geomfloor;
	std::vector<std::string> body_keys = { "LPT","Thigh_r","Shank_r", "Foot_r","Thigh_l","Shank_l", "Foot_l","MPT", "UPT", "Toe_r", "Toe_l"};//  RIGID ELEMENTS OF THE MODEL
	map<std::string, GeomNewton*> rigid_element;//  geom newton as map: use mass_keys as string
	map<std::string, dModelNode*> nodes;// nodes as map: use mass_keys as string
	map<std::string, dVector*> body_pos;// vector of body position
	map<std::string, dVector*> body_dim;// vector of body dimension
	map<std::string, std::string> child_father = { {"Thigh_r", "LPT"}, {"Shank_r", "Thigh_r"}, {"Foot_r", "Shank_r"},
		{"Thigh_l", "LPT"}, {"Shank_l", "Thigh_l"}, {"Foot_l", "Shank_l"},
		{"MPT", "LPT"}, {"UPT", "MPT"},
		{"Toe_r", "Foot_r"}, {"Toe_l", "Foot_l"} };// list with body child and father relationship;// bodies tree
	int COMXline_id;// id of line passing though com
	int COMYline_id;// id of line passing though com
	int COMZline_id;// id of line passing though com
	/// </RIGID ELEMENTS>	 
	/// 
	/// <MUSCLES>
	map<std::string, Muscle*> muscles;//  muscle as map: use muscle_keys as string
	//std::vector<std::string> muscle_keys = { "ham_r","glu_r","hfl_r", "rf_r", "vas_r","sol_r", "ta_r", "gas_r", "ham_l","glu_l","hfl_l", "rf_l", "vas_l", "sol_l", "ta_l", "gas_l"};//  MTUs OF THE MODEL. SOL and HAM MUST be befor TA and HFL
	std::vector<std::string> muscle_keys = { "ham_r","glu_r","hfl_r", "rf_r", "vas_r","sol_r", "ta_r", "gas_r", "ham_l","glu_l","hfl_l", "rf_l", "vas_l", "sol_l", "ta_l", "gas_l" };//  MTUs OF THE MODEL. SOL and HAM MUST be befor TA and HFL
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

	std::vector<std::string> muscle_p_keys = { "Fmax", "v_max", "lopt","lslk", "rho", "r", "r1", "phiM", "phiR", "phi1M", "phi1R" };// MTU's PROPERTIES
	map<std::string, float> sol, ta, gas, vas, hfl, glu, ham, rf;
	/// </MUSCLES>
	
	/// <JOINTS>
	dCustomSlider* slider;
	dCustomSlider* slider2;

	std::vector<std::string> DHjoint_keys = { "Hip_r", "Ankle_r","Hip_l", "Ankle_l"};
	map<std::string, dCustomDoubleHinge*> JDoubleHinge;//  muscle as map: use muscle_keys as string
	std::vector<std::string> Hjoint_keys = {"Knee_r", "Knee_l", "Spine1","Spine2", "Toe_hinge_r", "Toe_hinge_l"};
	map<std::string, dCustomHinge*> JHinge;//  muscle as map: use muscle_keys as string
	/// </JOINTS>

	/// <DATA IMPORT FROM XML>
	void dump_to_stdout(const char* pFilename);
	void dump_to_stdout(TiXmlNode* pParent, unsigned int indent = 0);
	const char* getIndent(unsigned int numIndents);
	const char* getIndentAlt(unsigned int numIndents);
	int dump_attribs_to_stdout(TiXmlElement* pElement, std::vector<float>& vector, unsigned int indent);
	int dump_attribs_to_stdout2(TiXmlElement* pElement, std::map<std::string, float>& l, unsigned int indent);
	/// </DATA IMPORT FROM XML>

	/// <GEOMETRIC AND INERTIA PARAMS>
	std::vector<float> v_scale, v_lengths, v_total_weight, v_masses, v_ixx, v_iyy, v_izz, v_com, v_angles, v_x1, v_y1, v_z1, v_x2, v_y2, v_z2, v_maxForce;

	map<std::string, float> lengths;
	map<std::string, float> mass_distrib, delta_cm;
	map<std::string, float> Ixx, Iyy, Izz, body_rot_ang;
	std::vector<std::string> lengths_keys = { "Head","UpArm","ForeArm","Hand","Trunk","Thigh","Shank","Foot","Toes","UPT","MPT","LPT","Hip","Shoulder","FootBreadth","AnkleHeight","Neck", "RadBones","RadSpheres" };
	std::vector<std::string> mass_keys = { "Head","UpArm","ForeArm","Hand","Trunk","Thigh","Shank","Foot","Toes","UPT","MPT","LPT" };

	float l_Hip,l_Up_Leg,l_Low_Leg,r_bones,l_foot,w_foot,l_toe,h_foot;
	float scale_mass, scale_length;
	float l_Clav,l_Up_Arm,l_Low_Arm,l_Hand,l_Head, r_Pad, h_sphere,l_UPT, l_MPT, l_LPT, l_trunk, l_neck, l_delta, ankle_j, knee_j;

	dVector  ContactFoot_L,NormalFoot_L,ContactGround_L;
	dVector  ContactFoot,NormalFoot,ContactGround;
	/// </GEOMETRIC AND INERTIA PARAMS>
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
	int GetContactPoints(NewtonBody* const body, dVector* const points) const;
private:
	WindowMain* m_winManager;
};


#endif //OXVEHICLE_RAYCAST__H_
