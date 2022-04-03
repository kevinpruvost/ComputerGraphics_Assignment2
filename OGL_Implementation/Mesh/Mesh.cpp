/*****************************************************************//**
 * \file   Mesh.cpp
 * \brief  Mesh class source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#include "Mesh.hpp"

static std::vector<std::unique_ptr<Mesh_Base>> meshesDB;

Mesh::Mesh(const uint16_t meshId)
	: __meshId(meshId)
{
}

GLuint Mesh::meshId() const { return __meshId; }
GLuint Mesh::verticesVAO() const { return meshesDB[__meshId]->GetVerticesVAO(); }
GLuint Mesh::facesVAO() const { return meshesDB[__meshId]->GetFacesVAO(); }
GLuint Mesh::verticesVBO() const { return meshesDB[__meshId]->GetVerticesVBO(); }
GLuint Mesh::facesVBO() const { return meshesDB[__meshId]->GetFacesVBO(); }
GLuint Mesh::verticesNVert() const { return meshesDB[__meshId]->GetVerticesCount(); }
GLuint Mesh::facesNVert() const { return meshesDB[__meshId]->GetFacesVerticesCount(); }

Mesh GenerateMeshSphere(float radius, int sectors, int stacks, bool smooth)
{
	meshesDB.emplace_back(new Mesh_Sphere(radius, sectors, stacks, smooth));
	return Mesh(meshesDB.size() - 1);
}

Mesh GenerateMesh(const Obj & obj)
{
	meshesDB.emplace_back(new Mesh_Obj(obj));
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
