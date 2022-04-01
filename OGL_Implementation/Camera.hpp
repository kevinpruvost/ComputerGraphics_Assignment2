/*****************************************************************//**
 * \file   Camera.hpp
 * \brief  Camera class (from TA's sample)
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

/**
 * @brief Defines direction the Camera follows.
*/
enum class Camera_Movement : uint8_t
{
    Forward  = 0,
    Backward = 1,
    Left     = 2,
    Right    = 3,
    Up       = 4,
    Down     = 5
};

/**
 * @brief Simple Camera used for viewing purpose, created Projection & View Matrix and everything else that comes along...
*/
class Camera
{
public:
    /**
     * @brief Constructor with vectors
     * @param position 
     * @param up 
     * @param yaw (y axis)
     * @param pitch (x axis)
    */
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = CameraDefault_Yaw, GLfloat pitch = CameraDefault_Pitch);
    /**
     * @brief Constructor with scalar values
     * @param posX 
     * @param posY 
     * @param posZ 
     * @param upX 
     * @param upY 
     * @param upZ 
     * @param yaw (y axis)
     * @param pitch (x axis)
    */
    Camera(GLfloat posX = 0.0f, GLfloat posY = 0.0f, GLfloat posZ = 0.0f, GLfloat upX = 0.0f, GLfloat upY = 1.0f, GLfloat upZ = 0.0f, GLfloat yaw = CameraDefault_Yaw, GLfloat pitch = CameraDefault_Pitch);

    /**
     * @brief Calculates the view matrix using Eular Angles and the LookAt Matrix
     * @return View Matrix
    */
    glm::mat4 GetViewMatrix();

    /**
     * @brief 
     * @param windowWidth 
     * @param windowHeight 
     * @return Projection Matrix
    */
    glm::mat4 GetProjectionMatrix(int windowWidth, int windowHeight);

    /**
     * @brief Processes input received from any keyboard-like input system.
     * Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
     * @param direction 
     * @param deltaTime 
    */
    void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime);

    /**
     * @brief Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
     * @param xoffset 
     * @param yoffset 
     * @param constrainPitch 
    */
    void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

    /**
     * @brief Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
     * @param yoffset 
    */
    void ProcessMouseScroll(GLfloat yoffset);

private:
    // Calculates the front vector from the Camera's (updated) Eular Angles
    void updateCameraVectors();

public:
    // Camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // Euler Angles
    GLfloat Yaw;
    GLfloat Pitch;
    // Camera options
    GLfloat MovementSpeed;
    GLfloat MouseSensitivity;
    GLfloat Zoom;
    float fov, zNear, zFar;

private:
    // Default camera values
    static constexpr const GLfloat CameraDefault_Yaw = -90.0f;
    static constexpr const GLfloat CameraDefault_Pitch = 0.0f;
    static constexpr const GLfloat CameraDefault_Speed = 3.0f;
    static constexpr const GLfloat CameraDefault_Sensitivity = 0.25f;
    static constexpr const GLfloat CameraDefault_Zoom = 45.0f;
};
