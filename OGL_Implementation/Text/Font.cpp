/*****************************************************************//**
 * \file   Font.cpp
 * \brief  Font source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 06 2022
 *********************************************************************/
#include "Font.hpp"

// C++ includes
#include <memory>

static std::vector<std::unique_ptr<Font_Base>> fontDB;

Font::Font(const GLuint fontId)
    : __fontId{ fontId }
{
}

GLuint Font::GetFontDatabaseID() const
{
    return __fontId;
}

Font GenerateFont(const char * fontPath)
{
    fontDB.emplace_back(new Font_Base(fontPath));
    return Font(fontDB.size() - 1);
}
