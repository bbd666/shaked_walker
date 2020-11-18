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
#ifndef PCH_H
#define PCH_H

#if defined(_DEBUG)
#pragma comment ( linker, "/subsystem:console")
#else
#pragma comment ( linker, "/subsystem:windows")
#endif

#ifdef _MSC_VER
#pragma warning (disable: 4100) //unreferenced formal parameter
#pragma warning (disable: 4505) //unreferenced local function has been removed
#pragma warning (disable: 4201) //nonstandard extension used : nameless struct/union
#pragma warning (disable: 4127) //conditional expression is constant
#pragma warning (disable: 4456) //declaration of 'bone' hides previous local declaration

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#if (_MSC_VER >= 1400)
#pragma warning (disable: 4996) // for 2005 users declared deprecated
#endif
#endif

#define DOMMY_API 

typedef char dInt8;
typedef unsigned char dUnsigned8;

typedef short dInt16;
typedef unsigned short dUnsigned16;

typedef int dInt32;
typedef unsigned dUnsigned32;
typedef unsigned int dUnsigned32;


typedef long long unsigned64;

#include "windows.h"
#define _USE_MATH_DEFINES
#include <cmath> 
#include <tchar.h>
#include <math.h>
#include <ctime>
#include <stdio.h>
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h> 
#include <string>
#include <iostream>
#include <set>
#include <string>
#include <iterator>
#include <fstream>
#include <list>		
#include <mmintrin.h>
#include <vector>
#include <unordered_map>
#include <map>
#include <algorithm>


/*
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
#else
#define DBG_NEW new
#endif
*/

#ifdef _MSC_VER
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#undef APIENTRY
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <windows.h>
#include <Commctrl.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <time.h>
#include <tchar.h>
#include <crtdbg.h>
#include <iostream>

#define GLM_ENABLE_EXPERIMENTAL
#define GLM_SWIZZLE_XYZW
#define GLM_FORCE_MESSAGES
//#define GLM_FORCE_AVX2
#include <glm\glm.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtx\vector_angle.hpp>
#include <glm\gtx\compatibility.hpp>
#include <glm\gtx\matrix_operation.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\norm.hpp>
#include <glm\gtx\compatibility.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\simd\matrix.h>
#include <glm\simd\vector_relational.h>

#define GLEW_STATIC
#include <glew.h>
#include <wglew.h>
#include <glfw3.h>
#include <glfw3native.h>
#endif

#include <Newton.h>
#include <dMathDefines.h>
#include <dLinearAlgebra.h>
#include <dMatrix.h>
#include <dQuaternion.h>
#include <dVector.h>
#include <dBezierSpline.h>

#include <dCustomJoint.h>
#include <dCustomGear.h>
#include <dCustomHinge.h>
#include <dCustomPlane.h>
#include <dCustomWheel.h>
#include <dCustomMotor.h>
#include <dCustomSixdof.h>
#include <dCustomSlider.h>
#include <dCustomPulley.h>
#include <dCustomCorkScrew.h>
#include <dCustomPathFollow.h>
#include <dCustomDoubleHinge.h>
#include <dCustomFixDistance.h>
#include <dCustomRackAndPinion.h>
#include <dCustomHingeActuator.h>
#include <dCustomBallAndSocket.h>
#include <dCustomSliderActuator.h>
#include <dCustomSlidingContact.h>
#include <dCustomDoubleHingeActuator.h>

#include <dCustomTriggerManager.h>
#include <dCustomKinematicController.h>
#include <dCustomPlayerControllerManager.h>

// 
#include <dModelAnimTree.h>
#include <dModelAnimTreePose.h>
#include <dModelAnimTreePoseBlender.h>
#include <dModelManager.h>
#include <dModelNode.h>
#include <dModelRootNode.h>
#include <dModelStdAfx.h>
//
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/mesh.h>
#include <assimp/texture.h>
#include <assimp/types.h>
#include <assimp/vector2.h>
#include <assimp/vector3.h>
#include <assimp/matrix3x3.h>
#include <assimp/matrix4x4.h>
#include <assimp/quaternion.h>
#include <assimp/postprocess.h>
#include <assimp/Exporter.hpp>

//

#include <thread>        
#include <mutex> 

#endif //PCH_H
