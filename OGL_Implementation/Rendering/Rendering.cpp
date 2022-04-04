/*****************************************************************//**
 * \file   Rendering.cpp
 * \brief  Rendering source code
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   April, 04 2022
 *********************************************************************/
#include "Rendering.hpp"

// Wireframe/Points Color
static constexpr const glm::vec3 color = glm::vec3(0.1f, 0.95f, 0.1f);
static constexpr const glm::vec3 color1 = glm::vec3(1.0f, 0.95f, 0.1f);
static constexpr const glm::vec3 color2 = glm::vec3(0.1f, 0.95f, 1.0f);
static constexpr const glm::vec3 color3 = glm::vec3(0.5f, 0.2f, 0.3f);
static std::array<glm::vec3, 4> wireframeColors = { color, color1, color2, color3 };

void Rendering::DrawFaces(const Entity & entity)
{
	const Shader shader = entity.GetFaceShader();
	const glm::mat4 model = entity.GetModelMatrix();

	shader.Use();

	// get uniform locations
	GLint modelLoc = glGetUniformLocation(shader.Program(), "model");

	// pass uniform values to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	glUniform1i(glGetUniformLocation(shader.Program(), "_texture"), 0);

	glBindTexture(GL_TEXTURE_2D, entity.GetTexture().GetTexture());
	glBindVertexArray(entity.GetMesh().facesVAO());
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawElements(GL_TRIANGLES, entity.GetMesh().facesNVert(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Rendering::DrawWireframe(const Entity & entity)
{
	const Shader shader = entity.GetWireframeShader();
	const glm::mat4 model = entity.GetModelMatrix();
	
	shader.Use();

	// get uniform locations
	GLint modelLoc = glGetUniformLocation(shader.Program(), "model");

	// pass uniform values to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// use the same color for all points
	GLint colorLoc = glGetUniformLocation(shader.Program(), "ourColor");
	glUniform3fv(colorLoc, 1, glm::value_ptr(wireframeColors[0]));

	glBindVertexArray(entity.GetMesh().facesVAO());
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawElements(GL_TRIANGLES, entity.GetMesh().facesNVert(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Rendering::DrawVertices(const Entity & entity)
{
	const Shader shader = entity.GetPointShader();
	const glm::mat4 model = entity.GetModelMatrix();
	
	shader.Use();

	// get uniform locations
	GLint modelLoc = glGetUniformLocation(shader.Program(), "model");

	// pass uniform values to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	// use the same color for all points
	GLint colorLoc = glGetUniformLocation(shader.Program(), "ourColor");
	glUniform3fv(colorLoc, 1, glm::value_ptr(wireframeColors[0]));

	glBindVertexArray(entity.GetMesh().verticesVAO());
	glDrawArrays(GL_POINTS, 0, entity.GetMesh().verticesNVert());
	glBindVertexArray(0);
}

void Rendering::RotateWireframeColor()
{
	std::rotate(wireframeColors.begin(), wireframeColors.begin() + 1, wireframeColors.end());
}
