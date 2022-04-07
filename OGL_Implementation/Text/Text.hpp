/*****************************************************************//**
 * \file   Text.hpp
 * \brief  Text class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 06 2022
 *********************************************************************/
#pragma once

// GLM includes
#include <GLM\glm.hpp>

// Project includes
#include "Font.hpp"
#include "OGL_Implementation\Shader\Shader.hpp"

// C++ includes
#include <string>

class Text2D
{
public:
    Text2D(const Font & _font, const Shader & _shader, const std::string & _str, const glm::vec2 & xy, const GLfloat _scale, const glm::vec3 & _color);
    Text2D(const std::string & _str, const glm::vec2 & xy, const GLfloat _scale, const glm::vec3 & _color);

public:
    Font font;
    Shader shader;
    std::string str;
    glm::vec2 pos;
    GLfloat scale;
    glm::vec3 color;
};

void SetDefaultFont(const Font & font);
void SetDefault2DTextShader(const Shader & shader);
void SetDefault3DTextShader(const Shader & shader);