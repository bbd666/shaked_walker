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
#ifndef BASESHADER_H
#define BASESHADER_H
#pragma once
#include "pch.h"
#include <string>

struct shaderType {
	shaderType() : type(-1), name("") {}
	shaderType(unsigned int type, std::string name) : type(type), name(name){}
	unsigned int type;
	std::string name;
};

static bool checkCompileErrors(unsigned int shader, std::string type, std::string shaderName);

struct BaseShader
{   // public
	BaseShader(const char * shaderPath);
	virtual ~BaseShader();
	std::string getName() 
	{
		return getShaderName(path.c_str());
	}
	shaderType getType() 
	{
		return shadType;
	}
	unsigned int getShad() 
	{
		return shad;
	}
private:
	std::string loadShaderFromFile(const char* shaderPath);
	std::string path;
    std::string getShaderName(const char* shaderPath);
	shaderType getShaderType(const char* path);
	unsigned int shad;
	shaderType shadType;
};

#endif //BASESHADER_H

