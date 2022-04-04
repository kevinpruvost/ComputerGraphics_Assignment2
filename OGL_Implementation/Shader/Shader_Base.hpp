/*****************************************************************//**
 * \file   Shader_Base.hpp
 * \brief  Shader_Base class (from TA's sample)
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Mesh\Mesh.hpp"

// GLAD includes
#include <glad/glad.h>

// C++ includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

/**
 * @brief Contains the real data of the shader to be
 * stored in a static database.
*/
class Shader_Base
{
public:
	/**
	 * @brief Constructs and generates the shader
	 * @param vertexPath 
	 * @param fragmentPath 
	*/
	Shader_Base(const GLchar * vertexPath, const GLchar * fragmentPath);
	~Shader_Base();

	/**
	 * @brief Uses the current shader
	*/
	void Use() const;

	/**
	 * @brief Returns Shader Program
	 * @return program
	*/
	GLuint Program() const;

	/**
	 * @brief Adds a Global Uniform Buffer Object
	 * @param bindingPoint (location)
	 * @param bindingPointName (name)
	*/
	void AddGlobalUbo(const GLuint bindingPoint, const char * bindingPointName) const;

private:
	/**
	 * @brief Shader program id
	*/
	GLuint __program;
};