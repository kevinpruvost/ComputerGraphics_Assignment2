/*****************************************************************//**
 * \file   Texture.hpp
 * \brief  Texture Loading Class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 01 2022
 *********************************************************************/
#pragma once

// Project includes
#include <SOIL.h>

// GLAD includes
#include <glad\glad.h>

// GLM includes
#include <glm\glm.hpp>

// C++ includes
#include <string>

class Texture
{
public:
    Texture();
    ~Texture();

    bool GenerateTexture(const std::string & filePath);

    int GetWidth() const;
    int GetHeight() const;
    glm::ivec2 GetDimensions() const;
    GLuint GetTexture() const;

private:
    int __width, __height;
    GLuint __textureId;
};

bool GenerateTexture(const std::string & filePath, Texture & texture);