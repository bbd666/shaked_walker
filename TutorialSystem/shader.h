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
#ifndef SHADERS_H
#define SHADERS_H
#pragma once
#include "pch.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <list>

#include "BaseShader.h"

struct Shader
{
	// public
	Shader(std::string name);
	Shader(std::string name, const char * computeShader);
	Shader * attachShader(BaseShader s);
	GLuint ID;
	void linkPrograms();

	virtual ~Shader();
	void use();
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	void setVec2(const std::string &name, glm::vec2 vector) const;
	void setVec3(const std::string &name, glm::vec3 vector) const;
	void setVec4(const std::string &name, glm::vec4 vector) const;
	void setNVec4(const std::string &name, dVector vector) const;
	void setMat4(const std::string &name, glm::mat4 matrix) const;
	void setNMat4(const std::string &name, dMatrix matrix) const;
	void setSampler2D(const std::string &name, unsigned int texture, int id) const;
	void setSampler3D(const std::string &name, unsigned int texture, int id) const;

protected:

	bool linked, isCompute;
	std::list<GLuint> shaders;
	std::string name;
	//std::string getShaderName(const char* path);
	shaderType getShaderType(const char* path);
};

#endif

