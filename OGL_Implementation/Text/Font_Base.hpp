/*****************************************************************//**
 * \file   Font_Base.hpp
 * \brief  Font_Base class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 06 2022
 *********************************************************************/
#pragma once

// GLAD includes
#include <glad\glad.h>

// FreeType includes
#include <ft2build.h>
#include <freetype/freetype.h>

// C++ includes
#include <unordered_map>

/**
 * @brief Contains every information about Rendering Characters.
*/
class Character
{
public:
    Character(const wchar_t character, FT_Face & face, bool & error);
    ~Character();

private:
    GLuint __texture;
    bool __error;
};

/**
 * @brief Contains the real data of the font to be
 * stored in a static database.
*/
class Font_Base
{
public:
    Font_Base(const char * fontPath);
    ~Font_Base();

    static bool InitFreeType();

private:
    FT_Face __face;
    std::unordered_map<GLchar, Character> __characters;
};
