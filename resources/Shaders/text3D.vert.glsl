/*
 * GLSL Vertex Shader code for OpenGL version 3.3
 */

#version 330 core

// input vertex attributes
layout (location = 0) in vec4 vertex;

layout (std140) uniform ViewProj
{
	mat4 viewProj;
};

out vec2 TexCoords;

void main()
{
	gl_Position = viewProj * vec4(vertex.xy, 0.0f, 1.0f);
	TexCoords = vec2(vertex.z, 1.0f - vertex.w);
}
