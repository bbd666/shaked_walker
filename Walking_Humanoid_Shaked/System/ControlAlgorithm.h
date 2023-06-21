#ifndef _CONTROLALGORITHM_H_
#define _CONTROLALGORITHM_H_

#pragma once
#include "Muscle.h"
/**
 * @class ControlAlgorithm
 * useful to apply the control logic of MTU and define the fitness function for optimization
 */
class ControlAlgorithm
{
public:
	ControlAlgorithm();
	~ControlAlgorithm();
	/**
	* Cumputes the torque of a joint produced by PD controller, based on the state of the related rigid bodies.
	* @param dof (input) name of the controlled degree of freedom
	* @param leading (input) flag to indicate if the leg is the leading one
	* @param torque (output) torque to be applied to the joint.
	*/
	float PD_controller(string dof, bool leading);
	/**
	* Cumputes the MTU excitation signal
	* @param m_name (input) name of the MTU
	* @param gait_state (input) vector containing the step state (stance, swing, SI and SP)
	* @param excitation (output) excitation signal.
	*/
	float MTU_excitation(Mtuname m_name,vector<bool> gait_state,

	char lead, char lat, float l_til, float f_norm, dVector com_Playervel);// the ouput is the excitation signal for muscle m_name
	/**
	* Detects the state phase of the foot (stance swing stance preparation or swing initiation
	* @param foot (input) postioon vector of the foot
	* @param com (input) postioon vector of the body com
	* @param leg (input) length of the leg
	* @param contact (input) number of contact points with the ground
	* @param foot_state (output) vector with foot state, order is: STANCE, SWING, SP, SI.
	*/
	vector<bool> Stance_Swing_Detection(dVector foot, dVector com, float leg, float contact);
	/**
	* Sets walker com velocity
	* @param COMvel (input) com velocity
	*/
	void SetPlayerCOMvel(dVector COMvel);
	/**
	* Sets the sagittal distance between the feet
	* @param d (input) distance between feet coms
	*/
	void SetSagittalDistance(float d);
	/**
	* Sets the coronal distance between the feet (step width)
	* @param d (input) distance between feet coms
	*/
	void SetCoronalDistance(float d1);
	/**
	* Sets optimization params
	* @param parameters (input) pointer to optimization parameters
	*/
	void SetOptimizationParam(double* parameters);

	/**
	* Get the shoulder target angles
	* @param state0 (input) if true -> initial condition values
	* @param lat (input) laterality
	* @param angles (output) sagittal and transverse angles of the shoulder
	*/
	vector<float> GetShoulderTargetAngles(bool state0, char lat);
	/**
	* Get the force feedback gain for given MTU
	* @param NAME (input) name of the MTU
	* @param Gain (output) force feedback gain
	*/
	float GetGain_Force_Feedback(Mtuname NAME);
	/**
	* Get the first length feedback gain for given MTU
	* @param NAME (input) name of the MTU
	* @param Gain1 (output) gain 1 length feedback gain
	*/
	float GetGain1_Length_Feedback(Mtuname NAME);
	/**
	* Get the second length feedback gain for given MTU
	* @param NAME (input) name of the MTU
	* @param Gain2 (output) gain 2 length feedback gain
	*/
	float GetGain2_Length_Feedback(Mtuname NAME);
	/**
	* Get the proportional gain for given MTU
	* @param NAME (input) name of the MTU
	* @param k (output) gain k
	*/
	float GetGain_PDk(Mtuname NAME);
	/**
	* Get the derivative gain for given MTU
	* @param NAME (input) name of the MTU
	* @param d (output) gain d
	*/
	float GetGain_PDd(Mtuname NAME);
	/**
	* Get the reference angle value for given MTU
	* @param NAME (input) name of the MTU
	* @param a (output) angle value
	*/
	float GetGain_PDa(Mtuname NAME);
	/**
	* Get the proportional gain for given MTU of the leading leg
	* @param NAME (input) name of the MTU
	* @param k (output) gain k
	*/
	float GetGain_Lead1(Mtuname NAME);
	/**
	* Get the derivative gain for given MTU of the leading leg
	* @param NAME (input) name of the MTU
	* @param d (output) gain d
	*/
	float GetGain_Lead2(Mtuname NAME);
	/**
	* Get the reference angle value for given MTU of the leading leg
	* @param NAME (input) name of the MTU
	* @param a (output) angle value
	*/
	float GetGain_Lead3(Mtuname NAME);
	/**
	* Get the additional P gain of vas and hfl
	* @param NAME (input) name of the MTU
	* @param P1 (output) proportional gain
	*/
	float GetGain_P1(Mtuname NAME);
	/**
	* Get the additional reference angle gain of vas and hfl
	* @param NAME (input) name of the MTU
	* @param A (output) reference angle
	*/
	float GetGain_P2(Mtuname NAME);
	/**
	* Get the pre stimulation values during stance initiation
	* @param NAME (input) name of the MTU
	* @param value (output) pre stimulation value 
	*/
	float GetPreStimSI(Mtuname NAME);
	/**
	* Get the pre stimulation values during stance 
	* @param NAME (input) name of the MTU
	* @param value (output) pre stimulation value
	*/
	float GetPreStimStance(Mtuname NAME);
	/**
	* Get the hfl gains during swing
	* @param vector (output) gain vector
	*/
	vector<float> GetGain_HFLswing();
	/**
	* Get proportional and derivative gains of a joint
	* @param dof (input) name of the joint
	* @param lead (input) if true the joint is part of the leading leg
	* @param value (output) vector with P and D
	*/
	vector<float> GetGain_PD(string dof, bool lead);
	/**
	* Set shoulder angle gain
	* @param gain(input) value of the gain
	* @param dof (input) name of the shoulder L or R
	*/
	void SetShoulderAngleGain(float gain, string dof);

