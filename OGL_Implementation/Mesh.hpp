/*****************************************************************//**
 * \file   Mesh.hpp
 * \brief  Mesh class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#pragma once

// C++ includes
#include <memory>

// Project includes
#include "Obj.hpp"

/**
 * @brief Contains the real data of the mesh to be
 * stored in a static database.
*/
class Mesh_Base
{
public:
    /**
     * @brief Constructor from .obj class
     * @param obj 
    */
    Mesh_Base(const Obj & obj);
    ~Mesh_Base();

private:
    void bindFaces(const Obj & obj);
    void bindVertices(const Obj & obj);

public:
    GLuint verticesVAO, facesVAO;
    GLuint verticesVBO, facesVBO;
    GLuint verticesNVert, facesNVert;
};

/**
 * @brief Contains Mesh Id and methods related
 * to the mesh database.
 * It also helps having a much lighter class because Mesh_Base weighs 24 bytes
 * and Mesh only 2 bytes.
*/
class Mesh
{
public:
    /**
     * @brief Constructor from Mesh ID.
     * @param meshId 
    */
    Mesh(const uint16_t meshId);

public:
    GLuint meshId() const;

    GLuint verticesVAO() const;
    GLuint facesVAO() const;
    GLuint verticesVBO() const;
    GLuint facesVBO() const;
    GLuint verticesNVert() const;
    GLuint facesNVert() const;

private:
    const uint16_t __meshId;
};

/**
 * @brief Generates mesh from obj class, stores it in the mesh database
 * and returns a Mesh class.
 * @param obj 
 * @return mesh
*/
Mesh GenerateMesh(const Obj & obj);
/**
 * @brief Generates mesh from mesh
 * @param mesh
 * @return mesh
*/
Mesh GenerateMesh(const Mesh & mesh);
/**
 * @brief Generates mesh from mesh id
 * @param meshId 
 * @return mesh
*/
Mesh GenerateMesh(const uint16_t meshId);