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
#include "dHighResolutionTimer.h"
#include "imgui.h"
#include "imgui_impl_glfw_gl3.h"

using namespace std;

static dFloat RayCastFilter(const NewtonBody* const body, const NewtonCollision* const collisionHit, const dFloat* const contact, const dFloat* const normal, dLong collisionID, void* const userData, dFloat intersetParam)
{
	dFloat mass;
	dFloat Ixx;
	dFloat Iyy;
	dFloat Izz;

	// check if we are hitting a sub shape
	const NewtonCollision* const parent = NewtonCollisionGetParentInstance(collisionHit);
	if (parent) {
		// you can use this to filter sub collision shapes.  
		dAssert(NewtonCollisionGetSubCollisionHandle(collisionHit));
	}

	NewtonMousePick* const data = (NewtonMousePick*)userData;
	NewtonBodyGetMass(body, &mass, &Ixx, &Iyy, &Izz);
	if ((mass > 0.0f) || (NewtonBodyGetType(body) == NEWTON_KINEMATIC_BODY)) {
		data->aPickBody = (NewtonBody*)body;
		GeomBase* guserdata = (GeomBase*)NewtonBodyGetUserData(data->aPickBody);
		guserdata->IsPicked = true;
	}

	if (intersetParam < data->aParam) {
		data->aParam = intersetParam;
		data->aNormal = dVector(normal[0], normal[1], normal[2]);
	}

	return intersetParam;
}

static unsigned RayCastPrefilter(const NewtonBody* body, const NewtonCollision* const collision, void* const userData)
{
	// ray cannot pick trigger volumes
	//return NewtonCollisionIsTriggerVolume(collision) ? 0 : 1;

	const NewtonCollision* const parent = NewtonCollisionGetParentInstance(collision);
	if (parent) {
		// you can use this to filter sub collision shapes.  
		dAssert(NewtonCollisionGetSubCollisionHandle(collision));
	}

	return (NewtonBodyGetType(body) == NEWTON_DYNAMIC_BODY) ? 1 : 0;
}

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	WindowMain* acontext = (WindowMain*)glfwGetWindowUserPointer(window);
	acontext->ContextKeypress(key, scancode, action, mods);
}

static void glfw_error_callback(int error, const char* description)
{
	cout << "Tutorial CONTEXT ERROR: " << error << " " << description << endl;
}

static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	WindowMain* acontext = (WindowMain*)glfwGetWindowUserPointer(window);
	GLsizei w1 = width;
	GLsizei h1 = height;
	if (w1 < 1) w1 = 1;
	if (h1 < 1) h1 = 1;
	acontext->ContextResize(w1, h1);
	glViewport(0, 0, (GLsizei)w1, (GLsizei)h1);
}

static void glfw_mouse_button_callback(GLFWwindow* awindow, int button, int action, int mods)
{
	WindowMain* acontext = (WindowMain*)glfwGetWindowUserPointer(awindow);
	//
	acontext->ContextMouseButton(button, action, mods);
}

static void glfw_mouse_position_callback(GLFWwindow* awindow, double xpos, double ypos)
{
	WindowMain* acontext = (WindowMain*)glfwGetWindowUserPointer(awindow);
	//
	acontext->ContextMouseMove(xpos, ypos);
}

static void glfw_mouse_scroll_callback(GLFWwindow* awindow, double xoffset, double yoffset)
{
	WindowMain* acontext = (WindowMain*)glfwGetWindowUserPointer(awindow);
	//
	acontext->ContextMouseScrl((float)xoffset, (float)yoffset);
}

void GetDesktopResolution(int& horizontal, int& vertical)
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	horizontal = desktop.right;
	vertical = desktop.bottom;
}

///////////////////////////////////////////////////////////////

NewtonMousePick::NewtonMousePick(MainCamera* mCamera, NewtonManager* mManager)
  : aPickJoint(NULL),
	aPickBody(NULL),
	aCamera(mCamera),
	aMousebutton(0),
    aMouseaction(0),
    aMousemods(0),
	aManager(mManager)
{
	aViewport[0] = 0;
	aViewport[1] = 0;
	aViewport[2] = aCamera->GetContextWidth();
	aViewport[3] = aCamera->GetContextHeight();
}

