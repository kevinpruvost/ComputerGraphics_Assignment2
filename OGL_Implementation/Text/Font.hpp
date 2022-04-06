/*****************************************************************//**
 * \file   Font.hpp
 * \brief  Font class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 06 2022
 *********************************************************************/
#pragma once

// Project includes
#include "Font_Base.hpp"

class Font
{
public:
    /**
     * @brief Constructor from Shader ID.
     * @param meshId
    */
    Font(const GLuint fontId);

public:
    GLuint GetFontDatabaseID() const;

public:
    GLuint __fontId;
};

/*
 * @brief Generates font from ttf file
 * @param Font Path
*/
Font GenerateFont(const char * fontPath);