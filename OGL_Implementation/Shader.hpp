/*****************************************************************//**
 * \file   Shader.hpp
 * \brief  Shader class (from TA's sample)
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#pragma once

// C++ includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

// Glad includes
#include <glad/glad.h>

/**
 * @brief Contains information & utilities about shaders.
*/
class Shader
{
public:
	/**
	 * @brief Constructs and generates the shader
	 * @param vertexPath 
	 * @param fragmentPath 
	*/
	Shader(const GLchar * vertexPath, const GLchar * fragmentPath);
	/**
	 * @brief Uses the current shader
	*/
	void Use();

public:
	/**
	 * @brief Shader program id
	*/
	GLuint program;
};