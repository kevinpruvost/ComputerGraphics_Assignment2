/*****************************************************************//**
 * \file   Mesh_Sphere.cpp
 * \brief  Mesh_Sphere Source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#include "Mesh_Sphere.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

Mesh_Sphere::~Mesh_Sphere()
{
    LOG_PRINT(Log::LogMainFileName, "Destroyed\n");
}
