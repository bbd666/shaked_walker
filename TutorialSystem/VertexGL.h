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
#ifndef VERTEXGL_H
#define VERTEXGL_H
#pragma once

#include "pch.h"

struct MainVertexPTN
{ // Used in a similar way to record from pascal
  glm::vec3 posit;
  glm::vec2 uv;
  glm::vec3 normal;
};

struct MainVertice
{
	float vx;
	float vy;
	float vz;
};

struct MainNormal
{
	float nx;
	float ny;
	float nz;
};

struct MainTexUv
{
	float uvx;
	float uvy;
};

struct MainVertexPC
{ // Used in a similar way to record from pascal
	glm::vec3 posit;
	glm::vec3 color;
};



#endif //VERTEXGL_H