dVector NewtonMousePick::ScreenToWorld(const dVector& screenPoint)
{
	GLdouble winX = screenPoint.m_x; //Store the x cord;
	GLdouble winY = screenPoint.m_y; //Store the y cord
	GLdouble winZ = screenPoint.m_z; //Store the Z cord

	aViewport[2] = aCamera->GetContextWidth();
	aViewport[3] = aCamera->GetContextHeight();

	//Now windows coordinates start with (0,0) being at the top left 
	//whereas OpenGL cords start lower left so we have to do this to convert it: 
	//Remember we got viewport value before 
	winY = (dFloat)aViewport[3] - winY;

	glm::vec3 obj;
	obj = glm::unProject(glm::vec3(winX, winY, winZ), aCamera->GetMatrixView(), aCamera->GetMatrixProjection(), aViewport);

	return dVector(obj.x, obj.y, obj.z);
}

void NewtonMousePick::MouseButton(int button, int action, int mods)
{
  aMousebutton = button;
  aMouseaction = action;
  aMousemods = mods;
}

void NewtonMousePick::MouseMove(double msx, double msy)
{
	//
	if ((aPickBody == NULL) && (aMousebutton == 0)) {
      // not supposed to happen, just for prevention.
	  if (aPickJoint) {
		delete aPickJoint;
		aPickJoint = NULL;
	  }
	  dFloat x = dFloat(msx);
	  dFloat y = dFloat(msy);
	  dVector p0(ScreenToWorld(dVector(x, y, 0.0f, 0.0f)));
	  dVector p1(ScreenToWorld(dVector(x, y, 1.0f, 0.0f)));
	  dVector posit;
	  //
	  aParam = 1.1f;
	  //
	  NewtonWorldRayCast(aManager->GetWorld(), &p0[0], &p1[0], RayCastFilter, this, RayCastPrefilter, 0);
	  //
	  posit = p0  + (p1 - p0).Scale(aParam);
	  //
	  if (aPickBody) {
		  if (!aPickJoint) {
			  aPickJoint = new dCustomKinematicController((NewtonBody*)aPickBody, posit);
			  dFloat Ixx;
			  dFloat Iyy;
			  dFloat Izz;
			  dFloat mass;
			  NewtonBodyGetMass(aPickBody, &mass, &Ixx, &Iyy, &Izz);

			  const dFloat angularFritionAccel = 10.0f;
			  const dFloat linearFrictionAccel = 400.0f * dMax(dAbs(-10.0f), dFloat(10.0f));
			  const dFloat inertia = dMax(Izz, dMax(Ixx, Iyy));
			  //
			  aPickJoint->SetControlMode(dCustomKinematicController::m_linearPlusAngularFriction);
		      aPickJoint->SetMaxLinearFriction(mass * linearFrictionAccel);
		      aPickJoint->SetMaxAngularFriction(inertia * angularFritionAccel);
		  }
	  }
	}
	else {
		if ((aPickJoint) && (aMousebutton == 0) && (aMouseaction == 1)) {
			dFloat ax = dFloat(msx);
			dFloat ay = dFloat(msy);
			dVector ap0(ScreenToWorld(dVector(ax, ay, 0.0f, 0.0f)));
			dVector ap1(ScreenToWorld(dVector(ax, ay, 1.0f, 0.0f)));
			dVector targetposit;
			targetposit = ap0 + (ap1 - ap0).Scale(aParam);
			aPickJoint->SetTargetPosit(targetposit);
		}
	}
	//
	if (aMouseaction == 0) {
		aMousebutton = -1;
		UnPickBody();
	}
}

void NewtonMousePick::UnPickBody()
{
	aMousebutton = -1;
	//
	if (aPickBody) {
		GeomBase* guserdata = (GeomBase*)NewtonBodyGetUserData(aPickBody);
		guserdata->IsPicked = false;
		aPickBody = NULL;
	}
	if (aPickJoint) {
		delete aPickJoint;
		aPickJoint = NULL;
	}
}

