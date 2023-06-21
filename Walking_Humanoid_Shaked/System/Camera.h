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
//
// WIP: Just a very simple camera helper
#ifndef CAMERA_H
#define CAMERA_H
#pragma once
#include "pch.h"

struct MainCamera
{   // public
	MainCamera();
	~MainCamera();
	void SetPosition(float px, float py, float pz);
	void SetCenter(float cx, float cy, float cz);
	void SetUp(float upx, float upy, float upz);
	void ApplyView(int dwidth, int dheight);
	void MoveAround(double amousex, double amousey);
	void MouseAction(double amousex, double amousey, int aaction);
	//
	glm::mat4 GetMatrixProjection();
	glm::mat4 GetMatrixView();
	int GetContextWidth();
	int GetContextHeight();
	glm::vec3 GetEye();
	glm::vec3 GetCenter();
	glm::vec3 GetUp();

private:
	void offsetOrientation(dFloat upAngle, dFloat rightAngle);
	void normalizeAngles();
	//
	float afov;
	float aAspectRatio;
	int awidth;
	int aheight;
	float azNear;
	float azFar;
	glm::mat4 aMatProj;
	glm::mat4 aMatView;
	//
	glm::vec3 aEye;
	glm::vec3 aCenter;
	glm::vec3 aUp;
	float aCurrentRotationX;
	float aCurrentRotationY;
	float aCamposrotoldX;
	float aCamposrotoldY;
	float oldRotationX;
	float oldRotationY;
	float aHorizontalAngle;
	float aVerticalAngle;
	bool aOrientationchanged;
};

#endif //CAMERA_H
