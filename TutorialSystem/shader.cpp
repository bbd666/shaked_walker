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
#include "shader.h"

bool checkCompileErrors(GLuint shader, std::string type, std::string shaderName)
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

Shader::Shader(std::string name) 
: name(name)
{
	linked = false;
	isCompute = false;
	ID = glCreateProgram();
}

std::string getShaderName(const char* path) 
{
	std::string pathstr = std::string(path);
	const size_t last_slash_idx = pathstr.find_last_of("/");
	if (std::string::npos != last_slash_idx)
	{
		pathstr.erase(0, last_slash_idx + 1);
	}
	return pathstr;
}

shaderType Shader::getShaderType(const char* path) 
{
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
	//
	// wrong shader
	return shaderType();
}

Shader::~Shader() 
{
	glDeleteProgram(ID);
}

Shader::Shader(std::string name, const char * computeShaderPath) 
: name(name)
{
	linked = false;
	isCompute = false;
	ID = glCreateProgram();

	this->attachShader(BaseShader(computeShaderPath));
	this->linkPrograms();
}


Shader * Shader::attachShader(BaseShader s)
{
	if (!isCompute) 
	{
		glAttachShader(ID, s.getShad());
		if (s.getName() == "COMPUTE")
			isCompute = true;
		this->shaders.push_back(s.getShad());
	}
	else {
		std::cout << "ERROR: TRYING TO LINK A NON COMPUTE SHADER TO COMPUTE PROGRAM" << std::endl;
	}

	return this;
}

void Shader::linkPrograms()
{
	glLinkProgram(ID);

	if (checkCompileErrors(ID, "PROGRAM", "")) 
	{
		linked = true;
		std::cout << "PROGRAM " << name << " CORRECTLY LINKED" << std::endl;
		while (!shaders.empty()) {
			glDeleteShader(shaders.back());
			shaders.pop_back();
		}
	}
	else {
		std::cout << "ERROR WHILE LINKING TO " << name << " PROGRAM" << std::endl;
	}
}


void Shader::use()
{
	if (linked)
		glUseProgram(ID);
	else {
		std::cout << "PROGRAMS NOT LINKED!" << std::endl;
	}
}

void Shader::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setVec2(const std::string &name, glm::vec2 vector) const
{
	unsigned int location = glGetUniformLocation(ID, name.c_str());
	glUniform2fv(location, 1, glm::value_ptr(vector));
}
void Shader::setVec3(const std::string &name, glm::vec3 vector) const
{
	unsigned int location = glGetUniformLocation(ID, name.c_str());
	glUniform3fv(location, 1, glm::value_ptr(vector));
}

void Shader::setVec4(const std::string &name, glm::vec4 vector) const
{
	unsigned int location = glGetUniformLocation(ID, name.c_str());
	glUniform4fv(location, 1, glm::value_ptr(vector));
}

void Shader::setNVec4(const std::string &name, dVector vector) const
{
	unsigned int location = glGetUniformLocation(ID, name.c_str());
	glUniform4fv(location, 1, &vector.m_x);
}

void Shader::setMat4(const std::string &name, glm::mat4 matrix) const
{
	unsigned int mat = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(mat, 1, false, glm::value_ptr(matrix));
}

void Shader::setNMat4(const std::string &name, dMatrix matrix) const
{
	unsigned int mat = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(mat, 1, false, &matrix[0][0]);
}

void Shader::setSampler2D(const std::string &name, unsigned int texture, int id) const
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, texture);
	this->setInt(name, id);
}
void Shader::setSampler3D(const std::string &name, unsigned int texture, int id) const
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_3D, texture);
	this->setInt(name, id);
}
