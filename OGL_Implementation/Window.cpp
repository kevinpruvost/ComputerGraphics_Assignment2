/*****************************************************************//**
 * \file   Window.cpp
 * \brief  Source code of the Window class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#include "Window.hpp"

// Project includes
#include "OGL_Implementation\DebugInfo\Log.hpp"
#include <SOIL.h>

// Window dimensions
static int WIDTH = 1200;
static int HEIGHT = 800;
static bool windowDimensionsChanged_ = false;
static bool windowIsFocused_ = true;

Window::Window()
	: window { nullptr }
	, __initialized { GL_FALSE }
{
}

Window::~Window()
{
	// Terminate GLFW, clearing any resources allocated by GLFW.
	if (__initialized) glfwTerminate();
	if (window) glfwDestroyWindow(window);
}

bool Window::Init(const char * windowName, const char * iconPath)
{
	// Init GLFW
	if ((__initialized = glfwInit()) == GL_FALSE)
		return false;
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	if ((window = glfwCreateWindow(WIDTH, HEIGHT, windowName, nullptr, nullptr)) == nullptr)
		return false;
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1); // Disables V-Sync

	// Sets icon if iconPath != nullptr
	if (iconPath)
	{
		GLFWimage icon;
		icon.pixels = SOIL_load_image(iconPath, &icon.width, &icon.height, nullptr, SOIL_LOAD_RGBA);
		if (icon.pixels)
		{
			glfwSetWindowIcon(window, 1, &icon);
			SOIL_free_image_data(icon.pixels);
		}
	}

	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow * window, int width, int height) {
		WIDTH = width;
		HEIGHT = height;
		glViewport(0, 0, width, height);
		windowDimensionsChanged_ = true;
	});

	// Input related
	Input::Init(window);
	glfwSetKeyCallback(window, InputKeyCallback);
	glfwSetCursorPosCallback(window, InputMouseCallback);
	glfwSetScrollCallback(window, InputScrollCallback);
	glfwSetWindowFocusCallback(window, [](GLFWwindow * window, int focusState) {
		windowIsFocused_ = static_cast<bool>(focusState);
		LOG_PRINT(stdout, "Window is %s focused.\n", windowIsFocused_ ? "" : "not");
	});

	// Initialize GLAD to setup the OpenGL Function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize glad" << std::endl;
		return false;
	};

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST); // Depth
	glEnable(GL_CULL_FACE); // Face Culling
	glEnable(GL_BLEND); // Blending
	glEnable(GL_LIGHTING);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // Blending options
	return true;
}

bool Window::Loop(const std::function<bool()> & lambda)
{
	GLfloat lastFrame = 0.0f;      // Time of last frame

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		// Calculate deltatime of current frame
		GLfloat currentFrame = glfwGetTime();
		__deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		InputCallbackCorrection();
		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Launches lambda passed by user
		if (!lambda())
			return false;
		
		windowDimensionsChanged_ = false;

		// Swap the screen buffers
		glfwSwapBuffers(window);
	}
	return true;
}

float Window::deltaTime() const { return __deltaTime; }

int Window::windowWidth() const
{
	return WIDTH;
}

int Window::windowHeight() const
{
	return HEIGHT;
}

bool Window::windowDimensionsHasChanged() const
{
	return windowDimensionsChanged_;
}

bool Window::GetWindowFocused() const
{
	return windowIsFocused_;
}
