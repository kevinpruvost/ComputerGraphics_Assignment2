/*****************************************************************//**
 * \file   Entity.hpp
 * \brief  Entity class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 28 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Entity_Skeleton.hpp"

#include "OGL_Implementation\Mesh\Mesh.hpp"
#include "OGL_Implementation\Shader\Shader.hpp"
#include "OGL_Implementation\Texture.hpp"

// GLM includes
#include <glm\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\quaternion.hpp>

// C++ includes
#include <memory>

/**
 * @brief Representation of an entity with a mesh,
 * position, rotation, scale.
 * Able to give model matrix.
*/
class Entity : public Entity_Skeleton
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
        const Shader & pointShader,
        const Shader & wireframeShader,
        const Shader & faceShader,
        const glm::vec3 & defaultPosition = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 & defaultEulerAngles = glm::vec3(0.0f, 0.0f, 0.0f),
        const glm::vec3 & defaultScale = glm::vec3(1.0f, 1.0f, 1.0f));

    /**
     * @brief Constructor with default point/wireframe/face shader already set
     * /!\ I repeat, do not forget to set the appropriate default shaders
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

    void SetPointShader(const Shader & shader);
    void SetWireframeShader(const Shader & shader);
    void SetFaceShader(const Shader & shader);
    Shader GetPointShader() const;
    Shader GetWireframeShader() const;
    Shader GetFaceShader() const;

    void SetTexture(const Texture & texture);
    const Texture & GetTexture() const;

    const Mesh & GetMesh() const;
    /**
     * @brief Calculates Model Matrix.
     * @return Model Matrix
    */
    virtual glm::mat4 GetModelMatrix() const;

    // Entity_Skeleton abstract
    virtual glm::vec3 Get3DPosition() const;

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
    /**
     * @brief Shader Identifier.
    */
    Shader __shaderPoint, __shaderWireframe, __shaderFace;
    Texture __texture;
};

void SetDefaultPointShader(const Shader & shader);
void SetDefaultWireframeShader(const Shader & shader);
void SetDefaultFaceShader(const Shader & shader);
