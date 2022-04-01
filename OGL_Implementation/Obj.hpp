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

/**
 * @brief Contains information about Tri Faces.
*/
struct TriFace
{
	int v[3];
	float c[3];

	/**
	 * @brief Split array constructor
	 * @param f0 
	 * @param f1 
	 * @param f2 
	 * @param c0 
	 * @param c1 
	 * @param c2 
	*/
	TriFace(int f0, int f1, int f2, float c0, float c1, float c2)
		: v{ f0, f1, f2 }
		, c{ c0, c1, c2 }
	{
	}

	/** Array constructor
	 * @brief 
	 * @param fd 
	 * @param cd 
	*/
	TriFace(int fd[3], float cd[3])
		: v{fd[0], fd[1], fd[2]}
		, c{cd[0], cd[1], cd[2]}
	{
	}

	int& operator[](int idx)
	{
		return v[idx];
	}

	const int& operator[](int idx) const
	{
		return v[idx];
	}
};

/**
 * @brief Contains information about vertices.
*/
struct Vertex
{
	GLfloat x, y, z;
	Vertex(GLfloat x_, GLfloat y_, GLfloat z_) : x(x_), y(y_), z(z_) {}
};

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