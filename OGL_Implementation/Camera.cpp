/*****************************************************************//**
 * \file   Camera.cpp
 * \brief  Source code of Camera class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : Front{ glm::vec3(0.0f, 0.0f, -1.0f) }
    , MovementSpeed{ CameraDefault_Speed }
    , MouseSensitivity{ CameraDefault_Sensitivity }
    , Zoom{ CameraDefault_Zoom }
    , Position{ position }
    , WorldUp{ up }
    , Yaw{ yaw }
    , Pitch{ pitch }
    , fov { 45.0f }
    , zNear { 0.1f }
    , zFar { 100.0f }
{
    updateCameraVectors();
}

Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch)
    : Front{ glm::vec3(0.0f, 0.0f, -1.0f) }
    , MovementSpeed{ CameraDefault_Speed }
    , MouseSensitivity{ CameraDefault_Sensitivity }
    , Zoom{ CameraDefault_Zoom }
    , Position{ glm::vec3(posX, posY, posZ) }
    , WorldUp{ glm::vec3(upX, upY, upZ) }
    , Yaw{ yaw }
    , Pitch{ pitch }
    , fov{ 45.0f }
    , zNear{ 0.1f }
    , zFar{ 100.0f }
{
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}

glm::mat4 Camera::GetProjectionMatrix(int windowWidth, int windowHeight)
{
    return glm::perspective(
        glm::radians(fov), // FOV
        (GLfloat)windowWidth / (GLfloat)windowHeight, // Aspect Ratio
        zNear,  // zNear
        zFar // zFar
    );
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    GLfloat velocity = this->MovementSpeed * deltaTime;
    switch (direction)
    {
        case Camera_Movement::Forward:
            this->Position += this->Front * velocity;
            break;
        case Camera_Movement::Backward:
            this->Position -= this->Front * velocity;
            break;
        case Camera_Movement::Left:
            this->Position -= this->Right * velocity;
            break;
        case Camera_Movement::Right:
            this->Position += this->Right * velocity;
            break;
        case Camera_Movement::Up:
            this->Position += this->Up * velocity;
            break;
        case Camera_Movement::Down:
            this->Position -= this->Up * velocity;
            break;
    }
}

void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw += xoffset;
    this->Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
        this->Zoom -= yoffset;
    if (this->Zoom <= 1.0f)
        this->Zoom = 1.0f;
    if (this->Zoom >= 45.0f)
        this->Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up = glm::normalize(glm::cross(this->Right, this->Front));
}
