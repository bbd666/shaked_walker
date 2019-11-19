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
	//
	//
	// create a box test object.
	GeomNewton* GeomBox = new GeomNewton(ContextGL->aManager);
	GeomBox->SetBodyType(adtDynamic);
	GeomBox->SetTexture0("Textures//woodbox.png", "Tex0");
	GeomBox->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	GeomBox->SetTurnAngle(45.0f);
	GeomBox->SetPosition(0.0f, 5.0f, 0.0f);
	GeomBox->InitNewton(atBox, 1.0f, 1.0f, 1.0f, 10.0f);
	//
	//
	// create a box2 test object.
	GeomNewton* GeomBox2 = new GeomNewton(ContextGL->aManager);
	GeomBox2->SetBodyType(adtDynamic);
	GeomBox2->SetTexture0("Textures//woodbox.png", "Tex0");
	GeomBox2->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	GeomBox2->SetTurnAngle(45.0f);
	GeomBox2->SetPosition(3.0f, 6.0f, 0.0f);
	GeomBox2->InitNewton(atBox, 1.0f, 1.0f, 1.0f, 10.0f);
	//
	//
	// create a box3 test object.
	GeomNewton* GeomBox3 = new GeomNewton(ContextGL->aManager);
	GeomBox3->SetBodyType(adtDynamic);
	GeomBox3->SetTexture0("Textures//woodbox.png", "Tex0");
	GeomBox3->SetDiffuseColor(1.0f, 1.0f, 1.0f);
	GeomBox3->SetTurnAngle(45.0f);
	GeomBox3->SetPosition(3.0f, 7.0f, 0.0f);
	GeomBox3->InitNewton(atBox, 1.0f, 1.0f, 1.0f, 10.0f);
	//
	//
	// create hinge joint exemple.
	dMatrix jPinMatrix(dGetIdentityMatrix());
	jPinMatrix = jPinMatrix * dYawMatrix(45.0f * dDegreeToRad);
	jPinMatrix.m_posit = dVector(3.0f, 6.5f, 0.0f);
    dCustomHinge* hingejoint = new dCustomHinge(jPinMatrix, GeomBox2->GetBody(), GeomBox3->GetBody());
	// Add joint in a list for auto delete it when you close the tutorial.
	// This method is for the tutorial, It can cause problem if you delete the connected body in runtime.
	// The best way to avoid the problem is to create a custom class with the joint inside, 
	// For auto remove the joint when it is auto delete by the system.
	ContextGL->aManager->vJointList.push_back(hingejoint);
	//
	//
	// create a sphere test object.
	GeomNewton* GeomSphere = new GeomNewton(ContextGL->aManager);
	GeomSphere->SetBodyType(adtDynamic);
	GeomSphere->SetTexture0("Textures//wood6.png", "Tex0");
	GeomSphere->SetDiffuseColor(0.7f, 0.0f, 0.0f);
	GeomSphere->SetPosition(0.0f, 7.0f, 0.0f);
	GeomSphere->InitNewton(atSphere, 0.5f, 0.0f, 0.0f, 10.0f);
	//
	//
	// create a sphere2 test object.
	GeomNewton* GeomSphere2 = new GeomNewton(ContextGL->aManager);
	GeomSphere2->SetBodyType(adtKinematic);
	GeomSphere2->SetTexture0("Textures//wood6.png", "Tex0");
	GeomSphere2->SetDiffuseColor(0.2f, 0.5f, 0.7f);
	GeomSphere2->SetPosition(0.0f, 1.5f, 0.0f);
	GeomSphere2->InitNewton(atSphere, 0.75f, 0.0f, 0.0f);
	//
	//
	// create a cone test object.
	GeomNewton* GeomCone = new GeomNewton(ContextGL->aManager);
	GeomCone->SetBodyType(adtDynamic);
	GeomCone->SetTexture0("Textures//wood6.png", "Tex0");
	GeomCone->SetDiffuseColor(0.0f, 0.0f, 0.7f);
	GeomCone->SetPosition(2.0f, 7.0f, 2.0f);
	GeomCone->InitNewton(atCone, 0.5f, 1.25f, 0.0f, 10.0f);
	//
	//
	// create a cylinder test object.
	GeomNewton* GeomCylinder = new GeomNewton(ContextGL->aManager);
	GeomCylinder->SetBodyType(adtDynamic);
	GeomCylinder->SetTexture0("Textures//wood6.png", "Tex0");
	GeomCylinder->SetDiffuseColor(0.0f, 0.7f, 0.7f);
	GeomCylinder->SetPosition(-2.0f, 7.0f, -2.0f);
	GeomCylinder->InitNewton(atCylinder, 0.5f, 0.5f, 1.25f, 10.0f);
	//
	//
	// create a cylinder2 test object.
	GeomNewton* GeomCylinder2 = new GeomNewton(ContextGL->aManager);
	GeomCylinder2->SetBodyType(adtDynamic);
	GeomCylinder2->SetTexture0("Textures//wood6.png", "Tex0");
	GeomCylinder2->SetDiffuseColor(0.4f, 0.7f, 0.3f);
	GeomCylinder2->SetPosition(2.0f, 7.0f, 2.0f);
	GeomCylinder2->InitNewton(atCylinder, 0.5f, 0.75f, 1.25f, 10.0f);
	//
	//
	// create a capsule test object.
	GeomNewton* GeomCapsule = new GeomNewton(ContextGL->aManager);
	GeomCapsule->SetBodyType(adtDynamic);
	GeomCapsule->SetTexture0("Textures//wood6.png", "Tex0");
	GeomCapsule->SetDiffuseColor(0.7f, 0.7f, 0.0f);
	GeomCapsule->SetPosition(2.0f, 7.0f, -2.0f);
	GeomCapsule->InitNewton(atCapsule, 0.5f, 0.5f, 1.25f, 10.0f);
	//
	//
	// create a capsule2 test object.
	GeomNewton* GeomCapsule2 = new GeomNewton(ContextGL->aManager);
	GeomCapsule2->SetBodyType(adtDynamic);
	GeomCapsule2->SetTexture0("Textures//wood6.png", "Tex0");
	GeomCapsule2->SetDiffuseColor(0.7f, 0.7f, 0.0f);
	GeomCapsule2->SetPosition(2.0f, 7.0f, -4.0f);
	GeomCapsule2->InitNewton(atCapsule, 0.5f, 0.75f, 1.25f, 10.0f);
	//
	//
	// create a cylinderchamfer test object.
	GeomNewton* GeomCylinderChamfer = new GeomNewton(ContextGL->aManager);
	GeomCylinderChamfer->SetBodyType(adtDynamic);
	GeomCylinderChamfer->SetTexture0("Textures//wood6.png", "Tex0");
	GeomCylinderChamfer->SetDiffuseColor(0.7f, 0.0f, 0.7f);
	GeomCylinderChamfer->SetPosition(-2.0f, 7.0f, 2.0f);
	GeomCylinderChamfer->InitNewton(atCylinderChamfer, 0.5f, 0.75f, 0.0f, 10.0f);
	// 
	//
	// create a collisiontree mesh test object 
	GeomAssimp* GeomMesh = new GeomAssimp(ContextGL->aManager);
	GeomMesh->SetBodyType(adtDynamic);
	GeomMesh->SetTurnAngle(90.0f);
	GeomMesh->SetPosition(3.0f, 3.0f, 0.0f);
	GeomMesh->ImportMeshFile("Meshes//Model0//impreza3.3ds");
	// WIP: Assimp Mesh Import for the tutorial.
	// Load the texture manually after have load the mesh file.
	GeomMesh->SetTextureMeshID(0, "Meshes//Model0//chasis_a.jpg", "chasis_a");
	GeomMesh->SetTextureMeshID(1, "Meshes//Model0//ventanas.jpg", "ventanas");
	GeomMesh->SetTextureMeshID(2, "Meshes//Model0//chasis.jpg", "chasis");
	GeomMesh->InitNewton(amtCollTree);
	//
	//
	// create a convexhull mesh test object 
	GeomAssimp* GeomMeshHull = new GeomAssimp(ContextGL->aManager);
	GeomMeshHull->SetBodyType(adtDynamic);
	GeomMeshHull->SetMeshScale(0.5f, 0.5f, 0.5f);
	GeomMeshHull->SetPosition(0.0f, 8.0f, 0.0f);
	GeomMeshHull->ImportMeshFile("Meshes//Model0//impreza3.3ds");
	// WIP: Assimp Mesh Import for the tutorial.
	// Load the texture manually after have load the mesh file.
	GeomMeshHull->SetTextureMeshID(0, "Meshes//Model0//chasis_a.jpg", "chasis_a");
	GeomMeshHull->SetTextureMeshID(1, "Meshes//Model0//ventanas.jpg", "ventanas");
	GeomMeshHull->SetTextureMeshID(2, "Meshes//Model0//chasis.jpg", "chasis");
	GeomMeshHull->InitNewton(amtConvex, 0.001f, 10.0f);
	//
	if (ContextGL != NULL) {
		ContextGL->MainLoop();
		//
		// delete context and newton manager and close tutorial.
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
