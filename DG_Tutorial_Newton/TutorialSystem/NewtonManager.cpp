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
#include "NewtonManager.h"
#include "GeomGL.h"
#include "dHighResolutionTimer.h"
#include "biped.h"

//ofstream monFlux("history.txt");


// memory allocation for Newton
static void* PhysicsAlloc(int sizeInBytes)
{
	//	m_totalMemoryUsed += sizeInBytes;
	return new char[sizeInBytes];
}

// memory free use by the engine
static void PhysicsFree(void* ptr, int sizeInBytes)
{
	//	m_totalMemoryUsed -= sizeInBytes;
	delete[](char*)ptr;
}

static int UserOnAABBOverlap(const NewtonJoint* const contactJoint, dFloat timestep, int threadIndex)
{
#ifdef _DEBUG
	NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
	NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);
	NewtonJoint* const contact0 = NewtonBodyFindContact(body0, body1);
	NewtonJoint* const contact1 = NewtonBodyFindContact(body1, body0);
	dAssert(!contact0 || contact0 == contactJoint);
	dAssert(!contact1 || contact1 == contactJoint);
#endif	
	return 1;
}

void GenericContactProcess(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
	NewtonBody* const body = NewtonJointGetBody0(contactJoint);
	for (void* contact = NewtonContactJointGetFirstContact(contactJoint); contact; contact = NewtonContactJointGetNextContact(contactJoint, contact)) {
		dVector point(0.0f);
		dVector normal(0.0f);
		dVector dir0(0.0f);
		dVector dir1(0.0f);
		dVector force(0.0f);

		NewtonMaterial* const material = NewtonContactGetMaterial(contact);

		NewtonMaterialGetContactForce(material, body, &force.m_x);
		NewtonMaterialGetContactPositionAndNormal(material, body, &point.m_x, &normal.m_x);
		NewtonMaterialGetContactTangentDirections(material, body, &dir0.m_x, &dir1.m_x);
		//dFloat speed = NewtonMaterialGetContactNormalSpeed(material);

		//speed = NewtonMaterialGetContactNormalSpeed(material);
		// play sound base of the contact speed.
		//
	}
}

static void UserContactFriction(const NewtonJoint* contactJoint, dFloat timestep, int threadIndex)
{
	/*
	// call  the basic call back
	GenericContactProcess(contactJoint, timestep, threadIndex);

	const NewtonBody* const body0 = NewtonJointGetBody0(contactJoint);
	const NewtonBody* const body1 = NewtonJointGetBody1(contactJoint);

	//now core 3.14 can have per rampCollision user data
	const NewtonCollision* const collision0 = NewtonBodyGetCollision(body0);
	const NewtonCollision* const collision1 = NewtonBodyGetCollision(body1);

	NewtonCollisionMaterial material0;
	NewtonCollisionMaterial material1;
	NewtonCollisionGetMaterial(collision0, &material0);
	NewtonCollisionGetMaterial(collision1, &material1);
	dAssert((material0.m_userId == 1) || (material1.m_userId == 1));
	dFloat frictionValue = dMax(material0.m_userParam[0], material1.m_userParam[0]);

	for (void* contact = NewtonContactJointGetFirstContact(contactJoint); contact; contact = NewtonContactJointGetNextContact(contactJoint, contact)) {
		NewtonMaterial* const material = NewtonContactGetMaterial(contact);
		NewtonMaterialSetContactFrictionCoef(material, frictionValue + 0.1f, frictionValue, 0);
		NewtonMaterialSetContactFrictionCoef(material, frictionValue + 0.1f, frictionValue, 1);
	}
	*/
}

void NewtonManager::PhysicsApplyGravityForce(const NewtonBody* body, dFloat timestep, int threadIndex)
{

	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;
	dFloat mass;
	//
	NewtonWorld* nWorld = (NewtonWorld*)NewtonBodyGetWorld(body);
	NewtonManager* nManager = (NewtonManager*)NewtonWorldGetUserData(nWorld);

	NewtonBodyGetMass(body, &mass, &Ixx, &Iyy, &Izz);

	dVector force(nManager->GetGravity().Scale(mass));
	NewtonBodyAddForce(body, &force.m_x);

	// for regular gravity objects, clamp high angular velocities 
	dVector omega(0.0f);	
	NewtonBodyGetOmega(body, &omega[0]);
	dFloat mag2 = omega.DotProduct3(omega);
	dFloat maxMag = 100.0f;
	if (mag2 > (maxMag * maxMag)) {
		omega = omega.Normalize().Scale(maxMag);
		NewtonBodySetOmega(body, &omega[0]);
	}


}


