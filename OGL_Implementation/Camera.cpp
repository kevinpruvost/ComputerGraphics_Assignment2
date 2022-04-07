/*****************************************************************//**
 * \file   Camera.cpp
 * \brief  Source code of Camera class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 27 2022
 *********************************************************************/
#include "Camera.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

Camera * mainCamera = nullptr;

Camera::Camera(int windowWidth, int windowHeight,
    GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ,
    GLfloat yaw, GLfloat pitch)
    : Front{ glm::vec3(0.0f, 0.0f, -1.0f) }
    , MovementSpeed{ CameraDefault_Speed }
    , MouseSensitivity{ CameraDefault_Sensitivity }
    , Zoom{ CameraDefault_Zoom }
    , Position{ glm::vec3(posX, posY, posZ) }
    , WorldUp{ glm::vec3(upX, upY, upZ) }
    , Yaw{ yaw }
    , Pitch{ pitch }
    , __fov{ 45.0f }
    , __zNear{ 0.1f }
    , __zFar{ 100.0f }
    , __hasMoved{ true }
    , __hasReshaped{ true }
    , __uboProjView{ 0 }
    , __uboProjection{ 0 }
    , __uboProjAndView{ 0 }
    , __wWidth{ windowWidth }
    , __wHeight{ windowHeight }
{
    updateCameraVectors();
    
    // Allocating UBO ViewProj
    glGenBuffers(1, &__uboProjView);
    glGenBuffers(1, &__uboProjection);
    glGenBuffers(1, &__uboProjAndView);


    glBindBuffer(GL_UNIFORM_BUFFER, __uboProjAndView);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 2, __uboProjAndView, 0, sizeof(glm::mat4) * 2);


    glBindBuffer(GL_UNIFORM_BUFFER, __uboProjView);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBufferRange(GL_UNIFORM_BUFFER, 0, __uboProjView, 0, sizeof(glm::mat4));

    // Binds buffer to a specific binding point so that it'll be used at this exact place
    // by shaders

    glBindBuffer(GL_UNIFORM_BUFFER, __uboProjection);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), NULL, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // Binds buffer to a specific binding point so that it'll be used at this exact place
    // by shaders
    glBindBufferRange(GL_UNIFORM_BUFFER, 1, __uboProjection, 0, sizeof(glm::mat4));
}

Camera::Camera(int windowWidth, int windowHeight, glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : Camera(windowWidth, windowHeight, position.x, position.y, position.z, up.x, up.y, up.z, yaw, pitch)
{
}

Camera::~Camera()
{
    glDeleteBuffers(1, &__uboProjView);
    glDeleteBuffers(1, &__uboProjAndView);
    glDeleteBuffers(1, &__uboProjection);

    if (mainCamera == this) mainCamera = nullptr;
}

glm::mat4 Camera::GetViewMatrix()
{
    return __view;
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return __projection;
}

GLuint Camera::GetProjViewMatrixUbo()
{
    if (__hasMoved || __hasReshaped)
    {
        if (__hasMoved)
        {
            __view = glm::lookAt(this->Position, this->Position + this->Front, this->Up);
            __hasMoved = false;
        }
        if (__hasReshaped)
        {
            __projection = glm::perspective(
                glm::radians(__fov), // FOV
                (GLfloat)__wWidth / (GLfloat)__wHeight, // Aspect Ratio
                __zNear,  // zNear
                __zFar // zFar
            );
            glm::mat4 projection2D = glm::ortho(0.0f, static_cast<GLfloat>(__wWidth), 0.0f, static_cast<GLfloat>(__wHeight));
            __hasReshaped = false;

            // Reassign projection matrix
            glBindBuffer(GL_UNIFORM_BUFFER, __uboProjection);
            glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection2D));
        }
        // Reassign view & proj matrix
        glBindBuffer(GL_UNIFORM_BUFFER, __uboProjAndView);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(__view));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(__projection));

        // Reassign viewProj matrix
        glBindBuffer(GL_UNIFORM_BUFFER, __uboProjView);
        const glm::mat4 viewProj = __projection * __view;
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(viewProj));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
    return __uboProjView;
}

void Camera::ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
{
    __hasMoved = true;
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
    __hasMoved = true;
}

void Camera::Reshape(int windowWidth, int windowHeight, float fov, float zNear, float zFar)
{
    __wWidth = windowWidth;
    __wHeight = windowHeight;
    __fov = fov;
    __zNear = zNear;
    __zFar = zFar;
    __hasReshaped = true;
}

const float & Camera::GetFov() const { return __fov; }
const float & Camera::GetZNear() const { return __zNear; }
const float & Camera::GetZFar() const { return __zFar; }

void Camera::SetFov(float fov)
{
    __fov = fov;
    __hasReshaped = true;
}

void Camera::SetZNearFar(float zNear, float zFar)
{
    __zNear = zNear;
    __zFar = zFar;
    __hasReshaped = true;
}

void Camera::SetWindowDimensions(int windowWidth, int windowHeight)
{
    __wWidth = windowWidth;
    __wHeight = windowHeight;
    __hasReshaped = true;
}

glm::vec2 Camera::GetWindowDimensions() const
{
    return {__wWidth, __wHeight};
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
    __hasMoved = true;
}