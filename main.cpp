/*****************************************************************//**
 * \file   main.cpp
 * \brief  Main of the Assignment 1 of the Fundamentals
 *         of Computer Graphics course
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/

// C++ includes
#include <format>

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
#include "OGL_Implementation\Shader\Shader.hpp"
#include "OGL_Implementation\Obj.hpp"
#include "OGL_Implementation\Camera.hpp"
#include "OGL_Implementation\Mesh\Mesh.hpp"
#include "OGL_Implementation\GUI.hpp"
#include "OGL_Implementation\Entity\Entity.hpp"
#include "OGL_Implementation\OpenGL_Timer.hpp"
#include "OGL_Implementation\DebugInfo\FpsCounter.hpp"
#include "OGL_Implementation\Texture.hpp"
#include "OGL_Implementation\Rendering\Rendering.hpp"

// Constants
#include "Constants.hpp"

// Display Mode
GLuint displayMode = 0;

// pointers to model / view / projection matrices
glm::mat4 model(1);

// The MAIN function, from here we start the application and run the game loop
int main()
{
	Window window;
	if (!window.Init(Constants::Window::windowName, Constants::Paths::windowIcon))
		return EXIT_FAILURE;

	// Build and compile our shader program
	Shader pointShader = GenerateShader(Constants::Paths::pointShaderVertex, Constants::Paths::pointShaderFrag);
	Shader faceShader = GenerateShader(Constants::Paths::faceShaderVertex, Constants::Paths::faceShaderFrag);
	Shader wireframeShader = GenerateShader(Constants::Paths::wireframeShaderVertex, Constants::Paths::wireframeShaderFrag);

	pointShader.AddGlobalUbo(0, "ViewProj");
	faceShader.AddGlobalUbo(0, "ViewProj");
	wireframeShader.AddGlobalUbo(0, "ViewProj");

	// Load model
	// Obj my_obj;
	// if (!my_obj.TryLoad(Constants::Paths::Models::Rat::objFile))
	// {
	// 	std::cerr << "Couldn't load " << Constants::Paths::Models::Rat::objFile << std::endl;
	// 	return EXIT_FAILURE;
	// } 
	// Mesh mesh = GenerateMesh(my_obj);

	Mesh sphereMesh = GenerateMeshSphere(1.0f, 36, 18, false);
	Texture texture;
	if (!texture.GenerateTexture(Constants::Paths::earth))
	{
		Log::Print(stderr, "%s couldn't be loaded!\n", Constants::Paths::earth);
		return EXIT_FAILURE;
	}

	Camera camera(window.windowWidth(), window.windowHeight(), 0.0f, 0.0f, 3.0f);

	Entity entity(sphereMesh, pointShader, wireframeShader, faceShader);
	entity.SetTexture(texture);
	entity.eulerAngles.y = 50.0f;
	entity.eulerAngles.x = 70.0f;

	bool cameraLock = false;

	// GUI
	GUI gui(window.window);
	// Creating Second Window
	bool autoRotation = true;
	gui.AddCallback([&]() {
		ImGui::SetNextWindowPos(
			{ImGui::GetIO().DisplaySize.x - 20.0f - 300.0f, 20.0f},
			ImGuiCond_::ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowSize({ 300.0f, 150.0f }, ImGuiCond_::ImGuiCond_FirstUseEver);
		ImGui::Begin("Object Properties:");

		ImGui::Text(std::format("FPS: {}", GetFpsCount(window.deltaTime(), 0.5f)).c_str());
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
		model = entity.GetModelMatrix();

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

		// Window Dimensions changed
		if (window.windowDimensionsHasChanged())
			camera.SetWindowDimensions(window.windowWidth(), window.windowHeight());

		// Camera/View transformation
		camera.GetProjViewMatrixUbo();

		// Wireframe Color change
		if (window.key(GLFW_KEY_P) == InputKey::JustPressed)
			Rendering::RotateWireframeColor();

		// display mode & activate shader
		if (displayMode == 0) Rendering::DrawVertices(entity);
		if (displayMode & 1)  Rendering::DrawFaces(entity);
		if (displayMode & 2)  Rendering::DrawWireframe(entity);

		// Drawing ImGui GUI
		if (!gui.DrawGUI()) return false;

		return true;
	});

	return EXIT_SUCCESS;
}