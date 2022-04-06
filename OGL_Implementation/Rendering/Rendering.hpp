/*****************************************************************//**
 * \file   Rendering.hpp
 * \brief  Rendering class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 04 2022
 *********************************************************************/
#pragma once

// Project includes
#include "OGL_Implementation\Entity\Entity.hpp"

/**
 * @brief Static class containing rendering utilities
*/
class Rendering
{
public:
    static void DrawFaces(const Entity & entity);
    static void DrawWireframe(const Entity & entity);
    static void DrawVertices(const Entity & entity);

    static void RotateWireframeColor();
};
