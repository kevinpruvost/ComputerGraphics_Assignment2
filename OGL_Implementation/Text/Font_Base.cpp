/*****************************************************************//**
 * \file   Font_Base.cpp
 * \brief  Font_Base source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 06 2022
 *********************************************************************/
#include "Font_Base.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"

static bool FreeTypeInitialized = false;
static FT_Library ft;

Character::Character(const wchar_t character, FT_Face & face, bool & error)
    : __texture{ 0 }
    , __error{ true }
{
    error = true;
    if (FT_Load_Char(face, character, FT_LOAD_RENDER))
    {
        __error = error = false;
        return;
    }

    glGenTextures(1, &__texture);
    glBindTexture(GL_TEXTURE_2D, __texture);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RED,
        face->glyph->bitmap.width,
        face->glyph->bitmap.rows,
        0,
        GL_RED,
        GL_UNSIGNED_BYTE,
        face->glyph->bitmap.buffer
    );
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Character::~Character()
{
    if (__error) glDeleteTextures(1, &__texture);
}

Font_Base::Font_Base(const char * fontPath)
    : __face{ nullptr }
{
    if (!InitFreeType()) throw std::exception("FreeType not initialized.");

    if (FT_New_Face(ft, fontPath, 0, &__face))
    {
        LOG_PRINT(stderr, "FreeType couldn't load '%s'.\n", fontPath);
        throw std::exception("Couldn't load font.");
    }
    if (FT_Set_Pixel_Sizes(__face, 0, 48))
    {
        LOG_PRINT(stderr, "FreeType couldn't change font size: '%s'.\n", fontPath);
        throw std::exception("Couldn't change font size.");
    }

    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // Load first 128 characters of ASCII set
    __characters.reserve(128);
    for (GLubyte c = 0; c < 128; c++)
    {
        bool error;
        Character newChar(c, __face, error);
        if (!error)
        {
            LOG_PRINT(stderr, "Couldn't load '%c' character from '%s' font.\n", c, fontPath);
            continue;
        }   
        __characters.insert({c, newChar});
    }
}

Font_Base::~Font_Base()
{
    FT_Done_Face(__face);
    if (FreeTypeInitialized)
    {
        FT_Done_FreeType(ft);
        FreeTypeInitialized = false;
    }
}

bool Font_Base::InitFreeType()
{
    if (FreeTypeInitialized) return true;

    if (FT_Init_FreeType(&ft))
    {
        LOG_PRINT(stderr, "FreeType couldn't be initialized.\n");
        return false;
    }
    FreeTypeInitialized = true;
    return true;
}
