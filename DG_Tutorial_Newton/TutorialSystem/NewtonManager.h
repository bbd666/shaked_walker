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
#ifndef NEWTONMANAGER_H
#define NEWTONMANAGER_H
#pragma once
#include "pch.h"
#include "shader.h"
#include "texture.h"

struct NewtonManager
{   // public
	NewtonManager();
	~NewtonManager();
	NewtonWorld* GetWorld();
	dVector GetGravity();
	void Render(Shader* cshd, dFloat steptime);
	void UpdateNewton(dFloat steptime);
	// Surely not the best method but for the tutorial it is ok I think...
	std::vector<void*> vGeomList;
	std::vector<void*> vAssMeshList;
	std::vector<void*> vJointList;
	std::vector<void*> vMuscleList;
	std::vector<void*> vMuscleV2List;
	Texture* vTextureManager;
	//
	bool IsTerminated;
	//
	// WIP: I can show in the later tutorial version how to update all newton options in runtime.
	//void UpdateNewtonOptions();
	//
	void CalculateFPS(dFloat timestep);
	dFloat GetFps();
	dFloat GetPhysicTime();
	static void PhysicsApplyGravityForce(const NewtonBody* body, dFloat timestep, int threadIndex);
	static void PhysicsApplyControlForce(const NewtonBody* body1, bool action, dFloat timestep, int threadIndex);
	static void TransformCallback(const NewtonBody* body, const dFloat* matrix, int threadIndex);
	void SetExcitationList(const std::vector<float> iExcitationList);
private:
	int aLastpluginnewton; 
	int aCurrentPluginID;
	std::string aCurrentPlugin;
	bool aSuspendPhysicsUpdate;
	bool aAsynchronousPhysicsUpdate;
	int aPhysicsFramesCount;
	int aFramesCount;
	dFloat aMaxphysicfps;
	dFloat aFps;
	dFloat aTimestepAcc;
	dFloat aCurrentListenerTimestep;
	dFloat aMainThreadPhysicsTime;
	dFloat aMainThreadPhysicsTimeAcc;
	unsigned64 aMicrosecunds;
	NewtonWorld* nWorld;
	dVector nGravity;
};



#endif // NEWTONMANAGER_H
