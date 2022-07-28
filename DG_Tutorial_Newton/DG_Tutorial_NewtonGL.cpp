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
	WindowMain* ContextGL = new WindowMain();
	//
	//
	ContextGL->SetUseMouseViewRotation(true);
	//
	//	 
		DGVehicleRCManager* aWalkerManager = new DGVehicleRCManager(ContextGL);
	dMatrix matrix(dGetIdentityMatrix());
	dRaycastVHModel* const Model = aWalkerManager->CreateWalkerPlayer("WALKER", matrix);

	#if defined(NDEBUG)
		// Convert the wide character wchar_t string to a string
		wchar_t** origin = __wargv;
		vector<string> argv(__argc);
		for (int i=0; i < __argc; ++i) {
			wstring ws(origin[i]);
			string str(ws.begin(), ws.end());
			argv[i] = str;
		}

		if (__argc > 1) {
			Model->controller.SetGain_InitialCondition(stof(argv[1]), stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]), stof(argv[6]), stof(argv[7]));
			Model->controller.SetGain_StanceLead(stof(argv[8]), stof(argv[9]), stof(argv[10]), stof(argv[11]), stof(argv[12]), stof(argv[13]), stof(argv[14]), stof(argv[15]), stof(argv[16]));
			Model->controller.SetGain_ForceFeedback(stof(argv[17]), stof(argv[18]), stof(argv[19]), stof(argv[20]), stof(argv[21]), stof(argv[22]));
			Model->controller.SetGain_LengthFeedback(stof(argv[23]), stof(argv[24]), stof(argv[25]), stof(argv[26]), stof(argv[27]), stof(argv[28]));
			Model->controller.SetGain_Coronal_lead(stof(argv[29]), stof(argv[30]), stof(argv[31]), stof(argv[32]));
			Model->controller.SetGain_Coronal(stof(argv[33]), stof(argv[34]), stof(argv[35]), stof(argv[36]), stof(argv[37]), stof(argv[38]));
			Model->controller.SetGain_Arm(stof(argv[39]), stof(argv[40]));
		}
	#else
		if (argc > 1) {
			Model->controller.SetGain_InitialCondition(stof(argv[1]), stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]), stof(argv[6]), stof(argv[7]));
			Model->controller.SetGain_StanceLead(stof(argv[8]), stof(argv[9]), stof(argv[10]), stof(argv[11]), stof(argv[12]), stof(argv[13]), stof(argv[14]), stof(argv[15]), stof(argv[16]));
			Model->controller.SetGain_ForceFeedback(stof(argv[17]), stof(argv[18]), stof(argv[19]), stof(argv[20]), stof(argv[21]), stof(argv[22]));
			Model->controller.SetGain_LengthFeedback(stof(argv[23]), stof(argv[24]), stof(argv[25]), stof(argv[26]), stof(argv[27]), stof(argv[28]));
			Model->controller.SetGain_Coronal_lead(stof(argv[29]), stof(argv[30]), stof(argv[31]), stof(argv[32]));
			Model->controller.SetGain_Coronal(stof(argv[33]), stof(argv[34]), stof(argv[35]), stof(argv[36]), stof(argv[37]), stof(argv[38]));
			Model->controller.SetGain_Arm(stof(argv[39]), stof(argv[40]));
		}
	#endif

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
