/*****************************************************************//**
 * \file   Mesh_Obj.cpp
 * \brief  Mesh_Obj Source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#include "Mesh_Obj.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

Mesh_Obj::Mesh_Obj(const Obj & obj)
{
	LOG_PRINT(Log::LogMainFileName, "Constructed\n");

	glGenVertexArrays(2, &__verticesVAO);
	glGenBuffers(2, &__verticesVBO);

	// bind VAO and VBO for drawing vertices
	bindVertices(obj);
	// bind VAO and VBO for drawing faces
	bindFaces(obj);
}

Mesh_Obj::~Mesh_Obj()
{
	LOG_PRINT(Log::LogMainFileName, "Destroyed\n");

	glDeleteVertexArrays(2, &__verticesVAO);
	glDeleteBuffers(2, &__verticesVBO);
}

void Mesh_Obj::bindFaces(const Obj & obj)
{
	// bind VAO
	glBindVertexArray(__facesVAO);

	// bind VBO, buffer data to it
	glBindBuffer(GL_ARRAY_BUFFER, __facesVBO);

	std::vector<GLfloat> data;
	for (const auto & face : obj.faces)
	{
		for (int i = 0; i < 3; i++)
		{
			int vid = face[i] - 1;
			data.emplace_back(obj.vertices[vid].x);
			data.emplace_back(obj.vertices[vid].y);
			data.emplace_back(obj.vertices[vid].z);
			data.emplace_back(face.c[0]);
			data.emplace_back(face.c[1]);
			data.emplace_back(face.c[2]);
		}
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * data.size(), &data.front(), GL_STATIC_DRAW);

	// set vertex attribute pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid *)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// unbind VBO & VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	__facesNVert = GLsizei(data.size() / 6);
}

void Mesh_Obj::bindVertices(const Obj & obj)
{
	// bind VAO
	glBindVertexArray(__verticesVAO);

	// bind VBO, buffer data to it
	glBindBuffer(GL_ARRAY_BUFFER, __verticesVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * obj.numVertices(), &obj.vertices.front(), GL_STATIC_DRAW);

	// set vertex attribute pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// unbind VBO & VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	__verticesNVert = GLsizei(obj.numVertices());
}
