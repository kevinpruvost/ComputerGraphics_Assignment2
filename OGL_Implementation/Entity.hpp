/*****************************************************************//**
 * \file   Entity.hpp
 * \brief  Entity class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 28 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Mesh.hpp"
#include "Shader.hpp"

// GLM includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\quaternion.hpp>

/**
 * @brief Representation of an entity with a mesh,
 * position, rotation, scale.
 * Able to give model matrix.
*/
class Entity
{
public:
    /**
     * @brief Default Constructor
     * @param mesh 
     * @param defaultPosition 
     * @param defaultEulerAngles 
     * @param defaultScale 
    */
    Entity(const Mesh & mesh,
        const glm::vec3 & defaultPosition = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 & defaultEulerAngles = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 & defaultScale = glm::vec3(1.0f, 1.0f, 1.0f));
    ~Entity();

    const Mesh & getMesh() const;
    /**
     * @brief Calculates Model Matrix.
     * @return Model Matrix
    */
    glm::mat4 getModelMatrix() const;

public:
    /**
     * @brief Transformation Vector.
    */
    glm::vec3 pos;
    /**
     * @brief Scaling Vector.
    */
    glm::vec3 scale;
    /**
     * @brief Rotation vector (expressed with euler angles).
    */
    glm::vec3 eulerAngles;
private:
    /**
     * @brief Mesh Identifier.
    */
    const Mesh __mesh;
};
