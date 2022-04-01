/*****************************************************************//**
 * \file   main.cpp
 * \brief  Main of the Assignment 1 of the Fundamentals
 *         of Computer Graphics course
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// Project includes
#include "OGL_Implementation\Window.hpp"
#include "OGL_Implementation\Shader.hpp"
#include "OGL_Implementation\Obj.hpp"
#include "OGL_Implementation\Camera.hpp"
#include "OGL_Implementation\Mesh.hpp"
#include "OGL_Implementation\GUI.hpp"
#include "OGL_Implementation\Entity.hpp"

// buffer binding & drawing functions
GLsizei bindFaces(GLuint VAO, GLuint VBO, const Obj& obj);
void drawFaces(Shader& shader, GLuint VAO, int num);

GLsizei bindVertices(GLuint VAO, GLuint VBO, const Obj& obj);
void drawVertices(Shader& shader, GLuint VAO, int num);
void drawWireframe(Shader & shader, GLuint VAO, int num);

// Camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
bool firstMouse = true;

// hyper-parameters
glm::vec3 translation = glm::vec3(0.0f, 0.0f, -2.0f);
glm::vec3 rotationAxis = glm::vec3(-1.0f, 1.0f, -1.0f);
glm::vec3 color = glm::vec3(0.1f, 0.95f, 0.1f); 
glm::vec3 color1 = glm::vec3(1.0f, 0.95f, 0.1f);
glm::vec3 color2 = glm::vec3(0.1f, 0.95f, 1.0f);
glm::vec3 color3 = glm::vec3(0.5f, 0.2f, 0.3f);
std::array<glm::vec3, 4> wireframeColors = { color, color1, color2, color3 };
GLfloat rotationDegrees = 0.0f;
GLuint displayMode = 0;

constexpr float ROTATE_SPEED = glm::radians(5.0f);
constexpr float TRANSLATE_SPEED = 0.15f;

constexpr const glm::vec3 GLM_UP(0.0f, 1.0f, 0.0f);
constexpr const glm::vec3 GLM_RIGHT(0.0f, 0.0f, 1.0f);
constexpr const glm::vec3 GLM_DOWN = -GLM_UP;
constexpr const glm::vec3 GLM_LEFT = -GLM_RIGHT;

// pointers to model / view / projection matrices
glm::mat4 model(1);
glm::mat4 view(1);
glm::mat4 projection(1);

// object file path
const char* const OBJ_FILE = "resources/eight.uniform.obj";

// The MAIN function, from here we start the application and run the game loop
int main()
{
	Window window;
	if (!window.Init("Assignment 1: Pruvost Kevin 2021400603"))
		return EXIT_FAILURE;

	// Build and compile our shader program
	Shader pointShader("resources/point.vert.glsl", "resources/point.frag.glsl");
	Shader faceShader("resources/face.vert.glsl", "resources/face.frag.glsl");
	Shader wireframeShader("resources/wireframe.vert.glsl", "resources/wireframe.frag.glsl");

	// load model
	Obj my_obj;
	if (!my_obj.TryLoad(OBJ_FILE))
	{
		std::cerr << "Couldn't load " << OBJ_FILE << std::endl;
		return EXIT_FAILURE;
	}

	Mesh mesh = GenerateMesh(my_obj);

	Camera camera(0.0f, 0.0f, 3.0f);

	// Create transformations
	model = glm::rotate(
		glm::rotate(glm::mat4(1.0f),
		glm::radians(50.0f), GLM_UP),
		glm::radians(70.0f), GLM_RIGHT);

	Entity entity(mesh);
	entity.eulerAngles.y = 50.0f;
	entity.eulerAngles.x = 70.0f;

	//std::cout << glm::to_string(model) << std::endl;
	//std::cout << glm::to_string(entity.getModelMatrix()) << std::endl;

	model = entity.getModelMatrix();

	bool cameraLock = false;

	// GUI
	GUI gui(window.window);
	// Creating Second Window
	bool autoRotation = true;
	gui.AddCallback([&]() {
		ImGui::SetNextWindowPos(
			{ImGui::GetIO().DisplaySize.x - 20.0f - 300.0f, 20.0f},
			ImGuiCond_::ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ 300.0f, 125.0f }, ImGuiCond_::ImGuiCond_FirstUseEver);
		ImGui::Begin("Object Properties:");

		ImGui::SliderFloat3("Position", glm::value_ptr(entity.pos), -1.0f, 1.0f);
		ImGui::SliderFloat3("Rotation", glm::value_ptr(entity.eulerAngles), 0.0f, 360.0f);
		ImGui::SliderFloat3("Scale", glm::value_ptr(entity.scale), 0.05f, 3.0f);

		ImGui::Checkbox("Auto-Rotation", &autoRotation);

		ImGui::End();
		return true;
	});

	window.Loop([&]() {
		// Render
		// Clear the colorbuffer
		glClearColor(gui.backgroundColor[0], gui.backgroundColor[1],
			gui.backgroundColor[2], gui.backgroundColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Switch mesh mode
		if (window.key(GLFW_KEY_C) == InputKey::JustPressed)
			displayMode = (displayMode + 1) % 4;

		if (autoRotation)
			entity.eulerAngles.y = fmod(entity.eulerAngles.y + window.deltaTime() * 10.0f, 360.0f);
		model = entity.getModelMatrix();

		// Model movement
		if (window.key(GLFW_KEY_UP) == InputKey::Pressed)
			entity.eulerAngles.x += 30.0f * window.deltaTime();
		if (window.key(GLFW_KEY_DOWN) == InputKey::Pressed)
			entity.eulerAngles.x -= 30.0f * window.deltaTime();
		if (window.key(GLFW_KEY_LEFT) == InputKey::Pressed)
			entity.pos.x -= 3.0f * window.deltaTime();
		if (window.key(GLFW_KEY_RIGHT) == InputKey::Pressed)
			entity.pos.x += 3.0f * window.deltaTime();

		// Camera Lock
		if (window.key(GLFW_KEY_L) == InputKey::JustPressed)
		{
			cameraLock = !cameraLock;
			glfwSetInputMode(window.window, GLFW_CURSOR,
				cameraLock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		}

		if (cameraLock)
		{
			// Camera movement
			if (window.key(GLFW_KEY_W) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Forward, window.deltaTime());
			if (window.key(GLFW_KEY_S) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Backward, window.deltaTime());
			if (window.key(GLFW_KEY_A) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Left, window.deltaTime());
			if (window.key(GLFW_KEY_D) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Right, window.deltaTime());
			if (window.key(GLFW_KEY_SPACE) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Up, window.deltaTime());
			if (window.key(GLFW_KEY_LEFT_CONTROL) == InputKey::Pressed)
				camera.ProcessKeyboard(Camera_Movement::Down, window.deltaTime());

			// Camera Rotation
			if (window.mouseHasMoved())
				camera.ProcessMouseMovement(window.mousePositionOffset().x, window.mousePositionOffset().y);
			if (window.mouseScrollHasMoved())
				camera.ProcessMouseScroll(window.mouseWheelOffset().y);
		}

		// Camera/View transformation
		view = camera.GetViewMatrix();
		projection = camera.GetProjectionMatrix(window.windowWidth(), window.windowHeight());

		// Wireframe Color change
		if (window.key(GLFW_KEY_P) == InputKey::JustPressed)
			std::rotate(wireframeColors.begin(), wireframeColors.begin() + 1, wireframeColors.end());

		// display mode & activate shader
		if (displayMode == 0) drawVertices(pointShader, mesh.verticesVAO(), mesh.verticesNVert());
		if (displayMode & 1) drawFaces(faceShader, mesh.facesVAO(), mesh.facesNVert());
		if (displayMode & 2) drawWireframe(wireframeShader, mesh.facesVAO(), mesh.facesNVert());

		// Drawing ImGui GUI
		if (!gui.DrawGUI()) return false;

		return true;
	});

	return EXIT_SUCCESS;
}

void drawFaces(Shader& shader, GLuint VAO, int num)
{
	shader.Use();

	// get uniform locations
	GLint modelLoc = glGetUniformLocation(shader.program, "model");
	GLint viewLoc = glGetUniformLocation(shader.program, "view");
	GLint projLoc = glGetUniformLocation(shader.program, "projection");

	// pass uniform values to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glDrawArrays(GL_TRIANGLES, 0, num);
	glBindVertexArray(0);
}

void drawWireframe(Shader & shader, GLuint VAO, int num)
{
	shader.Use();

	// get uniform locations
	GLint modelLoc = glGetUniformLocation(shader.program, "model");
	GLint viewLoc = glGetUniformLocation(shader.program, "view");
	GLint projLoc = glGetUniformLocation(shader.program, "projection");

	// pass uniform values to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// use the same color for all points
	GLint colorLoc = glGetUniformLocation(shader.program, "ourColor");
	glUniform3fv(colorLoc, 1, glm::value_ptr(wireframeColors[0]));

	glBindVertexArray(VAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, num);
	glBindVertexArray(0);
}

void drawVertices(Shader& shader, GLuint VAO, int num)
{
	shader.Use();

	// get uniform locations
	GLint modelLoc = glGetUniformLocation(shader.program, "model");
	GLint viewLoc = glGetUniformLocation(shader.program, "view");
	GLint projLoc = glGetUniformLocation(shader.program, "projection");

	// pass uniform values to shader
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

	// use the same color for all points
	GLint colorLoc = glGetUniformLocation(shader.program, "ourColor");
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	glBindVertexArray(VAO);
	glDrawArrays(GL_POINTS, 0, num);
	glBindVertexArray(0);
}