#include "pch.h"
#include "MuscleV2.h"
#include "GeomGL.h"
#include "OXVehicleRayCast.h"

// muscle as slider + stiffness

MuscleV2::MuscleV2(NewtonManager* wMain, GeomNewton* first_body, GeomNewton* second_body) :
	m_Manager(wMain)
	, body1(first_body)
	, body2(second_body)
{
	m_Manager->vMuscleV2List.push_back(this);
	m_Fmax = 6000.f; // [N] 
	m_Vmax = 10.f; // [lopts^-1]

	
}

MuscleV2::~MuscleV2() {
	if (!m_Manager->IsTerminated)
		for (auto itr = m_Manager->vMuscleV2List.begin();
			itr != m_Manager->vMuscleV2List.end(); itr++) {
		if ((MuscleV2*)*itr == this) {
			m_Manager->vMuscleV2List.erase(itr);
			break;
		}
	}
}


// this function returns the muscle force as linear function of V_ce and P_ce (velocity and position of CE)
float MuscleV2::GetForceMTU_V2(dFloat t)
{
	//float A = 1;
	//float deltapm = 0.1; // [m]
	//float m_Fmtu, fp, fv, vs, ps;

	//if (m_time < 0) // CE position initialization 
	//	p_initial = slider->GetJointPosit();

	//// Get velocity and position of the slider
	//vs = abs(slider->GetJointSpeed());
	//ps = abs(slider->GetJointPosit());
	//// Force computation
	//fv = 1 - (vs / m_Vmax);// Check value
	//fp = 1 - ((ps - p_initial) / deltapm); // Check value
	//m_Fmtu = A * m_Fmax * fp * fv;

	//cout << m_Fmtu/m_Fmax << '\t' << vs/m_Vmax << '\t' << ps/deltapm << endl;
	//m_time = m_time + t; // time update

	//return m_Fmtu;
	return 0.0;
}