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
#ifndef _HUMANOID__H_
#define _HUMANOID__H_

#include "pch.h"
#include "dCustomJoint.h"
#include "NewtonManager.h"
#include "WindowGL.h"
#include "tinyxml.h"
#include "Muscle.h"
#include "NewtonUtil.h"
#include "ControlAlgorithm.h"
#include "Params.h"
/**
 * @class dWalkerModel
 * Walker model definition: rigid bodies, joints, muscles, floor.
 */
class dWalkerModel: public dModelRootNode
{
public:
	dWalkerModel(WindowMain* const winctx, const char* const modelName, const dMatrix& location, int linkMaterilID, double* params, WBV wbv);
	~dWalkerModel();
	/**
	* Draws a reference frame of three lines. The origin is the location of the interest point
	* @param posit (input) point vector to be drawn
	* @param scale (input) length of lines
	*/
	void DrawFrame(const dVector& posit, dFloat scale);
	//virtual void DrawFrame(const dMatrix& matrix, dFloat scale);
	/**
	* Draws a line.
	* @param posit (output) ID of the line
	*/
	int CreateLine();
	/**
	* Creates the foot as a collision of spheres. The contact happens at the heel, forefoot and toes.
	* @param footBody (input) body of the foot
	*/
	void CreateDescreteContactFootCollision(NewtonBody* const footBody) const;
	/**
	* Gets the sagittal orientation angle and rotation speed of a body
	* @param body (input) point vector to be drawn
	* @param state (output) state vector with angle and rotation speed in sagittal plane of body
	*/
	vector<dFloat> GetSagittalState(string body);
	/**
	* Gets the coronal orientation angle and rotation speed of the trunk
	* @param state (output) state vector with angle and rotation speed in coronal plane of the trunk
	*/
	vector<dFloat> GetTrunkCoronalState();
	/**
	* Gets the coronal orientation angle and rotation speed of the foot
	* @param body (input) name of the body
	* @param state (output) state vector with angle and rotation speed in coronal plane of the foot
	*/
	vector<dFloat> GetFootCoronalState(string body);
	/**
	* Gets the transverse orientation angle and rotation speed of a the trunk
	* @param state (output) state vector with angle and rotation speed in transverse plane of the trunk
	*/
	vector<dFloat> GetTrunkTransverseState();
	/**
	* Gets the relative orientation angle and rotation speed of body1 and body2 in the sagittal plane
	* @param body1 (input) name of the body1
	* @param body2 (input) name of the body2
	* @param state (output) state vector with relative angle and rotation speed in sagittal plane
	*/
	vector<dFloat> GetRelativeSagittalState(string body1, string body2);
	/**
	* Gets the relative orientation angle and rotation speed of body1 and body2 in the transverse plane
	* @param body1 (input) name of the body1
	* @param body2 (input) name of the body2
	* @param state (output) state vector with relative angle and rotation speed in transverse plane
	*/
	vector<dFloat>GetRelativeTransverseState(string body1, string body2);
	/**
	* Compute the angle between vectors
	* @param dir (input) vector
	* @param ref (input) reference vector
	* @param crossref (input) vector to define he sign
	* @param angle (output) angle between dir and ref signed.
	*/
	float ComputeAngleBWVectors(dVector const& dir, dVector const& ref, dVector const& crossref);
	/**
	* Compute walker centre of mass
	* @param com (output) vector with com position
	*/
	dVector ComputeWalkerCOM();
	/**
	* Compute the walker com position
	* @param comvel (output) vector with com velocities
	*/
	dVector ComputeWalkerCOMvelocity();
	/**
	* Returns the pointer to the floor object
	*/
	GeomNewton* Get_Floor();
	/**
	* Returns a map with name an pointer of all bodies
	*/
	map<std::string, GeomNewton*> Get_RigidElemetList();
	/**
	* Returns a map with name an pointer of all hing joints
	*/
	map<std::string, dCustomHinge*> Get_HingeJointList();
	/**
	* Returns a map with name an pointer of all double hinge joints
	*/
	map<std::string, dCustomDoubleHinge*> Get_DoubleHingeJointList();
	/**
	* Returns the leg of the leg
	*/
	float GetLegLength();
	ControlAlgorithm controller;
	/**
	* Applies the torque of a joint to the bodies linked to the joint around a given axis
	* @param Torque (input) torque value
	* @param pin (input) axis of the torque
	* @param b1 (input) body 1
	* @param b2 (input) body 2
	*/
	void AddActionReactionTorque(float Torque, dVector pin, GeomNewton* b1, GeomNewton* b2);
	/**
	* Returns a the hip width
	*/
	float GetHipLength();
	/**
	* Returns walker mass
	*/
	float GetModelMass();
	/**
	* Apply a given inclination angle to the hinge actuator of the floor
	*/
	void ApplyTilting(float angle);
	int write_txt;

