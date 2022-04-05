#ifndef _CONTROLALGORITHM_H_
#define _CONTROLALGORITHM_H_

#pragma once
#include "biped.h"

class ControlAlgorithm
{
public:
	ControlAlgorithm();
	~ControlAlgorithm();
	float PD_controller(string dof, bool leading);
	float MTU_excitation(Mtuname m_name,vector<bool> gait_state,
		char lead, char lat, float l_til, float f_norm, dVector com_Playervel);// the ouput is the excitation signal for muscle m_name
	
	vector<bool> Stance_Swing_Detection(dVector foot, dVector com, float leg, float contact);

	void SetPlayerCOMvel(dVector COMvel);
	void SetSagittalDistance(float d);
	void SetCoronalDistance(float d1);
	void SetGain_InitialCondition(float Tangle, float alfaR, float alfaL, float beta, float gamma, float Tvel, float elbow);
	void SetGain_Force_Feedback(Mtuname NAME, float val);
	void SetGain_StanceLead(float Pham, float Aham, float Dham, float Pglu, float Aglu, float Dglu, float Phfl, float Ahfl, float Dhfl);

	void SetGain_ForceFeedback(float Gf_glu, float Gf_ham, float Gf_vas, float Gf_sol, float Gf_gas, float Gf_tasol);

	void SetGain_LengthFeedback(float Glg_hfl, float Glg_ham, float Glg_ta, float Glh_hfl, float Glh_ham, float Glh_ta);

	vector<float> GetShoulderTargetAngles(bool state0, char lat);
	float GetGain_Force_Feedback(Mtuname NAME);
	float GetGain1_Length_Feedback(Mtuname NAME);
	float GetGain2_Length_Feedback(Mtuname NAME);
	float GetGain_PDk(Mtuname NAME);
	float GetGain_PDd(Mtuname NAME);
	float GetGain_PDa(Mtuname NAME);
	float GetGain_Lead1(Mtuname NAME);
	float GetGain_Lead2(Mtuname NAME);
	float GetGain_Lead3(Mtuname NAME);
	float GetGain_P1(Mtuname NAME);
	float GetGain_P2(Mtuname NAME);
	vector<float> GetGain_HFLswing();
	vector<float> GetGain_PD(string dof, bool lead);

	void SetShoulderAngleGain(float gain, string dof);

	vector<float>  GetInitialCondition();

	vector<float> GetArmParams();

	void SetState(float position, float velocity, string dof);
	void SetState0(float position, float velocity, string dof);
	vector<float> GetState(string dof);
	vector<float> GetState0(string dof);

	void UpdateQueue(string muscle, map<string, list<float>> &queue, float value);
	string MuscleName(Mtuname name, char lat);

	float SimulationReturnValue(dFloat COMY_pos, dVector COM_vel, vector<float> Strunk, vector<float> Ftrunk);
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
	float trunk_a; // initial trunk forward inclination in rad
	float head_a; // initial force applied to lpt
	float AlphaR;
	float AlphaL;
	float Beta;
	float Gamma;
	float elbow_a;

	// State of each controlled dof
	map<string, float> State_position;// angle
	map<string, float> State_velocity;// velocity

	map<string, float> State0_position;// initial angle
	map<string, float> State0_velocity;// initial velocity

	float cost; // creturn simulation cost
};

#endif // _CONTROL_ALGORITHM_H_