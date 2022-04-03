/*****************************************************************//**
 * \file   Mesh_Obj.hpp
 * \brief  Mesh created from Obj class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Mesh_Base.hpp"
#include "OGL_Implementation\Obj.hpp"

class Mesh_Obj : public Mesh_Base
{
public:
    /**
     * @brief Constructor from .obj class
     * @param obj
    */
    Mesh_Obj(const Obj & obj);
    ~Mesh_Obj();

private:
    void bindFaces(const Obj & obj);
    void bindVertices(const Obj & obj);
};