	// vectors to output data
	vector<float> time_v;

	vector<float> HFLr_T1;
	vector<float> HFLl_T1;

	vector<float> HAMr_T1;
	vector<float> HAMr_T2;
	vector<float> HAMl_T1;
	vector<float> HAMl_T2;

	vector<float> GLUr_T1;
	vector<float> GLUl_T1;

	vector<float> VASr_T1;
	vector<float> VASl_T1;

	vector<float> GASr_T1;
	vector<float> GASr_T2;
	vector<float> GASl_T1;
	vector<float> GASl_T2;

	vector<float> SOLr_T1;
	vector<float> SOLl_T1;

	vector<float> TAr_T1;
	vector<float> TAl_T1;

	vector<float> RFr_T1;
	vector<float> RFr_T2;
	vector<float> RFl_T1;
	vector<float> RFl_T2;

	vector<float> HipRAng;
	vector<float> HipLAng;
	vector<float> KneeRAng;
	vector<float> KneeLAng;
	vector<float> AnkleRAng;
	vector<float> AnkleLAng;

	vector<float> COMPos_X;
	vector<float> COMPos_Y;
	vector<float> COMPos_Z;
	vector<float> COMVel_X;
	vector<float> COMVel_Y;
	vector<float> COMVel_Z;

	vector<float> FootRPos_X;
	vector<float> FootRPos_Y;
	vector<float> FootRPos_Z;
	vector<float> FootLPos_X;
	vector<float> FootLPos_Y;
	vector<float> FootLPos_Z;

	vector<float> StanceR;
	vector<float> StanceL;
	vector<float> SPR;
	vector<float> SPL;
	vector<float> SwingR;
	vector<float> SwingL;
	vector<float> SIR;
	vector<float> SIL;
private:
	WindowMain* m_winManager;
	/**
	* Creates the rigid body assembly of the walker
	*/
	void AssemblyCreation();
	/**
	* Creates the rigid body assembly of the trunk
	*/
	void TrunkCreation();
	/**
	* Creates the rigid body assembly  of the leg
	* @param trunk (input) name of the trunk body
	* @param thigh (input) name of the thigh body (left or right)
	* @param shank (input) name of the shank body (left or right)
	* @param foot (input) name of the foot body (left or right)
	* @param toes (input) name of the toes body (left or right)
	* @param hip (input) name of the hip joint (left or right)
	* @param knee (input) name of the knee joint (left or right)
	* @param ankle (input) name of the ankle joint (left or right)
	* @param toe_hinge (input) name of the toes joint (left or right)
	*/
	void LegCreation(string trunk, string thigh, string shank, string foot, string toes, string hip, string knee, string ankle, string toe_hinge);
	/**
	* Creates the rigid body assembly  of the arm
	* @param trunk (input) name of the trunk body
	* @param uarm (input) name of the upper arm body (left or right)
	* @param larm (input) name of the lower arm body (left or right)
	* @param hand (input) name of the hand body (left or right)
	* @param shoulder (input) name of the shoulder joint (left or right)
	* @param elbow (input) name of the elbow joint (left or right)
	* @param wrist (input) name of the wrist joint (left or right)
	*/
	void ArmCreation(string trunk, string uarm, string larm, string hand, string shoulder, string elbow, string wrist);
	/**
	* Creates the rigid body
	* @param body (input) name of the body
	*/
	void RigidBodyCreation(string body);
	/**
	* Creates a hinge joint
	* @param jname (input) name of the joint
	* @param body1 (input) name of the first body
	* @param body2 (input) name of the second body
	* @param pos (input) joint position
	* @param minAng (input) minumum angle
	* @param maxAng (input) max angle of the joint
	*/
	void HingeJoint(string jname, string body1, string body2, dVector pos, float minAng, float maxAng);
	/**
	*Creates a double hinge joint
		* @param jname(input) name of the joint
		* @param body1(input) name of the first body
		* @param body2(input) name of the second body
		* @param pos(input) joint position
		* @param minAng1(input) minumum angle 1
		* @param maxAng1(input) max angle 1 of the joint
		* @param minAng2(input) minumum angle 2
		* @param maxAng2(input) max angle 2 of the joint
	*/
	void DoubleHingeJoint(string jname, string body1, string body2, dVector pos, float minAng1, float maxAng1, float minAng2, float maxAng2);
	
	float LPT_A ,ThighR_A,ThighL_A,Shank_A ,Foot_A, Head_A, Elbow_A, Model_Mass;

	dCustomHingeActuator* FloorAct; // floor actuator

	/// <RIGID ELEMENTS>
	GeomNewton* box;
	GeomNewton* Geomfloor;
	
