/*****************************************************************//**
 * \file   Entity.cpp
 * \brief  Entity class source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 28 2022
 *********************************************************************/
#include "Entity.hpp"

Entity::Entity(const Mesh & mesh,
    const glm::vec3 & defaultPosition,
    const glm::vec3 & defaultEulerAngles,
    const glm::vec3 & defaultScale)
    : __mesh{ mesh }
    , pos{ defaultPosition }
    , scale{ defaultScale }
    , eulerAngles{ defaultEulerAngles }
{
}

Entity::~Entity()
{
}

const Mesh & Entity::getMesh() const { return __mesh; }

glm::mat4 Entity::getModelMatrix() const
{
    // Transformation Matrix
    glm::mat4 mat = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, 0.0f},
        {pos.x, pos.y, pos.z, 1.0f}
    };
    // Rotation Matrix
    mat *= glm::toMat4(glm::quat(glm::radians(eulerAngles)));
    // Scaling Matrix
    mat = glm::scale(mat, scale);
    return mat;
}
