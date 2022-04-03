/*****************************************************************//**
 * \file   Mesh_Base.hpp
 * \brief  Mesh_Base class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 03 2022
 *********************************************************************/
#pragma once

// GLAD includes
#include <GLAD\glad.h>

/**
 * @brief Contains the real data of the mesh to be
 * stored in a static database.
*/
class Mesh_Base
{
protected:
    Mesh_Base();
public:
    virtual ~Mesh_Base();

    GLuint GetVerticesVAO();
    GLuint GetFacesVAO();
    GLuint GetVerticesVBO();
    GLuint GetFacesVBO();
    GLuint GetVerticesCount();
    GLuint GetFacesVerticesCount();

    virtual GLuint GetFacesEBO() = 0;
    virtual bool IsUsingEBO() = 0;
private:

protected:
    GLuint __verticesVAO, __facesVAO;
    GLuint __verticesVBO, __facesVBO;
    GLuint __verticesNVert, __facesNVert;
};