void NewtonManager::TransformCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex)
{
	// Base class for the userdata permit to have multiple different class
	GeomBase* const gbase = (GeomBase*)NewtonBodyGetUserData(body);
	//
	// Check the userdata id, On this way you can have multiple class as userdata.
	// This part set the matrix to the update directly.
	// It can cause multithread race problem, the matrix need to become interpolate.
	// If you run in Async mode with multiple thread you can get little visual glitch.
	// I can update it for a better Async mode support in a later tutorial.
	// 
	if (gbase->GetClassID() == 666) {
		GeomNewton* gNewton = (GeomNewton*)gbase->GetDataPointer();
		gNewton->SetTransformMatrix((float*)matrix);
	} else
	if (gbase->GetClassID() == 777) {
		GeomAssimp* gmNewton = (GeomAssimp*)gbase->GetDataPointer();
		gmNewton->SetTransformMatrix((float*)matrix);
	}
}

NewtonManager::NewtonManager()
	:nWorld(NULL),
	//nGravity(0.0f, 0.0f, 0.0f, 0.0f),
	nGravity(0.0f, -9.81f, 0.0f, 0.0f),
	aMaxphysicfps(1.0f / 1000.0f),
	 aMicrosecunds(0),
	 aMainThreadPhysicsTimeAcc(0.0f),
	 aMainThreadPhysicsTime(0.0f),
	 aCurrentListenerTimestep(0.0f),
	 aTimestepAcc(0.0f),
	 aFps(0.0f),
	 aPhysicsFramesCount(0),
	 aAsynchronousPhysicsUpdate(false),
	 aSuspendPhysicsUpdate(false),
	 vTextureManager(NULL),
	 aLastpluginnewton(-1),
	 aCurrentPluginID(-1),
	 aCurrentPlugin("N/A"),
	 IsTerminated(false),
	 aFramesCount(0)
{   
	//
	NewtonSetMemorySystem(PhysicsAlloc, PhysicsFree);
	//
	nWorld = NewtonCreate();
#if defined(_DEBUG) && defined(_MSC_VER)
	NewtonLoadPlugins(nWorld, ".\\newtonPlugins\\Debug\\");
#else
	NewtonLoadPlugins(nWorld, ".\\newtonPlugins\\Release\\");
#endif
	NewtonWorldSetUserData(nWorld, this);
	//
	aAsynchronousPhysicsUpdate = false;
	//
	// -1 = no pluging 0 to 3 = newton plugins.
	aCurrentPluginID = 1;

	// by default runs on four micro threads
	NewtonSetThreadsCount(nWorld, 1);
	//
	NewtonSetSolverIterations(nWorld, 0);
	//
	NewtonSetNumberOfSubsteps(nWorld, 3);
	//
	NewtonSetParallelSolverOnLargeIsland(nWorld, 1);
	//
	NewtonSelectBroadphaseAlgorithm(nWorld, 1);
	//
	// WIP: I can show a better method to use material in a later tutorial version.
	// This is just a quick default setup for the first tutorial.
	int MaterialID = NewtonMaterialGetDefaultGroupID(nWorld);
	NewtonMaterialSetDefaultCollidable(nWorld, MaterialID, MaterialID, 1);
	NewtonMaterialSetDefaultSoftness(nWorld, MaterialID, MaterialID, 0.01f);
	NewtonMaterialSetDefaultElasticity(nWorld, MaterialID, MaterialID, 0.4f);
	NewtonMaterialSetDefaultFriction(nWorld, MaterialID, MaterialID, 1.25f, 1.25f);
	NewtonMaterialSetCollisionCallback(nWorld, MaterialID, MaterialID, UserOnAABBOverlap, /*UserContactFriction*/NULL);
	//
	//NewtonSetContactMergeTolerance(nWorld, ncoltol);
	NewtonInvalidateCache(nWorld);
	//
	aLastpluginnewton = aCurrentPluginID;
	void* plugin = NULL;
	if ((aCurrentPluginID >= 0) && (aCurrentPluginID < 4)) {
		int index = 0;
		for (void* ptr = NewtonGetFirstPlugin(nWorld); ptr; ptr = NewtonGetNextPlugin(nWorld, ptr)) {
			if (index == aCurrentPluginID) {
				plugin = ptr;
			}
			index++;
		}
		aLastpluginnewton = aCurrentPluginID;
		NewtonSelectPlugin(nWorld, plugin);
		aCurrentPlugin = (char*)NewtonGetPluginString(nWorld, plugin);
#if defined(_DEBUG) && defined(_MSC_VER)
		std::string pluginnew = aCurrentPlugin;
		pluginnew = "Newton Dynamics Plugin: " + pluginnew;
		printf("DG Tutorial Current - %s \n", (char*)pluginnew.c_str());
#endif
	}
	else {
		NewtonSelectPlugin(nWorld, NULL);
		std::string pluginnew = "N/A";
		aCurrentPlugin = pluginnew;
#if defined(_DEBUG) && defined(_MSC_VER)
		pluginnew = "Newton Dynamics Plugin: " + pluginnew;
		printf("DG Tutorial Current - %s \n", (char*)pluginnew.c_str());
#endif
	}
    //
	vTextureManager = new Texture();
}

