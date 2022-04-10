/*****************************************************************//**
 * \file   Quaternion.hpp
 * \brief  Quaternion class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 10 2022
 *********************************************************************/
#pragma once

// GLM includes
#include <GLM\glm.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtx\euler_angles.hpp>
#include <glm\gtx\quaternion.hpp>

class Quaternion : public glm::quat
{
public:
    /**
     * @brief Quaternion constructor with rotation in degrees
     * @param rotation 
    */
    Quaternion(const glm::vec3 & rotation = { 0.0f, 0.0f, 0.0f });
    ~Quaternion();
    Quaternion & operator=(const glm::quat & quat);
    operator glm::quat & ();
    operator const glm::quat & () const;

    void Rotate(const glm::vec3 & rotation);
    void RotateX(const float rotation);
    void RotateY(const float rotation);
    void RotateZ(const float rotation);

public:
};
