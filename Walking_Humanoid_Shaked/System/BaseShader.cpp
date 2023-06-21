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
#include "BaseShader.h"
#include <iostream>
#include <fstream>
#include <sstream>

BaseShader::BaseShader(const char * shaderPath)
{
	path = std::string(shaderPath);
	std::string shaderCode = loadShaderFromFile(shaderPath);
	const char * shaderString = shaderCode.c_str();

	shadType = getShaderType(shaderPath);
	shad = glCreateShader(shadType.type);
	glShaderSource(shad, 1, &shaderString, NULL);
	glCompileShader(shad);
	checkCompileErrors(shad, shadType.name.c_str(), getShaderName(shaderPath));
}

static bool checkCompileErrors(unsigned int shader, std::string type, std::string shaderName)
{
	int success;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR: SHADER" << shaderName << "COMPILATION ERROR of type: " << type 
			<< "\n" << infoLog << "\n == ===================== [SHADER ERROR] ===================== == " << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type 
			<< "\n" << infoLog << "\n == ===================== [SHADER ERROR] ===================== == " << std::endl;
		}
	}

	return success;
}

std::string BaseShader::loadShaderFromFile(const char* shaderPath) {
	std::string shaderCode;
	std::ifstream shaderFile;
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		shaderFile.open(shaderPath);
		std::stringstream shaderStream;
		// read file's buffer contents into streams
		shaderStream << shaderFile.rdbuf();
		// close file handlers
		shaderFile.close();
		// convert stream into string
		shaderCode = shaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER " << getShaderName(shaderPath) << " FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	return shaderCode;

}

std::string BaseShader::getShaderName(const char* path) {
	std::string pathstr = std::string(path);
	const size_t last_slash_idx = pathstr.find_last_of("/");
	if (std::string::npos != last_slash_idx)
	{
		pathstr.erase(0, last_slash_idx + 1);
	}
	return pathstr;
}

shaderType BaseShader::getShaderType(const char* path) {
	std::string type = getShaderName(path);
	const size_t last_slash_idx = type.find_last_of(".");
	if (std::string::npos != last_slash_idx)
	{
		type.erase(0, last_slash_idx + 1);
	}
	if (type == "vert")
		return shaderType(GL_VERTEX_SHADER, "VERTEX");
	if (type == "frag")
		return shaderType(GL_FRAGMENT_SHADER, "FRAGMENT");
	if (type == "tes")
		return shaderType(GL_TESS_EVALUATION_SHADER, "TESS_EVALUATION");
	if (type == "tcs")
		return shaderType(GL_TESS_CONTROL_SHADER, "TESS_CONTROL");
	if (type == "geom")
		return shaderType(GL_GEOMETRY_SHADER, "GEOMETRY");
	if (type == "comp")
		return shaderType(GL_COMPUTE_SHADER, "COMPUTE");
    // wrong shader...
	return shaderType();
}


BaseShader::~BaseShader()
{
	//glDeleteShader(shad);
}