void NewtonManager::CalculateFPS(dFloat timestep)
{
	aFramesCount++;
	aTimestepAcc += timestep;

	// this probably happing on loading of and a pause, just rest counters
	if ((aTimestepAcc <= 0.0f) || (aTimestepAcc > 2.0f)) {
		aTimestepAcc = 0;
		aFramesCount = 0;
	}

	//update fps every quarter of a second
	if (aTimestepAcc >= 0.25f) {
		aFps = dFloat(aFramesCount) / aTimestepAcc;
		aTimestepAcc -= 0.25f;
		aFramesCount = 0;
	}
}

dFloat NewtonManager::GetFps()
{
	return aFps;
}

dVector NewtonManager::GetGravity()
{
	return nGravity;
}

NewtonWorld* NewtonManager::GetWorld()
{
	return nWorld;
}

void NewtonManager::UpdateNewton(dFloat steptime)
{
	// update the physics
	if (nWorld && !aSuspendPhysicsUpdate) {
        //
		dFloat timestepInSecunds = aMaxphysicfps;
		unsigned64 timestepMicrosecunds = unsigned64(timestepInSecunds * 1000000.0f);
		//
		unsigned64 currentTime = dGetTimeInMicrosenconds();
		unsigned64 nextTime = currentTime - aMicrosecunds;
		if (nextTime > timestepMicrosecunds * 2) { 
			aMicrosecunds = currentTime - timestepMicrosecunds * 2;
			nextTime = currentTime - aMicrosecunds;
		}
		//
		bool newUpdate = false;
		dFloat physicsTime = 0.0f;
		//while (nextTime >= timestepMicrosecunds) 
		if (nextTime >= timestepMicrosecunds)
		{
			newUpdate = true;
			//ClearDebugDisplay(m_world);
			//
			if (aAsynchronousPhysicsUpdate) {
				NewtonUpdateAsync(nWorld, timestepInSecunds);
			}
			else {
				NewtonUpdate(nWorld, timestepInSecunds);
			}
			//
			physicsTime += NewtonGetLastUpdateTime(nWorld);
//			monFlux <<  physicsTime << std::endl;
			//
			nextTime -= timestepMicrosecunds;
			aMicrosecunds += timestepMicrosecunds;
		}
		//
		if (newUpdate) {
			aPhysicsFramesCount++;
			aMainThreadPhysicsTimeAcc += physicsTime;
			if (aPhysicsFramesCount >= 16) {
				aMainThreadPhysicsTime = aMainThreadPhysicsTimeAcc / aPhysicsFramesCount;
				aPhysicsFramesCount = 0;
				aMainThreadPhysicsTimeAcc = 0.0f;
			}
		}
	}
}

dFloat NewtonManager::GetPhysicTime()
{
	return aMainThreadPhysicsTime;
}

void NewtonManager::Render(Shader* cshd, dFloat steptime)
{
	for (auto itr = vAssMeshList.begin(); itr != vAssMeshList.end(); itr++) {
		((GeomAssimp*)*itr)->RenderMeshes(cshd, steptime);
	}
	//
	for (auto itr = vGeomList.begin(); itr != vGeomList.end(); itr++) {
	  ((GeomNewton*)*itr)->Render(cshd, steptime);
	}
	for (auto itr = vMuscleList.begin(); itr != vMuscleList.end(); itr++) {
		((Muscle*)*itr)->UpdateLineCoord(cshd, steptime);
	}
}

NewtonManager::~NewtonManager()
{
	//
	//for (auto itr = vJointList.begin();
	//	itr != vJointList.end(); itr++) {
		//dCustomJoint* aobj = (dCustomJoint*)*itr;
		//delete aobj;
	//}
	vJointList.clear();
	//
	for (auto itr = vAssMeshList.begin();
		itr != vAssMeshList.end(); itr++) {
		GeomAssimp* aobj = (GeomAssimp*)*itr;
		delete aobj;
	}
	vAssMeshList.clear();
	//
	for (auto itr = vGeomList.begin();
		itr != vGeomList.end(); itr++) {
		GeomNewton* aobj = (GeomNewton*)*itr;
		delete aobj;
	}
	vGeomList.clear();
	//
	for (auto itr = vMuscleList.begin();
		itr != vMuscleList.end(); itr++) {
		Muscle* aobj = (Muscle*)*itr;
		delete aobj;
	}
	vMuscleList.clear();
	//
	if (vTextureManager)
	  delete vTextureManager;
	//

	NewtonDestroyAllBodies(nWorld);
	NewtonDestroy(nWorld);
}

void NewtonManager::SetExcitationList(const std::vector<float> iExcitationList)
{
	const int iMuscleListSize = this->vMuscleList.size();
	if (iExcitationList.size() != iMuscleListSize)
	{
		cout << "Incorrect usage of method NewtonManager::SetExcitationList" << endl;
		return;
	}

	int iCount = 0;
	for (auto itr = vMuscleList.begin();
		itr != vMuscleList.end(); itr++) {
		Muscle* aobj = (Muscle*)*itr;
		aobj->SetExcitation(iExcitationList[iCount]);
		iCount++;
	}

}