	std::vector<std::string> body_keys = { "LPT",
	"Thigh_r","Shank_r", "Foot_r",
	"Thigh_l","Shank_l", "Foot_l",
	"MPT", "UPT",
	"Toe_r", "Toe_l",
	"UpArm_r", "ForeArm_r", "Hand_r",
	"UpArm_l", "ForeArm_l", "Hand_l",
	"Head" };//  RIGID ELEMENTS OF THE MODEL

	//  RIGID ELEMENTS OF THE MODEL
	map<std::string, GeomNewton*> rigid_element;//  geom newton as map: use mass_keys as string
	map<std::string, dModelNode*> nodes;// nodes as map: use mass_keys as string
	map<std::string, dVector*> body_pos;// vector of body position
	map<std::string, dVector*> body_dim;// vector of body dimension
	map<std::string, dVector*> body_com;// vector of body relative position
	map<std::string, std::string> child_father = { 
		{"Thigh_r", "LPT"}, {"Shank_r", "Thigh_r"}, {"Foot_r", "Shank_r"},
		{"Thigh_l", "LPT"}, {"Shank_l", "Thigh_l"}, {"Foot_l", "Shank_l"},
		{"MPT", "LPT"}, {"UPT", "MPT"},
		{"Toe_r", "Foot_r"}, {"Toe_l", "Foot_l"},
		{"UpArm_r", "UPT"}, {"ForeArm_r", "UpArm_r"}, {"Hand_r", "ForeArm_r"},
		{"UpArm_l", "UPT"}, {"ForeArm_l", "UpArm_l"}, {"Hand_l", "ForeArm_l"},
		{"Head", "UPT"} };// list with body child and father relationship;// bodies tree
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

	std::vector<std::string> DHjoint_keys = { "Hip_r", "Ankle_r","Hip_l", "Ankle_l","Shoulder_r","Shoulder_l"};
	map<std::string, dCustomDoubleHinge*> JDoubleHinge;//  muscle as map: use muscle_keys as string
	std::vector<std::string> Hjoint_keys = {"Knee_r", "Knee_l", "Spine1","Spine2", "Toe_hinge_r", "Toe_hinge_l", "Elbow_r", "Wrist_r", "Elbow_l", "Wrist_l","Neck"};
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
	std::vector<std::string> lengths_keys = { "Head","UpArm","ForeArm","Hand","Trunk","Thigh","Shank","Foot","Toe","UPT","MPT","LPT","Hip","Shoulder","FootBreadth","AnkleHeight","Neck", "RadBones","RadSpheres" };
	std::vector<std::string> mass_keys =	{ "Head","UpArm","ForeArm","Hand","Trunk","Thigh","Shank","Foot","Toe","UPT","MPT","LPT" };

	float l_Hip,l_Up_Leg,l_Low_Leg,r_bones,l_foot,w_foot,l_toe,h_foot;
	float scale_mass, scale_length;
	float l_Clav,l_Up_Arm,l_Low_Arm,l_Hand,l_Head, r_Pad, h_sphere,l_UPT, l_MPT, l_LPT, l_trunk, l_neck, l_delta, ankle_j, knee_j;
	float Shoulder_sagittal_R, Shoulder_sagittal_L, Shoulder_transverse;
	dVector  ContactFoot_L,NormalFoot_L,ContactGround_L;
	dVector  ContactFoot,NormalFoot,ContactGround;
	/// </GEOMETRIC AND INERTIA PARAMS>

	//! Result of fitness function at the end of simulation
	float SimulationCost;
};
/**
 * @class WalkerManager
 * Walker manager definition for Newton update.
 */
class WalkerManager: public dModelManager
{
public:
	WalkerManager(WindowMain* winctx);
	virtual ~WalkerManager();
	//
	/**
	* Updates Newton World: all torques from MTU and PD controllers. It is used by newton.dll and activated in WindowMain::MainRender
	* @param model (input) walker model
	* @param timestep (input) time step of the simulation
	* @param threadID (input) thread id
	*/
	virtual void OnPreUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const;
	virtual void OnPostUpdate(dModelRootNode* const model, dFloat timestep, int threadID) const;
	virtual void OnUpdateTransform(const dModelNode* const bone, const dMatrix& localMatrix) const;
	virtual void OnDebug(dModelRootNode* const model, dCustomJoint::dDebugDisplay* const debugContext);
	//
	/**
	* Creates the walker model
	* @param modelName (input) walker name
	* @param location (input) initial position of the model
	* @param params (input) pointer to optimization parameters of the model
	* @param wbv (input) whole-body vibration settings
	*/
	dWalkerModel* CreateWalkerPlayer(const char* const modelName, const dMatrix& location, double* params, WBV wbv);
	/**
	* Computes the contact points between the foot and the floor
	* @param body (input) newton body of the foot
	* @param points (input) vector of contact points
	* @param N (output) number of contact points
	*/
	int GetContactPoints(NewtonBody* const body, dVector* const points) const;
private:
	WindowMain* m_winManager;
};


#endif //HUMANOID__H_