NewtonMousePick::~NewtonMousePick()
{
	UnPickBody();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

LineDebugManager::LineDebugManager(MainCamera* mainCam) :
	aCamera(mainCam),
	aMatrix(dGetIdentityMatrix()),
	aShader(NULL),
	aLineVbo(0),
	aLineVao(0)
{
	if (!aShader)
		aShader = new Shader("LinesManager");
	//
	if (aShader) {
		aShader->attachShader(BaseShader("Shaders/DebugLine_VS.vert"));
		aShader->attachShader(BaseShader("Shaders/DebugLine_FS.frag"));
		// linking shader.
		aShader->linkPrograms();
	}
}
//
LineDebugManager::~LineDebugManager()
{
	if (aShader)
		delete aShader;
}
//
void LineDebugManager::InitBufferGL()
{
	MainVertexPC avert;
	//
	for (int i = 0; i < 2; i++) {
		avert.posit.x = 9999.999f;
		avert.posit.y = 9999.999f;
		avert.posit.z = 9999.999f;
		//
		avert.color.x = 1.0f;
		avert.color.y = 1.0f;
		avert.color.z = 1.0f;
		//
		aLineBuffer.push_back(avert);
	}
	//
	glGenVertexArrays(1, &aLineVao);
	if (aLineVao) {
		//
		glBindVertexArray(aLineVao);
		//
		glGenBuffers(1, &aLineVbo);
		//
		if (aLineVbo) {
			glBindBuffer(GL_ARRAY_BUFFER, aLineVbo);
			//
			glBufferData(GL_ARRAY_BUFFER, sizeof(MainVertexPC) * aLineBuffer.size(), &aLineBuffer[0], GL_DYNAMIC_DRAW);
			//
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(MainVertexPC), (void*)0);
			//
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(MainVertexPC), (void*)(offsetof(MainVertexPC, MainVertexPC::color)));
			//
			//glDisableVertexAttribArray(1);
			//glDisableVertexAttribArray(0);
			//
			//aBufferInited = true;
			//printf("OX_INFO: GL Debug lines static buffer create and inited. \n");
		}
		//
		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}
