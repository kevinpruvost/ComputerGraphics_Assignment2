/*****************************************************************//**
 * \file   Mesh.cpp
 * \brief  Mesh class source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#include "Mesh.hpp"

std::vector<Mesh_Base> meshesDB;

Mesh_Base::Mesh_Base(const Obj & obj)
{
	glGenVertexArrays(2, &verticesVAO);
	glGenBuffers(2, &verticesVBO);

	// bind VAO and VBO for drawing vertices
	bindVertices(obj);
	// bind VAO and VBO for drawing faces
	bindFaces(obj);
}

Mesh_Base::~Mesh_Base()
{
	// Properly de-allocate all resources once they've outlived their purpose
	glDeleteVertexArrays(2, &verticesVAO);
	glDeleteBuffers(2, &verticesVBO);
}

void Mesh_Base::bindFaces(const Obj & obj)
{
	// bind VAO
	glBindVertexArray(facesVAO);

	// bind VBO, buffer data to it
	glBindBuffer(GL_ARRAY_BUFFER, facesVBO);

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

	facesNVert = GLsizei(data.size() / 6);
}

void Mesh_Base::bindVertices(const Obj & obj)
{
	// bind VAO
	glBindVertexArray(verticesVAO);

	// bind VBO, buffer data to it
	glBindBuffer(GL_ARRAY_BUFFER, verticesVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * obj.numVertices(), &obj.vertices.front(), GL_STATIC_DRAW);

	// set vertex attribute pointers
	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glEnableVertexAttribArray(0);

	// unbind VBO & VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	verticesNVert = GLsizei(obj.numVertices());
}

Mesh::Mesh(const uint16_t meshId)
	: __meshId(meshId)
{
}

GLuint Mesh::meshId() const { return __meshId; }
GLuint Mesh::verticesVAO() const { return meshesDB[__meshId].verticesVAO; }
GLuint Mesh::facesVAO() const { return meshesDB[__meshId].facesVAO; }
GLuint Mesh::verticesVBO() const { return meshesDB[__meshId].verticesVBO; }
GLuint Mesh::facesVBO() const { return meshesDB[__meshId].facesVBO; }
GLuint Mesh::verticesNVert() const { return meshesDB[__meshId].verticesNVert; }
GLuint Mesh::facesNVert() const { return meshesDB[__meshId].facesNVert; }

Mesh GenerateMesh(const Obj & obj)
{
	meshesDB.emplace_back(obj);
	return Mesh(meshesDB.size() - 1);
}

Mesh GenerateMesh(const Mesh & mesh)
{
	return Mesh(mesh);
}

Mesh GenerateMesh(const uint16_t meshId)
{
	return Mesh(meshId);
}
