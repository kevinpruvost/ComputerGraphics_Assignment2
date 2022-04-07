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
#include "OGL_Implementation\Text\Text.hpp"

/**
 * @brief Static class containing rendering utilities
*/
class Rendering
{
public:
    Rendering();
    ~Rendering();
public:
    GLuint GetTextVAO();
    GLuint GetTextVBO();

private:
    GLuint __textVAO, __textVBO;

public:
    /**
     * @brief To call to initialize Rendering
    */
    static void Init();

    // Entities
    static void DrawFaces(const Entity & entity);
    static void DrawWireframe(const Entity & entity);
    static void DrawVertices(const Entity & entity);

    static void RotateWireframeColor();

    // Text
    static void Draw2DText(const Text2D & text);
};
