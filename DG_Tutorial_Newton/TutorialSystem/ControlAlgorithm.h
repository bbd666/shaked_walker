#ifndef _CONTROLALGORITHM_H_
#define _CONTROLALGORITHM_H_

#pragma once
#include "biped.h"

class ControlAlgorithm
{
public:
	ControlAlgorithm();
	~ControlAlgorithm();
	float MTU_excitation(Mtuname m_name, vector<bool> gait_state,
		vector<dFloat>  Tstate, vector<dFloat>  Pstate, char lead, char lat, float l_til, float f_norm, float time);// the ouput is the excitation signal for muscle m_name
	//
	vector<bool> Stance_Swing_Detection(dVector foot, dVector com, float leg, float clearance);

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
	void SaveOtherMuscleExcitation(float u, char lat, Mtuname mname);
	float GetOtherMuscleExcitation(Mtuname mname, char lat);

private:
	/// <Control parameters>
	map<Mtuname, float> Gf;// gain force feedback
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

	float uf_sol_r;
	float uf_sol_l;
	float ul_ham_r;
	float ul_ham_l;
	/// </Control parameters>
};

#endif // _CONTROL_ALGORITHM_H_