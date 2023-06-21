/*
* Copyright(c) < 1999 - 2020 > <Dave Gravel, Free Tutorial - Newton Dynamics and more.>
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
/* Tutorial how to use pure newton dynamics api + gl by Dave Gravel 2020. */
#version 460 core

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_color;

uniform mat4 Projection_Matrix, View_Matrix, Model_Matrix;

out VS_FS_INTERFACE
{
  vec3 vcolor;
} vertex;

void main()
{
  //gl_LinesSize = 5.0;
  //
  vertex.vcolor = in_color;
  //
  gl_Position = Projection_Matrix * View_Matrix * Model_Matrix * vec4(in_position, 1);
};