/*
* Copyright(c) < 1999 - 2019 > <Dave Gravel, Free Tutorial - Newton Dynamics and more.>
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
/*
  Tutorial for show how to use newton dynamics api with pure gl write by Dave Gravel - 2019.
  This tutorial if the first version, Some parts is quickly coded and need a better implementation.
  I have try to add the most options possible without have very complex code.
  I can surely update this tutorial later with better features and implementation.
  I think for the first version it already touch pretty much all subject about newton and GL.
  Have Fun...
  Dave Gravel.
*/

#include "pch.h"
#include "WindowGL.h"
#include "GeomGL.h"
#include "biped.h"
#include "OXVehicleRayCast.h"
#include "LoadXML.h"

#if defined(_DEBUG)
int main(int argc, char *argv[])
// if you like to use windows mode. 
// Comment the main section here, And uncomment the winmain section.
// Remember you to replace the console mode to windows mode in the preprossesor menu and in the system menu options.
#else
int WINAPI wWinMain(
	HINSTANCE   hInstance,
	HINSTANCE   hPrevInstance,
	PWSTR       lpCmdLine,
	int         nCmdShow)
#endif
{	
	ModelParams params;
	bool flag_xml = params.load("OptimizationParameters.xml");// params are normalized here
	if (flag_xml == 1)
	{
		cout << "Error in loading model parameters from ModelParameters.xml file" << endl;
		return 0;
	}
	params.ScaleParameters(); //parameter scaled for optimization

	// CALL CMAES // TO DO

	params.RemoveScaling();//not scaled params

	WindowMain* ContextGL = new WindowMain();
	//
	//
	ContextGL->SetUseMouseViewRotation(true);
	//
	//	 
		DGVehicleRCManager* aWalkerManager = new DGVehicleRCManager(ContextGL);
	dMatrix matrix(dGetIdentityMatrix());
	dRaycastVHModel* const Model = aWalkerManager->CreateWalkerPlayer("WALKER", matrix);

	Model->controller.SetGain_InitialCondition(params.InitialCondition[0], params.InitialCondition[1],params.InitialCondition[2], params.InitialCondition[3], params.InitialCondition[4], params.InitialCondition[5], params.InitialCondition[6]);
	Model->controller.SetGain_StanceLead(params.StanceLead[0], params.StanceLead[1], params.StanceLead[2], params.StanceLead[3], params.StanceLead[4], params.StanceLead[5], params.StanceLead[6], params.StanceLead[7], params.StanceLead[8]);
	Model->controller.SetGain_ForceFeedback(params.ForceFeedback[0], params.ForceFeedback[1], params.ForceFeedback[2], params.ForceFeedback[3], params.ForceFeedback[4], params.ForceFeedback[5]);
	Model->controller.SetGain_LengthFeedback(params.LengthFeedback[0], params.LengthFeedback[1], params.LengthFeedback[2], params.LengthFeedback[3], params.LengthFeedback[4], params.LengthFeedback[5]);
	Model->controller.SetGain_Coronal_lead(params.Coronal_lead[0], params.Coronal_lead[1], params.Coronal_lead[2], params.Coronal_lead[3]);
	Model->controller.SetGain_Coronal(params.Coronal[0], params.Coronal[1], params.Coronal[2], params.Coronal[3], params.Coronal[4], params.Coronal[5]);
	Model->controller.SetGain_Arm(params.Arm[0], params.Arm[1]);

	float cost = 0, mass = 0;
	vector<float> reward;
	if (ContextGL != NULL) {
		ContextGL->MainLoop();

		reward = Model->controller.GetRewardValues();
		mass = Model->GetModelMass();
		//
		// delete context and newton manager and close tutorial.
		//if (man) { delete(man); }
		//if (m1) { delete(m1); }
		delete ContextGL;
		//
	}
#if defined(_DEBUG)
	// The tutorial report some leaks but this leaks seen to come from the window create gl.
	// I have try to break on the number but it do nothing.
	// When a real leak happen from the tutorial object class I can break on the report number.
	// I need to test more about it later but the leak is always the same and it don't seen to grow more.
	// I have test to create the window gl only without the manager or any object and the leak is present.
	// If I don't create the window the leak go away.
	// You can find the break option in the class creation WindowMain::WindowMain(int dwidth, int dheight)
#endif 
	float we = 0.004 / mass; // weight for torques
	float wm = 100;// weight for Muscles torques
	float wpd = 1; // weight for PD torques
	float T = 3000 * 5; // 3000 Hz * simulation time (5s) number of iterations CHECK!

	cost = reward[0] + we/T*(wm*reward[1]+wpd*reward[2]);
	return cost;
}
