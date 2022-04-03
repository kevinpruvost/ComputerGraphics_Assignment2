/*****************************************************************//**
 * \file   Mesh_Base.cpp
 * \brief  Mesh_Base Source Code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#include "Mesh_Base.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

Mesh_Base::Mesh_Base()
{
	Log::Print(Log::LogMainFileName, "Mesh_Base constructor\n");
}

Mesh_Base::~Mesh_Base()
{
	Log::Print(Log::LogMainFileName, "Mesh_Base destructor\n");
}

GLuint Mesh_Base::GetVerticesVAO()
{
	return __verticesVAO;
}

GLuint Mesh_Base::GetFacesVAO()
{
	return __facesVAO;
}

GLuint Mesh_Base::GetVerticesVBO()
{
	return __verticesVBO;
}

GLuint Mesh_Base::GetFacesVBO()
{
	return __facesVBO;
}

GLuint Mesh_Base::GetVerticesCount()
{
	return __verticesNVert;
}

GLuint Mesh_Base::GetFacesVerticesCount()
{
	return __facesNVert;
}