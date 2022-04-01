/*****************************************************************//**
 * \file   Window.cpp
 * \brief  Source code of the Window class
 * 
 * \author Kevin Pruvost (pruvostkevin0@gmail.com)
 * \date   March, 26 2022
 *********************************************************************/
#include "Window.hpp"

// Window dimensions
static int WIDTH = 1000;
static int HEIGHT = 750;

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

bool Window::Init(const char * windowName)
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

	// Set the required callback functions
	glfwSetFramebufferSizeCallback(window, [](GLFWwindow * window, int width, int height) {
		WIDTH = width;
		HEIGHT = height;
		glViewport(0, 0, width, height);
	});

	// Input related
	Input::Init(window);
	glfwSetKeyCallback(window, InputKeyCallback);
	glfwSetCursorPosCallback(window, InputMouseCallback);
	glfwSetScrollCallback(window, InputScrollCallback);

	// Initialize GLAD to setup the OpenGL Function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize glad" << std::endl;
		return false;
	};

	// Setup OpenGL options
	glEnable(GL_DEPTH_TEST);
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
