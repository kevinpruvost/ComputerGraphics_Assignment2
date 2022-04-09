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
#include "OGL_Implementation\Text\Text.hpp"

// Constants
#include "Constants.hpp"

// Display Mode
GLuint displayMode = 1; // 0 = Point, 1 = Face, 2 = Wireframe, 3 = Face + Wireframe

// pointers to model / view / projection matrices
glm::mat4 model(1);

std::vector<Font> fonts;

void LoadShadersAndFonts()
{
	Font font1 = GenerateFont(Constants::Paths::arialFont);
	Font font2 = GenerateFont(Constants::Paths::starFont);
	Font font3 = GenerateFont(Constants::Paths::notesFont);

	// Setting default font
	SetDefaultFont(font1);

	fonts.insert(fonts.begin(), { font1, font2, font3 });

	// Build and compile our shader program
	Shader pointShader = GenerateShader(Constants::Paths::pointShaderVertex, Constants::Paths::pointShaderFrag);
	Shader faceShader = GenerateShader(Constants::Paths::faceShaderVertex, Constants::Paths::faceShaderFrag);
	Shader wireframeShader = GenerateShader(Constants::Paths::wireframeShaderVertex, Constants::Paths::wireframeShaderFrag);
	Shader text2DShader = GenerateShader(Constants::Paths::text2DShaderVertex, Constants::Paths::text2DShaderFrag);
	Shader text3DShader = GenerateShader(Constants::Paths::text3DShaderVertex, Constants::Paths::text3DShaderFrag);

	// Setting default shaders
	SetDefaultPointShader(pointShader);
	SetDefaultFaceShader(faceShader);
	SetDefaultWireframeShader(wireframeShader);
	SetDefault2DTextShader(text2DShader);
	SetDefault3DTextShader(text3DShader);

	pointShader.AddGlobalUbo(0, "ViewProj");
	faceShader.AddGlobalUbo(0, "ViewProj");
	wireframeShader.AddGlobalUbo(0, "ViewProj");
	text2DShader.AddGlobalUbo(1, "Projection");
	text3DShader.AddGlobalUbo(2, "ViewAndProj");
}