//
int LineDebugManager::AddLine(glm::vec3 linepos1, glm::vec3 linepos2, glm::vec3 linecolor)
{
	MainVertexPC avert1;
	avert1.posit = linepos1;
	avert1.color = linecolor;
	//
	MainVertexPC avert2;
	avert2.posit = linepos2;
	avert2.color = linecolor;
	//
	aLineBuffer.push_back(avert1);
	aLineBuffer.push_back(avert2);
	//
	glBindVertexArray(aLineVao);
	glBindBuffer(GL_ARRAY_BUFFER, aLineVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(MainVertexPC) * aLineBuffer.size(), &aLineBuffer[0], GL_DYNAMIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	
	return (int)aLineBuffer.size();
}
//
void LineDebugManager::RenderLine(double steptime)
{
	if ((!aShader) || (!aCamera)) return;
	//
	aShader->use();
	//printf("debug test \n");
	//
	aShader->setMat4("Projection_Matrix", aCamera->GetMatrixProjection());
	aShader->setMat4("View_Matrix", aCamera->GetMatrixView());
	aShader->setNMat4("Model_Matrix", aMatrix);
	//
	glBindVertexArray(aLineVao);
	//if (aRequestStaUpdate) {
	glBindBuffer(GL_ARRAY_BUFFER, aLineVbo);
	//	if (firstupdate0)
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(MainVertexPC) * aLineBuffer.size(), &aLineBuffer[0]);
	//	else
			//glBufferData(GL_ARRAY_BUFFER, sizeof(MainVertexPC) * aLineBuffer.size(), &aLineBuffer[0], GL_DYNAMIC_DRAW);
		//
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//	aRequestStaUpdate = false;
	//	firstupdate0 = true;
	//}
	//
	glDrawArrays(GL_LINES, 0, (GLsizei)(aLineBuffer.size()));
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

WindowMain::WindowMain(int dwidth, int dheight)
	:MainShader(NULL), 
	contextcolor(glm::vec4(0.125f, 0.125f, 0.125f, 1.0f)),
	Camera(NULL),
	mainwidth(dwidth),
	mainheight(dheight),
    mousebutton(0),
    mousebuttonaction(0),
    mousebuttonmods(0),
	aMousePick(NULL),
	mouvemovex(0.0),
    mouvemovey(0.0),
	usemouserotate(false),
    mousescrlx(0.0f),
    mousescrly(0.0f),
	aManager(NULL),
	aLineManager(NULL),
	delayerfps(0)
{
#if defined(_DEBUG) && defined(_MSC_VER)
	// Track all memory leaks at the operating system level.
	// make sure no Newton tool or utility leaves leaks behind.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF));
	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
	//_CrtSetBreakAlloc (1882);
#endif
	singletonContextGL = NULL;
	if (!glfwInit())
		exit(EXIT_FAILURE);
	//
	aManager = new NewtonManager();

	//
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//
	contextGL = glfwCreateWindow(dwidth, dheight, "Walking Humanoid", NULL, NULL);
	//
	if (contextGL == NULL) {
#if defined(_DEBUG) && defined(_MSC_VER)
		fprintf(stderr, "Failed to open GLFW window, Try with lower gl version or update your video card driver.\n");
#endif
		getchar();
		glfwTerminate();
		return;
	}
	glfwMakeContextCurrent(contextGL);
	//
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
#if defined(_DEBUG) && defined(_MSC_VER)
		fprintf(stderr, "Failed to initialize GLEW\n");
#endif
		getchar();
		glfwTerminate();
		return;
	}	
	//
	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(contextGL, GLFW_STICKY_KEYS, GL_TRUE);
	// Hide the mouse and enable unlimited mouvement
	// GLFW_CURSOR_DISABLED
	glfwSetInputMode(contextGL, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	//
	int deskwidth, deskheight = 0;
	GetDesktopResolution(deskwidth, deskheight);
	// Set the mouse at the center of the screen 
	glfwPollEvents();
	glfwSetCursorPos(contextGL, dwidth / 2, dheight / 2);
	glfwSetWindowPos(contextGL, (deskwidth / 2) - (dwidth / 2), (deskheight / 2) - (dheight / 2));
	//
	glfwSetWindowUserPointer(contextGL, this);
	//glfwSetWindowFocusCallback(contextGL, glfw_window_focus_callback);
	glfwSetErrorCallback(glfw_error_callback);
	glfwSetKeyCallback(contextGL, glfw_key_callback);
	glfwSetFramebufferSizeCallback(contextGL, glfw_framebuffer_size_callback);
	glfwSetMouseButtonCallback(contextGL, glfw_mouse_button_callback);
	glfwSetCursorPosCallback(contextGL, glfw_mouse_position_callback);
	glfwSetScrollCallback(contextGL, glfw_mouse_scroll_callback);
	// PS: Use 1 for active vsync...
	glfwSwapInterval(0);
	//glfwSetJoystickCallback(ox3DContext::joystick_callback);
	//
	dResetTimer();
	//
	// Initialize some gl commands
	InitGLRender();
	//
}

void WindowMain::ContextKeypress(int key, int scancode, int action, int mods)
{
	if (contextGL) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(contextGL, GLFW_TRUE);
	}
}

void WindowMain::SetFootPos_L(string s)
{
	FootPos_L=s;
}

NewtonMousePick* WindowMain::GetMousePicking()
{
	return aMousePick;
}

void WindowMain::ContextMouseScrl(float scrlx, float scrly)
{
	mousescrlx = scrlx;
	mousescrly = scrly;
	//
	if (Camera) {
		if (mousescrly > 0.001) {
			glm::vec4 forwa = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
			glm::vec4 ayes(glm::vec4(Camera->GetEye(), 1.0f) + forwa);
			Camera->SetPosition(ayes.x, ayes.y, ayes.z);
			mousescrly = 0.0;
		}
		if (mousescrly < -0.001) {
			glm::vec4 forwa = glm::vec4(0.0f, 0.0f, -1.0f, 1.0f);
			glm::vec4 ayes(glm::vec4(Camera->GetEye(), 1.0f) + forwa);
			Camera->SetPosition(ayes.x, ayes.y, ayes.z);
			mousescrly = 0.0;
		}
	}
}