	/**
	* Get initial condition parameters
	* @param values (output) vector with the 7 initial condition parameters
	*/
	vector<float>  GetInitialCondition();
	/**
	* Set the state of a rigid body
	* @param position (input) position (angle) value
	* @param velocity (input) velocity (angular vel) value
	* @param dof (input) name of the rigid body
	*/
	void SetState(float position, float velocity, string dof);
	/**
	* Set the initial state of a rigid body
	* @param position (input) position (angle) value
	* @param velocity (input) velocity (angular vel) value
	* @param dof (input) name of the rigid body
	*/
	void SetState0(float position, float velocity, string dof);
	/**
	* Get the state of a rigid body
	* @param dof (input) name of the rigid body
	* @param state (output) position (angle) and velocity value
	*/
	vector<float> GetState(string dof);
	/**
	* Get the initial state of a rigid body
	* @param dof (input) name of the rigid body
	* @param state (output) position (angle) and velocity value
	*/
	vector<float> GetState0(string dof);
	/**
	* Update the queue of activation values due to neural delay of a MTU
	* @param muscle (input) name of the MTU
	* @param queue (input) queue to be updated
	* @param value (input) value to append
	*/
	void UpdateQueue(string muscle, map<string, list<float>> &queue, float value);
	/**
	* Get the name of the MTU
	* @param name (input) key of the MTU
	* @param lat (input) laterality
	* @param string (output) string of the MTU name
	*/
	string MuscleName(Mtuname name, char lat);
	/**
	* Compute task reward of the fitnees function of the CMAES
	* @param COMY_pos (input) height of the walker COM
	* @param COM_vel (input) velocity vector of the walker COM
	* @param Strunk (input) trunk sagittal orientation
	* @param Ftrunk (input) trunk front orientation
	*/
	void UpdateTaskReward(dFloat COMY_pos, dVector COM_vel, float Strunk, float Ftrunk);
	/**
	* Compute MTU reward of the fitnees function of the CMAES
	* @param value (input) the power of 2 of muscle torque
	*/
	void UpdateMuscleReward(float value);
	/**
	* Get  task cost, MTU cost and PD controllers cost
	*/
	vector<float> GetRewardValues();
private:
	dVector COMvel;
	// SIMBICON target angle parameters and values
	float cd, cv, cd1, cv1;
	float dz; // sagittal distance between foot and player com
	float dx; // coronal distance between foot and player com

	// arm params
	float alfa, beta, gamma;
	/// <Control parameters>
	map<Mtuname, float> Gf;// gain force feedback
	map<Mtuname, float> Gf_TA_SOL;// gain force feedback fo TA depending on SOL
	map<Mtuname, float> Glg;// gain length 1 feedback
	map<Mtuname, float> Glh;// gain length 2 feedback
	map<Mtuname, float> GPDk;// gain PD controller stiffness
	map<Mtuname, float> GPDd;// gain PD controller damper
	map<Mtuname, float> GPDa;// gain PD controller angle

	map<Mtuname, float> GP1; // gain P controller hfl in swing 
	map<Mtuname, float> GP2; // gain P controller hfl in swing 

	map<Mtuname, float> Glead1; // gain P controller hfl in swing 
	map<Mtuname, float> Glead2; // gain P controller hfl in swing  
	map<Mtuname, float> Glead3; // gain P controller hfl in swing  

	map<Mtuname, float> PreSI;  //map pre stimulation added / subtracted to during swing initiation
	map<Mtuname, float> PreSTANCE; // map pre stimulation values for vas, hfl, ham, glu during stance

	map<string,list<float>> uf_l;// force feedback
	map<string, list<float>>uf_ta_sol_l;// force feedback for TA depending on SOL
	map<string, list<float>> ul_l;//length feedback
	map<string, list<float>> up_l;//pd controller of vas e hfl
	map<string, list<float>> u_sp_l;//pd controller for HIP during SP
	map<string, list<float>> uHIP_s_l;//pd controller for HIP during stance

	map<string, list<float>>f_ta_sol_l;// force values for TA depending on SOL

	// PD controllers params
	map<string, float> G1; // gain P 
	map<string, float> G2; // gain D 
	map<string, float> G3; // gain target angle 

	map<string, float> G1lead; // gain P for leading leg
	map<string, float> G2lead; // gain D  for leading leg
	map<string, float> G3lead; // gain target angle  for leading leg
	/// </Control parameters>

	// initial conditions
	float trunk_a, head_a, AlphaR, AlphaL, Beta,Gamma, elbow_a, Vel_initial;

	// State of each controlled dof
	map<string, float> State_position;// angle
	map<string, float> State_velocity;// velocity

	map<string, float> State0_position;// initial angle
	map<string, float> State0_velocity;// initial velocity
	//! Task simulation cost
	float TaskCost;
	//! Muscle Torque simulation cost
	float TorqueMuscleCost; // 
	//! PD Torque simulation cost
	float TorquePDCost;
};

#endif // _CONTROL_ALGORITHM_H_