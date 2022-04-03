/*****************************************************************//**
 * \file   Obj.hpp
 * \brief  .obj files loading class/parser
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#pragma once

// C++ includes
#include <exception>
#include <iostream>
#include <sstream>
#include <vector>
#include <array>

// Glad includes
#include <glad/glad.h>

#include "Mesh/Geometry.hpp"

/**
 * @brief Manages parsing and loading of .obj files.
*/
class Obj
{
public:
	Obj();

	std::vector<Vertex> vertices;
	std::vector<TriFace> faces;

	/**
	 * @brief Returns Count of triangle faces.
	 * @return count of triangle faces
	*/
	int numTriangles() const { return int(faces.size()); }
	/**
	 * @brief Returns Count of vertices.
	 * @return count of vertices
	*/
	int numVertices() const { return int(vertices.size()); }

	/**
	 * @brief Tries loading of a .obj file
	 * @param fileName
	 * @return true if everything happened without errors, false otherwise
	*/
	bool TryLoad(const char * fileName);
};