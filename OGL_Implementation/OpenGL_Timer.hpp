/*****************************************************************//**
 * \file   OpenGL_Timer.hpp
 * \brief  Class made for performance measures
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 02 2022
 *********************************************************************/
#pragma once

// OpenGL includes
#include <GLAD\glad.h>

class OpenGL_Timer
{
public:
    /**
     * @brief Timer Constructor
    */
    OpenGL_Timer();

    void Start();
    GLuint64 End();

public:
    GLuint query;
};