// The MAIN function, from here we start the application and run the game loop
int main()
{
	Window window;
	if (!window.Init(Constants::Window::windowName, Constants::Paths::windowIcon))
		return EXIT_FAILURE;

	Rendering::Init();

	LoadShadersAndFonts();

	// Load model
	//Obj my_obj;
	//if (!my_obj.TryLoad(Constants::Paths::Models::Rat::objFile))
	//{
	//	std::cerr << "Couldn't load " << Constants::Paths::Models::Rat::objFile << std::endl;
	//	return EXIT_FAILURE;
	//} 
	//Mesh mesh = GenerateMesh(my_obj);
	//Entity rat(mesh, { 1.0f, 1.0f, 0.0f });
	//Texture ratTexture;
	//if (!ratTexture.GenerateTexture(Constants::Paths::Models::Rat::texture))
	//{
	//	Log::Print(stderr, "%s couldn't be loaded!\n", Constants::Paths::earth);
	//	return EXIT_FAILURE;
	//}
	//rat.SetTexture(ratTexture);

	Mesh sphereMesh = GenerateMeshSphere(1.0f, 36, 18, true);
	Texture earthTexture, jupiterTexture, marsTexture, mercuryTexture, moonTexture,
		neptuneTexture, saturnTexture, sunTexture, uranusTexture, venusTexture;
	std::unordered_map<Texture *, const char *> texturesMapLoading = {
		{&earthTexture,  Constants::Paths::earth},  {&jupiterTexture, Constants::Paths::jupiter},
		{&marsTexture,   Constants::Paths::mars},   {&mercuryTexture, Constants::Paths::mercury},
		{&moonTexture,   Constants::Paths::moon},   {&neptuneTexture, Constants::Paths::neptune},
		{&saturnTexture, Constants::Paths::saturn}, {&sunTexture,     Constants::Paths::sun},
		{&uranusTexture, Constants::Paths::uranus}, {&venusTexture,   Constants::Paths::venus}
	};
	for (auto & pair: texturesMapLoading)
	{
		if (!pair.first->GenerateTexture(pair.second))
		{
			Log::Print(stderr, "%s couldn't be loaded!\n", pair.second);
			return EXIT_FAILURE;
		}
	}

	Camera camera(window.windowWidth(), window.windowHeight(), 17.560759f, -7.319709f, 1127.321655f);
	camera.MovementSpeed *= 50.0f;
	mainCamera = &camera;

	Entity sun(sphereMesh), jupiter(sphereMesh),
		mars(sphereMesh), mercury(sphereMesh),
		moon(sphereMesh), neptune(sphereMesh),
		saturn(sphereMesh), earth(sphereMesh),
		uranus(sphereMesh), venus(sphereMesh);
	earth.RotateX(-90.0f);
	jupiter.RotateX(-90.0f);
	mars.RotateX(-90.0f);
	moon.RotateX(-90.0f);
	saturn.RotateX(-90.0f);
	venus.RotateX(-90.0f);
	neptune.RotateX(-90.0f);
	sun.scale     = glm::vec3(50.0f);   // 1.3927 million km diameter
	jupiter.scale = glm::vec3(25.0f); // 139820 km diameter
	saturn.scale  = glm::vec3(24.0f); // 116460 km diameter
	uranus.scale  = glm::vec3(19.0f);  // 50724 km diameter
	neptune.scale = glm::vec3(17.0f);  // 49244 km diameter
	earth.scale   = glm::vec3(15.0f);  // 12472 km diameter
	venus.scale   = glm::vec3(15.0f);  // 12104 km diameter
	mars.scale    = glm::vec3(8.0f);  // 6779 km diameter
	mercury.scale = glm::vec3(5.0f);  // 4879 km diameter
	moon.scale    = glm::vec3(3.0f);  // 3474 km diameter

	{
		float planetSpan = sun.scale.x;
		float direction = 1.0f;
		// Planets order : Sun -> Mercury -> Venus -> Earth -> Moon -> Mars -> Jupiter -> Saturn -> Uranus -> Neptune
		for (auto e : { &mercury, &venus, &earth, &mars, &jupiter, &saturn, &uranus, &neptune })
		{
			planetSpan += 30.0f + e->scale.x;
			float x = direction * (planetSpan / 2.0f - std::fmod(rand(), planetSpan / 2.0f));
			float z = -direction * (std::sqrt(planetSpan * planetSpan - x * x)); // So that length of vector on x,z = planetSpan
			e->pos = glm::vec3(x, 0.0f, z);
			direction *= -1.0f;
			LOG_PRINT(stdout, "Planetspan: %.2f, X = %.2f, Z = %.2f\n", planetSpan, x, z);
		}
	}
	moon.pos.x = earth.scale.x + 10.0f;
	//moon.pos.z = earth.pos.z;
	earth.SetTexture(earthTexture); jupiter.SetTexture(jupiterTexture); mars.SetTexture(marsTexture);
	mercury.SetTexture(mercuryTexture); moon.SetTexture(moonTexture); neptune.SetTexture(neptuneTexture);
	saturn.SetTexture(saturnTexture); sun.SetTexture(sunTexture); uranus.SetTexture(uranusTexture);
	venus.SetTexture(venusTexture);

	const float textScale = 25.0f;
	const float textDistance = 1.5f;
	Text2D text("The Milky Way", { 0.5f, 0.95f }, 26.0f, { 0.6f, 0.0f, 1.0f });
	Text3D textSun("Sun", { 0.0f, textDistance, 0.0f }, textScale, { 0.0f, 1.0f, 0.0f }); textSun.SetParent(sun);
	Text3D textEarth("Earth", { 0.0f, -textDistance - 2.0f, 0.0f }, textScale, { 0.0f, 1.0f, 0.0f }); textEarth.SetParent(earth);
	Text3D textJupiter("Jupiter", { 0.0f, textDistance, 0.0f }, textScale, { 0.0f, 1.0f, 0.0f }); textJupiter.SetParent(jupiter);
	Text3D textMars("Mars", { 0.0f, -textDistance - 3.0f, 0.0f }, textScale, { 0.0f, 1.0f, 0.0f }); textMars.SetParent(mars);
	Text3D textMercury("Mercury", { 0.0f, -textDistance - 4.0f, 0.0f }, textScale, { 0.0f, 1.0f, 0.0f }); textMercury.SetParent(mercury);
	Text3D textMoon("Moon", { 0.0f, textDistance + 2.0f, 0.0f }, textScale, { 0.0f, 1.0f, 0.0f }); textMoon.SetParent(moon);
	Text3D textNeptune("Neptune", { 0.0f, textDistance, 0.0f }, textScale, { 0.0f, 1.0f, 0.0f }); textNeptune.SetParent(neptune);
	Text3D textSaturn("Saturn", { 0.0f, -textDistance - 1.0f, 0.0f }, textScale, { 0.0f, 1.0f, 0.0f }); textSaturn.SetParent(saturn);
	Text3D textUranus("Uranus", { 0.0f, textDistance, 0.0f }, textScale, { 0.0f, 1.0f, 0.0f }); textUranus.SetParent(uranus);
	Text3D textVenus("Venus", { 0.0f, textDistance, 0.0f }, textScale, { 0.0f, 1.0f, 0.0f }); textVenus.SetParent(venus);

	bool cameraLock = false;

	float rotationSpeed = 300.0f;

	earth.SetParent(sun);
	moon.SetParent(earth);
	jupiter.SetParent(sun);
	mars.SetParent(sun);
	mercury.SetParent(sun);
	neptune.SetParent(sun);
	saturn.SetParent(sun);
	uranus.SetParent(sun);
	venus.SetParent(sun);

	// GUI
	GUI gui(window.window);
	// Creating Second Window
	gui.AddCallback([&]() {
		ImGui::SetNextWindowPos(
			{ImGui::GetIO().DisplaySize.x - 20.0f - 300.0f, 20.0f},
			ImGuiCond_::ImGuiCond_Always);
		ImGui::SetNextWindowSize({ 300.0f, 150.0f }, ImGuiCond_::ImGuiCond_Always);
		ImGui::Begin("Object Properties:");

		ImGui::Text(std::format("FPS: {}", GetFpsCount(window.deltaTime(), 0.5f)).c_str());
		ImGui::SliderFloat("Planet Rotation Speed", &rotationSpeed, 0.0f, 1000.0f, "%.1f");

		ImGui::ColorEdit4("Title color", glm::value_ptr(text.color), ImGuiColorEditFlags_::ImGuiColorEditFlags_NoInputs);

//		ImGui::BeginChildFrame(5, { 300.0f, 50.0f });
		if (ImGui::Button("Activate the Funny", { 300.0f, 50.0 }))
		{

		}
//		ImGui::EndChildFrame();

		ImGui::End();
		return true;
	});

	float backgroundColor[4] = { 0.0f, 0.0f, 0.08f, 1.0f };

	window.Loop([&]() {
		// Render
		// Clear the colorbuffer
		glClearColor(backgroundColor[0], backgroundColor[1], backgroundColor[2], backgroundColor[3]);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Switch mesh mode
		if (window.key(GLFW_KEY_C) == InputKey::JustPressed)
			displayMode = (displayMode + 1) % 4;

		// Planets Rotation
		{
			// Around themselves
			earth.RotateY(window.deltaTime() * 1.0f * rotationSpeed); // 1 earth day
			sun.RotateY(window.deltaTime() * (1.0f / 35.0f) * rotationSpeed); // 35 earth days
			moon.RotateY(window.deltaTime() * (1.0f / 27.3f) * rotationSpeed); // 27.3 earth days
			mercury.RotateY(window.deltaTime() * (1.0f / 58.6f) * rotationSpeed); // 58.6 earth days
			venus.RotateY(window.deltaTime() * (1.0f / 243.0f) * rotationSpeed); // 243 earth days
			mars.RotateY(window.deltaTime() * (1.0f / 1.02f) * rotationSpeed); // 1.02 earth days
			jupiter.RotateY(window.deltaTime() * (1.0f / 0.413f) * rotationSpeed); // 0.413 earth days
			saturn.RotateY(window.deltaTime() * (1.0f / 0.44f) * rotationSpeed); // 0.44 earth days
			uranus.RotateY(window.deltaTime() * (1.0f / 0.7183f) * rotationSpeed); // 0.7183 earth days
			neptune.RotateY(window.deltaTime() * (1.0f / 0.67125f) * rotationSpeed); // 0.67125 earth days
		}

		// Model movement
		if (window.key(GLFW_KEY_LEFT) == InputKey::Pressed)
			rotationSpeed = std::max(0.0f, std::min(rotationSpeed - window.deltaTime() * 10.0f, 1000.0f));
		if (window.key(GLFW_KEY_RIGHT) == InputKey::Pressed)
			rotationSpeed = std::max(0.0f, std::min(rotationSpeed + window.deltaTime() * 10.0f, 1000.0f));

		// Camera Lock
		if (window.key(GLFW_KEY_L) == InputKey::JustPressed)
		{
			cameraLock = !cameraLock;
			glfwSetInputMode(window.window, GLFW_CURSOR,
				cameraLock ? GLFW_CURSOR_DISABLED : GLFW_CURSOR_NORMAL);
		}

		// Font change
		if (window.key(GLFW_KEY_F) == InputKey::JustPressed)
		{
			std::rotate(fonts.begin(), fonts.begin() + 1, fonts.end());
			for (auto t : { &textEarth, &textJupiter, &textMars, &textMercury, &textMoon, &textNeptune, &textSaturn, &textSun, &textUranus, &textVenus })
			{
				t->font = fonts[0];
			}
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
		for (const auto & e : { earth, jupiter, mars, mercury, moon, neptune, saturn, sun, uranus, venus })
		{
			if (displayMode == 0) Rendering::DrawVertices(e);
			if (displayMode & 1)  Rendering::DrawFaces(e);
			if (displayMode & 2)  Rendering::DrawWireframe(e);
		}

		// Drawing 2D Text
		Rendering::DrawText(text);
		
		// Drawing 3D Text
		for (const auto & t : { textEarth, textJupiter, textMars, textMercury, textMoon, textNeptune, textSaturn, textSun, textUranus, textVenus })
		{
			Rendering::DrawText(t);
		}


		// Drawing ImGui GUI
		if (!gui.DrawGUI()) return false;

		return true;
	});

	return EXIT_SUCCESS;
}