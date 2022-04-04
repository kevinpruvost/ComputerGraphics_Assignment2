/*****************************************************************//**
 * \file   Geometry.hpp
 * \brief  Geometry related functions & classes
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#pragma once

// C++ includes
#include <cmath>

// GLM includes
#include <glm\glm.hpp>

const double Pi = acos(-1.0f);

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
		: v{ fd[0], fd[1], fd[2] }
		, c{ cd[0], cd[1], cd[2] }
	{
	}

	int & operator[](int idx)
	{
		return v[idx];
	}

	const int & operator[](int idx) const
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
	Vertex(GLfloat x_ = 0.0f, GLfloat y_ = 0.0f, GLfloat z_ = 0.0f) : x(x_), y(y_), z(z_) {}
};

/**
 * @brief Contains information about vertices.
*/
struct VertexNormalTexture
{
	VertexNormalTexture(GLfloat x_ = 0.0f, GLfloat y_ = 0.0f, GLfloat z_ = 0.0f,
		GLfloat nx_ = 0.0f, GLfloat ny_ = 0.0f, GLfloat nz_ = 0.0f,
		GLfloat s_ = 0.0f, GLfloat t_ = 0.0f)
		: xyz(x_, y_, z_)
		, normals(nx_, ny_, nz_)
		, textureCoords(s_, t_)
	{}
	union
	{
		struct { GLfloat x, y, z; };
		glm::fvec3 xyz;
	};
	union
	{
		struct { GLfloat nx, ny, nz; };
		glm::fvec3 normals;
	};
	union
	{
		struct { GLfloat s, t; };
		glm::fvec2 textureCoords;
	};
};