void WindowMain::SetUseMouseViewRotation(bool val)
{
	usemouserotate = val;
}

void WindowMain::ContextMouseButton(int button, int action, int mods)
{
	mousebutton = button;
	mousebuttonaction = action;
	mousebuttonmods = mods;
	if (aMousePick) {
		aMousePick->MouseButton(mousebutton, mousebuttonaction, mousebuttonmods);
	}
}

void WindowMain::ContextMouseMove(double xpos, double ypos)
{
	mouvemovex = xpos;
	mouvemovey = ypos;
	if ((Camera) && (usemouserotate)) {
		Camera->MouseAction(mouvemovex, mouvemovey, mousebuttonaction);
		//
		if ((mousebutton == 1) && (mousebuttonaction == 1)) {
			Camera->MoveAround(mouvemovex, mouvemovey);
		}
	}
}

double WindowMain::GetMouseMoveX()
{
	return mouvemovex;
}

double WindowMain::GetMouseMoveY()
{
	return mouvemovey;
}

int WindowMain::GetMousebutton()
{
	return mousebutton;
}

int WindowMain::GetMousebuttonaction()
{
	return mousebuttonaction;
}

int WindowMain::GetMousebuttonmods()
{
	return mousebuttonmods;
}

void WindowMain::ContextResize(int cw, int ch)
{
	mainwidth = cw;
	mainheight = ch;
}

MainCamera* WindowMain::GetCamera()
{
	return Camera;
}

void WindowMain::MainLoop()
//initialize ImGui
{
ImGui_ImplGlfwGL3_Init(contextGL, false);

//initialize excitation list
//std::vector<float> initExcitation = { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0,
//										  0.0, 0.0, 0.0, 0.0, 0.0 , 0.0 , 0.0, 0.0 };
std::vector<float> initExcitation = { 0.0 };
while (!glfwWindowShouldClose(contextGL))
{
	//create empty frame
	ImGui_ImplGlfwGL3_NewFrame();

	// populate frame with 16 Sliders: 8 for left, 8 for right
	{
		ImGui::Text("Excitation");
		ImGui::SliderFloat("HFL Left", &initExcitation[0], 0.0f, 1.0f);
		/*ImGui::Columns(2);

		ImGui::SliderFloat("SOL Left", &initExcitation[0], 0.0f, 1.0f);
		ImGui::SliderFloat("TA Left", &initExcitation[1], 0.0f, 1.0f);
		ImGui::SliderFloat("GAS Left", &initExcitation[2], 0.0f, 1.0f);
		ImGui::SliderFloat("VAS Left", &initExcitation[3], 0.0f, 1.0f);
		ImGui::SliderFloat("HAM Left", &initExcitation[4], 0.0f, 1.0f);
		ImGui::SliderFloat("RF Left", &initExcitation[5], 0.0f, 1.0f);
		ImGui::SliderFloat("GLU Left", &initExcitation[6], 0.0f, 1.0f);
		ImGui::SliderFloat("HFL Left", &initExcitation[7], 0.0f, 1.0f);

		ImGui::NextColumn();

		ImGui::SliderFloat("SOL Right", &initExcitation[8], 0.0f, 1.0f);
		ImGui::SliderFloat("TA Right", &initExcitation[9], 0.0f, 1.0f);
		ImGui::SliderFloat("GAS Right", &initExcitation[10], 0.0f, 1.0f);
		ImGui::SliderFloat("VAS Right", &initExcitation[11], 0.0f, 1.0f);
		ImGui::SliderFloat("HAM Right", &initExcitation[12], 0.0f, 1.0f);
		ImGui::SliderFloat("RF Right", &initExcitation[13], 0.0f, 1.0f);
		ImGui::SliderFloat("GLU Right", &initExcitation[14], 0.0f, 1.0f);
		ImGui::SliderFloat("HFL Right", &initExcitation[15], 0.0f, 1.0f);*/

		//set excitation values if the ImGui box is clicked
		//if not for this check, values of excitation are set redundantly at every frame
		if (ImGui::IsMouseHoveringWindow() && ImGui::IsMouseDown(0))
			aManager->SetExcitationList(initExcitation);
	}

	// Rendering
	MainRender();
	ImGui::Render();
	glfwSwapBuffers(contextGL);
	glfwPollEvents();
}
//close ImGui on exit
ImGui_ImplGlfwGL3_Shutdown();
}

