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
#ifndef WINDOWGL_H
#define WINDOWGL_H
#pragma once

#include "pch.h"
#include "shader.h"
#include "VertexGL.h"
#include "Camera.h"
#include "GeomGL.h"
#include "NewtonManager.h"

// General glfw callback's.
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
static void glfw_error_callback(int error, const char* description);

struct NewtonMousePick
{ // public
	NewtonMousePick(MainCamera* mCamera, NewtonManager* mManager);
	~NewtonMousePick();
	//void UpdateMousePick();
	dVector ScreenToWorld(const dVector& screenPoint);
	void MouseButton(int button, int action, int mods);
	void MouseMove(double msx, double msy);
	void UnPickBody();
	//
    NewtonBody* aPickBody;
	dFloat aParam;
	dVector aNormal;
private:
	dCustomKinematicController* aPickJoint;
	NewtonManager* aManager;
	glm::ivec4 aViewport;
	MainCamera* aCamera;
	int aMousebutton;
	int aMouseaction;
	int aMousemods;
};

struct LineVertex
{
	glm::vec3 point1;
	glm::vec3 point2;
	glm::vec4 color;
	int lineID;
};

struct LineDebugManager
{
	LineDebugManager(MainCamera* mainCam);
	~LineDebugManager();

	void InitBufferGL();
	//
	int AddLine(glm::vec3 linepos1, glm::vec3 linepos2, glm::vec3 linecolor);
	//
	void RenderLine(double steptime);
	//
	std::vector<LineVertex*> aLineList;
	std::vector<MainVertexPC> aLineBuffer;
	//
	GLuint aLineVbo;
	GLuint aLineVao;
	MainCamera* aCamera;
	Shader* aShader;
	dMatrix aMatrix;
};
/**
 * @struct WindowMain
 * Main window definition. The simulation environment is created inside this window.
 */
struct WindowMain
{   // public
	WindowMain(int dwidth = 1024, int dheight = 768);
	virtual ~WindowMain();

	//
	void MainLoop();
	void InitGLRender();
	void MainRender(string info, int iteration, int max_iteration, int remaining_time);
	void ContextResize(int cw, int ch);
	void ContextKeypress(int key, int scancode, int action, int mods);
	void ContextMouseButton(int button, int action, int mods);
	void ContextMouseMove(double xpos, double ypos);
	void ContextMouseScrl(float scrlx, float scrly);
	void SetUseMouseViewRotation(bool val);
	int GetMousebutton();
	int GetMousebuttonaction();
	int GetMousebuttonmods();
	double GetMouseMoveX();
	double GetMouseMoveY();
	NewtonMousePick* GetMousePicking();
	MainCamera* GetCamera();
	//
	NewtonManager* aManager;
	//
	LineDebugManager* aLineManager;
	void SetSimulationTime(float time);

	float GetSimulationTime();

	void SetMaxSimulationTime(float time);

	float GetMaxSimulationTime();

	void SetSimulationInfo(int operation_, int iteration_, int max_iteration_, int remaining_time_, string info_wbv, float Amp, float freq);
	float GetWVBFreq();
	float GetWVBAmp();
private:
	NewtonMousePick* aMousePick;
	GLFWwindow* contextGL;
	Shader* MainShader;
	MainCamera* Camera;
	glm::vec4 contextcolor;
	int mainwidth;
	int mainheight;
	int mousebutton;
	int mousebuttonaction;
	int mousebuttonmods;
	double mouvemovex;
	double mouvemovey;
	bool usemouserotate;
	float mousescrlx;
	float mousescrly;
	int delayerfps;
	
	float Time;
	float Max_Time;
	int operation, iteration, max_iteration, remaining_time;
	string wbvinfo;
	float WBV_amp, WBV_freq;
};

// WIP: unimplemented, I let's it present just to remember have I need to do later.
static WindowMain* singletonContextGL = NULL;

#endif //WINDOWGL_H
