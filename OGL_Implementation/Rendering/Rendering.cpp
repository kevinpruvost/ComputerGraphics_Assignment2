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

static std::unique_ptr<Rendering> s_Rendering(nullptr);

Rendering::Rendering()
	: __textVAO{ 0 }
	, __textVBO{ 0 }
{
	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &__textVAO);
	glGenBuffers(1, &__textVBO);
	glBindVertexArray(__textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, __textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Rendering::~Rendering()
{
	glDeleteVertexArrays(1, &__textVAO);
	glDeleteBuffers(1, &__textVBO);
}

GLuint Rendering::GetTextVAO() { return __textVAO; }
GLuint Rendering::GetTextVBO() { return __textVBO; }

void Rendering::Init()
{
	s_Rendering.reset(new Rendering());
}

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

void Rendering::Draw2DText(const Text2D & text)
{
	text.shader.Use();
	text.shader.SetUniformFloat("textColor", text.color);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(s_Rendering->GetTextVAO());

	// Iterate through all characters
	const auto & characters = text.font.GetCharacters();
	GLfloat x = text.pos.x, y = text.pos.y;
	for (const auto & c : text.str)
	{
		const Character & ch = *characters.at(c);

		GLfloat xpos = x + ch.GetBearing().x * text.scale;
		GLfloat ypos = y - (ch.GetSize().y - ch.GetBearing().y) * text.scale;

		GLfloat w = ch.GetSize().x * text.scale;
		GLfloat h = ch.GetSize().y * text.scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos,     ypos,       0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 0.0 },

			{ xpos,     ypos + h,   0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 0.0 },
			{ xpos + w, ypos + h,   1.0, 1.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.GetTextureID());
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, s_Rendering->GetTextVBO());
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.GetAdvance() >> 6) * text.scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