void WindowMain::InitGLRender()
{
	//
	// Prepare the initial opengl commands.
	//
	if (!Camera) {
		Camera = new MainCamera();
		Camera->SetUp(0.0f, 1.0f, 0.0f);
		Camera->SetCenter(0.0f, 0.0f, 0.0f);
		Camera->SetPosition(0.0f, 5.0f, -20.0f);
		//
		if (!aMousePick)
		  aMousePick = new NewtonMousePick(Camera, aManager);
	}
	//
	glClearDepth(1.0);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//
	glClearColor(contextcolor.r, contextcolor.g, contextcolor.b, contextcolor.a);
	//
	// Just for apply the first view and init the shader matrix's.
	if (Camera)
		Camera->ApplyView(mainwidth, mainheight);
	//
	if (!MainShader)
	  MainShader = new Shader("MainShader");
	//
	if (MainShader) {
		MainShader->attachShader(BaseShader("Shaders/MainShader.vert"));
		MainShader->attachShader(BaseShader("Shaders/MainShader.frag"));
		// linking shader.
		MainShader->linkPrograms();
	}
	aLineManager = new LineDebugManager(Camera);
	aLineManager->InitBufferGL();
}

void WindowMain::MainRender()
{
	//
	// prepare the main rendering loop.
	// Reset again the color just in case it change in the render state later.
	glClearColor(contextcolor.r, contextcolor.g, contextcolor.b, contextcolor.a);
	// Clear buffer before main rendering.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Default camera view right hand.
	if (Camera) {
		Camera->ApplyView(mainwidth, mainheight);
		//
		// This is the default scene shader.
		if (MainShader) {
			//
			if (aManager) {
				dFloat timestep = dGetElapsedSeconds();
				if (aMousePick) {
					aMousePick->MouseMove(mouvemovex, mouvemovey);
				}
				//
				aManager->UpdateNewton(timestep);
				//
				aManager->CalculateFPS(timestep);
				//
				MainShader->use();
				MainShader->setMat4("mat_projection", Camera->GetMatrixProjection());
				MainShader->setMat4("mat_view", Camera->GetMatrixView());
				MainShader->setVec4("diffuselight", glm::vec4(1.25f, 1.25f, 1.25f, 1.0f));
				//
				if (delayerfps >= 1000) {
					std::stringstream ss;
					ss  << "left foot distance from the Ground : " << FootPos_L << " - Walking Humanoid - " << " [" << aManager->GetFps() << " :fps "<< aManager->GetPhysicTime() * 1000.0f <<" :ms]";
					glfwSetWindowTitle(contextGL, ss.str().c_str());
					delayerfps = 0;
				}
				delayerfps++;
				aManager->Render(MainShader, timestep);
			    if (aLineManager) {
				aLineManager->RenderLine(timestep);
			    }			
			}

			//
		}
	}
}

WindowMain::~WindowMain()
{
  if (aManager) {
	  aManager->IsTerminated = true;
  }
  if (aMousePick) {
	aMousePick->UnPickBody();
	delete aMousePick;
  }
  //
  if (MainShader)
	delete MainShader;
  //
  if (Camera)
	  delete Camera;
  //

  if (aManager) {
	  delete aManager;
  }
  //
  if (contextGL) {
	  glfwSetKeyCallback(contextGL, NULL);
	  glfwSetFramebufferSizeCallback(contextGL, NULL);
	  glfwSetMouseButtonCallback(contextGL, NULL);
	  glfwSetCursorPosCallback(contextGL, NULL);
	  //
	  glfwDestroyWindow((GLFWwindow*)contextGL);
	  contextGL = NULL;
  }
  //
  glfwMakeContextCurrent(NULL);
  //
  // Close OpenGL window and terminate GLFW
  glfwTerminate();
  glfwSetErrorCallback(NULL);
  //
  if (aLineManager) {
	  delete aLineManager;
  }
}