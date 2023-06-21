#ifndef BIPED_H
#define BIPED_H

#pragma once
#include "pch.h"

#include "Shader.h"
#include "Camera.h"
#include "VertexGL.h"
#include "WindowGL.h"

/**
 * Keys for type of joints
 */
enum JointType {Hinge, BallAndSocket, DoubleHinge, NOtype};
/**
 * Keys for MTU names
 */
enum Mtuname {GLU, HAM, HFL, RF, VAS, GAS, SOL, TA};

/**
 * @struct Muscle
 * Muscle-Tendon Unit model definition
 */
struct Muscle
{
	Muscle(LineDebugManager* LManager, NewtonManager* wMain, GeomNewton* insert1, GeomNewton* insert2, GeomNewton* insert3, dVector ins1, dVector ins2, JointName jname, JointType jtype, JointName jname1, JointType jtype1, Mtuname mname);
	virtual ~Muscle();
	/**
	* Gets MTU name enum
	* @param name (output) enum of MTU name
	*/
	Mtuname GetMuscleName();
	glm::vec3 mColor;
	/**
	* Update graphic  line of the MTU
	*/
	void UpdateLineCoord(Shader* cshd, dFloat steptime);
	/**
	* Define insertion point of MTU line
	* @param px (input) x coord
	* @param py (input) y coord
	* @param pz (input) z coord
	*/
	void SetInsert1(float px, float py, float pz);
	/**
	* Define insertion point 2 of MTU line
	* @param px (input) x coord
	* @param py (input) y coord
	* @param pz (input) z coord
	*/
	void SetInsert2(float px, float py, float pz);
	/**
	* Convert line point 1 coords in global coords
	*/
	dVector GetInsert1_GlobalRef();
	/**
	* Convert line point 2 coords in global coords
	*/
	dVector GetInsert2_GlobalRef();
	/**
	* Get origin and insertion points coordinates of MTU
	*/
	void GetOriginAndInsertion(dVector &vOrigin, dVector &vInsert);
	/**
	* Convert excitation in activation of MTU due to neural delay
	* @param iExcitation (input) excitation value
	*/
	void SetActivation(const float iExcitation);
	/**
	* Get activation
	* @param a (input) activation value
	*/
	float GetActivation();
	/**
	* Get MTU parameters
	* @param angle (input) actual angle of joint 1
	* @param angle1 (input) actual angle of joint 2
	* @param lce (input) length of contractile element
	* @param lopt (input) optimal length of contractile element
	* @param lmtu (input) length of MTU
	* @param Fmuscle (input) force of the muscle
	* @param Fmax (input) maximum force of the MTU
	* @param V (input) contraction velocity
	*/
	void GetMuscleParams(float& angle, float& angle1, float& lce, float& lopt, float& lmtu, float& Fmuscle, float& Fmax, float& V);
	/**
	* Set stepsize
	* @param iStepSize (input) step size
	*/
	void SetNeuralDelay(const float iStepSize);
	/**
	* Get the angle of the joint 1 actuated by the MTU
	*/
	float GetAngle();
	/**
	* Set the angle of the joint 1 actuated by the MTU
	* @param a (input) angle value
	*/
	void SetAngle(float ang);
	/**
	* Get the angle of the joint 2 actuated by the MTU
	*/
	float GetAngle1();
	/**
	* Set the angle of the joint 2 actuated by the MTU
	* @param a (input) angle value
	*/
	void SetAngle1(float ang);
	/**
	* Set the angle limits of the two joints actuated by the MTU
	* @param a_lim (input) limit 1 value
	* @param a_lim1 (input) limit 2 value
	*/
	void SetLimits(float a_lim, float a1_lim);
	/**
	* compute derivative of residu
	* @param l (input) length
	* @param t (input) timestep
	*/
	float dresidu(const float l,const float t);
	/**
	* compute Err(delta) = Fce + Fpe + Fse as a function of ce increment
	* @param l (input) length
	* @param t (input) timestep
	*/
	float residu(const float l,const float t);
	/**
	* Force of the series element
	* @param l (input) length
	*/
	float fSE(const float l);
	/**
	* Force of the contractile element
	* @param l (input) length
	* @param t (input) timestep
	*/
	float fCE(const float l, const float t);
	/**
	* Force of the parallel element
	* @param l (input) length
	*/
	float fPE(const float l);
	/**
	* Force of the dumper element
	* @param l (input) length
	* @param t (input) timestep
	*/
	float fDE(const float l, const float t);
	/**
	* Compute muscle torque with implicit integration method(Newton - Rhaphson)
	* @param time (input) timestep
	* @param torques (output) T.m_x = torque joint 1, T.m_y = torque joint 2
	*/
	dVector Compute_muscle_Torque(dFloat time);
	/**
	* Compute MTU length
	* @param jointangle (input) angle of joint 1 on the MTU
	* @param jointangle1 (input)  angle of joint 2 on the MTU
	*/
	float Compute_muscle_length(float jointangle, float jointangle1);
	/**
	* Set the initial angle of joint 1
	* @param angle (input) angle of joint 1 on the MTU
	*/
	void SetThetazero(float angle);
	/**
	* Set the initial angle of joint 2
	* @param angle (input) angle of joint 2 on the MTU
	*/
	void SetTheta1zero(float angle);
	/**
	* Set the MTU parameters
	* @param Fmax (input) angle of joint 1 on the MTU
	* @param v_max (input) contraction velocity
	* @param opt (input) optimal length
	* @param slk (input) slack length
	* @param rhoin (input) pennation angle
	* @param r (input) arm on joint 1
	* @param r1 (input) arm on joint 2
	* @param phiM (input) angle of joint 1 with max moment arm
	* @param phiR (input) angle of joint 1 with delta equal to 0
	* @param phiM1 (input) angle of joint 2 with max moment arm
	* @param phiR1 (input) angle of joint 2 with delta equal to 0
	*/
	void SetMuscleParams(const float Fmax, const float v_max, const float opt, const float slk, const float rhoin, const float r, const float r1, const float phiM, const float phiR, const float phi1M, const float phi1R);
	/**
	* Activate the graphics rendering of the MTU
	*/
	void GenerateMesh();
	/**
	* Get the number of iterations of NewtonRaphson algo
	*/
	float GetNmax();
	/**
	* Get the length delta of MTU
	*/
	float GetDelta_l();
	/**
	* Set the length delta of MTU
	*/
	void SetDelta_l(const float l);
	/**
	* Get the CE length  MTU
	*/
	float GetLCE();
	/**
	* Set the CE length  MTU
	*/
	void SetLCE(const float l);
	/**
	* Get the laterality of MTU
	*/
	char GetLaterality();
	/**
	* Set the laterality of MTU
	*/
	void SetLaterality(char letter);

