/*
 * GLSL Fragment Shader code for OpenGL version 3.3
 */

#version 330 core

in vec3 randColor;

out vec4 color;

void main()
{
	color = vec4(randColor, 1.0f);
}
