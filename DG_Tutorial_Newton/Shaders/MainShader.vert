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
/* Tutorial how to use pure newton dynamics api + gl by Dave Gravel 2019. */

#version 460 core

layout (location = 0) in vec3 in_vertices;
layout (location = 1) in vec2 in_texuv;
layout (location = 2) in vec3 in_normals;

uniform mat4 mat_projection, mat_view, mat_model;

out vec2 TexCoords;
out vec3 geomnormals;
out vec3 FragPos; 

void main()
{
    geomnormals = mat3(transpose(inverse(mat_model))) * in_normals;
	FragPos = vec3(mat_model * vec4(in_vertices, 1.0));
    mat4 concocted = mat_projection * mat_view * mat_model;
    gl_Position = concocted * vec4(in_vertices, 1.0);
    TexCoords = in_texuv;
}  