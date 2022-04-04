/*****************************************************************//**
 * \file   Shader.cpp
 * \brief  Shader source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 04 2022
 *********************************************************************/
#include "Shader.hpp"

static std::vector<std::unique_ptr<Shader_Base>> shaderDB;

Shader::Shader(const GLuint shaderId)
    : __shaderId{ shaderId }
{
}

GLuint Shader::Program() const
{
    return shaderDB[__shaderId]->Program();
}

void Shader::Use() const
{
    shaderDB[__shaderId]->Use();
}

void Shader::AddGlobalUbo(const GLuint bindingPoint, const char * bindingPointName) const
{
    shaderDB[__shaderId]->AddGlobalUbo(bindingPoint, bindingPointName);
}

GLuint Shader::GetShaderDatabaseID() const
{
    return __shaderId;
}

Shader GenerateShader(const GLchar * vertexPath, const GLchar * fragmentPath)
{
    shaderDB.emplace_back(new Shader_Base(vertexPath, fragmentPath));
    return Shader(shaderDB.size() - 1);
}
