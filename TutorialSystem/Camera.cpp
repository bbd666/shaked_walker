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
// WIP: Just a very simple camera helper
#include "pch.h"
#include "Camera.h"

dFloat afmod(dFloat ax, dFloat ay) 
{
	return (ax - ay * floor(ax / ay));
}

MainCamera::MainCamera()
	: aMatProj(1.0f),
	  aMatView(1.0f),
	  aEye(glm::vec3(0.0f, 0.0f, 0.0f)),
	  aCenter(glm::vec3(0.0f, 0.0f, 0.0f)),
	  aUp(glm::vec3(0.0f, 1.0f, 0.0f)),
	  afov(45.0f),
      awidth(1024),
      aheight(768),
      azNear(0.01f),
      azFar(5000.0f),
	  aAspectRatio(1.0f),
	  aCurrentRotationX(0.0f),
      aCurrentRotationY(0.0f),
      aCamposrotoldX(0.0f),
      aCamposrotoldY(0.0f),
      oldRotationX(0.0f),
      oldRotationY(0.0f),
      aOrientationchanged(false),
	  aVerticalAngle(0.0f),
      aHorizontalAngle(0.0f)
{

}

glm::vec3 MainCamera::GetEye()
{
	return aEye;
}

glm::vec3 MainCamera::GetCenter()
{
	return aCenter;
}

glm::vec3 MainCamera::GetUp()
{
	return aUp;
}

int MainCamera::GetContextWidth()
{
	return awidth;
}

int MainCamera::GetContextHeight()
{
	return aheight;
}

void MainCamera::MoveAround(double amousex, double amousey)
{
  //
  aCurrentRotationX = aCamposrotoldX - (float)amousex;
  aCurrentRotationY = aCamposrotoldY - (float)amousey;
  //
  oldRotationX = aCurrentRotationX;
  oldRotationY = aCurrentRotationY;
  //
  aCamposrotoldX = (float)amousex;
  aCamposrotoldY = (float)amousey;
  //
  offsetOrientation(-aCurrentRotationY * 0.1f, -aCurrentRotationX * 0.1f);
}

void MainCamera::MouseAction(double amousex, double amousey, int aaction)
{
  if (aaction == 0) {
    aCamposrotoldX = (float)amousex;
    aCamposrotoldY = (float)amousey;
  }
}

void MainCamera::normalizeAngles()
{
    aHorizontalAngle = afmod(aHorizontalAngle, 360.0f);
	//
	if (aHorizontalAngle < 0.0)
		aHorizontalAngle  = aHorizontalAngle + 360.0f;
	//
    aVerticalAngle = afmod(aVerticalAngle, 360.0f);
	//
	if (aVerticalAngle < 0.0)
	  aVerticalAngle  = aVerticalAngle + 360.0f;
}

void MainCamera::offsetOrientation(dFloat upAngle, dFloat rightAngle)
{
  aHorizontalAngle = aHorizontalAngle + rightAngle;
  aVerticalAngle = aVerticalAngle + upAngle;
  //
  normalizeAngles();
  //
  aOrientationchanged = true;
}

void MainCamera::SetPosition(float px, float py, float pz)
{
	aEye = glm::vec3(px, py, pz);
}

void MainCamera::SetCenter(float cx, float cy, float cz)
{
	aCenter = glm::vec3(cx, cy, cz);
}

void MainCamera::SetUp(float upx, float upy, float upz)
{
	aUp = glm::vec3(upx, upy, upz);
}

glm::mat4 MainCamera::GetMatrixProjection()
{
	return aMatProj;
}

glm::mat4 MainCamera::GetMatrixView()
{
	return aMatView;
}

void MainCamera::ApplyView(int dwidth, int dheight)
{
	awidth = dwidth;
	aheight = dheight;
	//
	glm::mat4 avb;
	glm::mat4 rotViewMatrix = glm::mat4(1.0f);
	//
	aMatProj = glm::perspectiveRH(glm::radians(afov), aAspectRatio * ((float)awidth / (float)aheight), azNear, azFar);
	aMatView = glm::lookAtRH(aEye, aCenter, aUp);
	//
	avb = aMatView;
	//
	if (aOrientationchanged == true) {
	  //
	  rotViewMatrix = glm::rotate(rotViewMatrix, glm::radians(-aVerticalAngle), glm::vec3(1.0, 0, 0));
	  avb =  avb * rotViewMatrix;
	  //
	  rotViewMatrix = glm::rotate(rotViewMatrix, glm::radians(aHorizontalAngle), glm::vec3(0, 1.0, 0));
	  avb = avb * rotViewMatrix;
	  //
	  aMatView = avb;
	  //
	  //orientationchanged = false;
	}
}

MainCamera::~MainCamera()
{

}