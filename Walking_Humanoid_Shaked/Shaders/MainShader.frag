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

in vec2 TexCoords;
in vec3 geomnormals;
in vec3 FragPos; 

out vec4 color;

uniform sampler2D aSamplerTex;
uniform vec4 diffuselight;
uniform vec4 diffusecol;

void main()
{    
    vec4 sampled = vec4(texture(aSamplerTex, TexCoords).xyz, 1.0);
    vec3 norm = normalize(geomnormals);
	// Very basic light setup for the tutorial.
    vec3 lightDir = normalize(vec3(-5, 25, -15) - FragPos); 
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * diffuselight.xyz;	
    color = sampled * diffusecol * vec4(diffuse, 1.0f);
}  