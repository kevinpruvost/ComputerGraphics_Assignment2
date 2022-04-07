/*
 * GLSL Vertex Shader code for OpenGL version 3.3
 */

#version 330 core

// input vertex attributes
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
layout (std140) uniform ViewProj
{
	mat4 viewProj;
};

void main()
{
	gl_Position = viewProj * model * vec4(aPos, 1.0f);
}
