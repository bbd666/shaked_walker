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
int main()
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
	// create floor object zero mass
	GeomNewton* Geomfloor = new GeomNewton(ContextGL->aManager);
	Geomfloor->SetBodyType(adtDynamic);
	Geomfloor->SetTexTileU(3.0f);
	Geomfloor->SetTexTileV(3.0f);
	Geomfloor->SetTexture0("Textures//MRAMOR6X6.jpg", "Tex0");
	Geomfloor->SetDiffuseColor(0.45f, 0.45f, 0.45f);
	Geomfloor->InitNewton(atBox, 25.0f, 0.5f, 25.0f);
	
	   	 

	DGVehicleRCManager* aWalkerManager = new DGVehicleRCManager(ContextGL);
	dMatrix matrix(dGetIdentityMatrix());
	aWalkerManager->CreateWalkerPlayer("WALKER", matrix);

	if (ContextGL != NULL) {
		ContextGL->MainLoop();

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
	_CrtDumpMemoryLeaks();
#endif 
	return 0;
}
