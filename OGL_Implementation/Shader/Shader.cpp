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

void Shader::SetUniformFloat(const GLchar * uniformName, const GLfloat nb) const
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nb);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const glm::vec2 & nbs) const
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nbs);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2) const
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nb1, nb2);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const glm::vec3 & nbs) const
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nbs);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2, const GLfloat nb3) const
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nb1, nb2, nb3);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const glm::vec4 & nbs) const
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nbs);
}

void Shader::SetUniformFloat(const GLchar * uniformName, const GLfloat nb1, const GLfloat nb2, const GLfloat nb3, const GLfloat nb4) const
{
    shaderDB[__shaderId]->SetUniformFloat(uniformName, nb1, nb2, nb3, nb4);
}