	GeomNewton* body1;
	GeomNewton* body2;
	GeomNewton* body3;	
	JointName Jname;
	JointType Jtype;
	JointName Jname1;
	JointType Jtype1;
	Mtuname m_name;

private:
	char laterality;
	
	NewtonManager* m_Manager;
	LineDebugManager* LDebug_Manager;
	GeomBase* aUserData;
	MainVertexPTN* aVtx;
	glm::vec3 lineColor;
	GLuint aVao;
	GLuint aVbo;
	GLuint aIbo;
	float m_Length0;
	float m_Stiffness;
	dVector m_Insert1;
	dVector m_Insert2;
	int LineIndex;
	float l_opt; // [cm] optimum muscle length
	float v; // [cm/s]  muscle velocity
	float vm; // [cm/s] max muscle velocity
	float lCE; // [cm] contractile element length
	float activation; // activation level of the muscle [0-1]
	double stepSize;
	int m_nmax;
	float m_Delta_l; // [cm] muscle increment
	float arm; // [cm] maximum moment arm of the muscle
	float arm1; // [cm] maximum moment arm of the muscle. arm 1 refers to the lowest joint of the muscle (proximal ref system)
	float phi_M; // [rad] joint angle with maximum moment arm
	float phi_R; // [rad] joint angle in which delta = 0 (l_mtu = l_opt + l_slk)
	float phi1_M; // [rad] joint 1 angle with maximum moment arm. Joint 1 is the lowest joint of the muscle (proximal ref system)
	float phi1_R; // [rad] joint 1 angle in which delta = 0 (l_mtu = l_opt + l_slk)
	float F_max; // max isometric force [N]
	float m_Fmtu;
	float rho; // accounts for pennation angle
	float l_slk; // slack length
	float l_mtu;
	float theta_actual; // actual angle of the joint
	float theta1_actual; // actual angle of the joint 1
	float theta_0; // initial angle of the joint
	float theta1_0; // initial angle of the joint 1
	float damp;
	float Jang; // angle of the joint
	float Jang1; // angle of the joint 1
	float kj; // joint stiffness
	float vj_max; // max joint relaxation speed
	float ang_lim; //min joint angle [rad] local angle reference system
	float ang1_lim;// max joint angle [rad] local angle reference system joint 1
};
#endif //BIPED_H
