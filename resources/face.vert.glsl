/*
 * GLSL Vertex Shader code for OpenGL version 3.3
 */

#version 330 core

// input vertex attributes
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 aColor;

uniform mat4 model;
layout (std140) uniform ViewProj
{
	mat4 viewProj;
};

out vec3 ourColor;
out vec3 randColor;


void main()
{
	gl_Position = viewProj * model * vec4(position, 1.0f);
	randColor = aColor;
}
