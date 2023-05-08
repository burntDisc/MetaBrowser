
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include <iostream>
#include "Skybox.h"
#include "Camera.h"
#include "Player.h"
#include "InputHandler.h"
#include "MotionHandler.h"
#include "Grid.h"

#include "Text.h"

#include <ft2build.h>
#include FT_FREETYPE_H  

// TODO update filesystem include here------only used for finding parentdir----
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
//-----------------------------------------------------------------------------

int main()
{
	// Set up window-----------------------------------------------------------
	// initilize glfw to handle input and window
	glfwInit();
	// Set GLFW version to OpenGL 3.3 with Core profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create window with screen width of monitor (DISABLED)
	auto monitor = glfwGetPrimaryMonitor();
	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
	glfwWindowHint(GLFW_RED_BITS, mode->redBits);
	glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
	glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
	glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

	// Set Screen and Size
	int width = 1500; //mode->width;//1500;
	int height = 900; //mode->height;//900;
	GLFWwindow* window = glfwCreateWindow(width, height, "5t4r", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(mode->width, mode->height, "5t4r", monitor, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	//Load GLAD to configure opengl---------------------------------------------
	gladLoadGL();
	glViewport(0, 0, width, height);
	// Enables the Depth Testing
	glEnable(GL_DEPTH_TEST);
	//Configures the blending function
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Uncomment to ignore internals of models (breaks explosion internals)
	//glEnable(GL_CULL_FACE);
	// Keeps front faces
	//glCullFace(GL_FRONT);
	// Uses counter clock-wise standard
	//glFrontFace(GL_CCW);
	


	// Shaders---------------------------------------------------------------------------

	Shader skyboxShader("shaders/skybox.vert", "shaders/skybox.frag");
	Shader textShader("shaders/text.vert", "shaders/text.frag");
	Shader rawShader("shaders/raw.vert", "shaders/raw.frag");
	
	// Create SkyBox---------------------------------------------------------------------
	std::string parentDir = fs::current_path().string();
	std::string skyboxFacesDirectory = parentDir + "/models/skybox/";
	Skybox skybox(skyboxShader, skyboxFacesDirectory);

	// create player----------------------------------------------------------------------
	Player player(glm::vec3(3.0f, 1000.0f, 10.0f));

	//create camera-----------------------------------------------------------------------
	float FOV = 45.0f;
	Camera camera(width, height);
	camera.Bind(&player.translation, &player.orientation, &FOV);
	camera.SetSkyboxUniforms(skyboxShader);

	// create Text
	glm::vec3 textTranslation1(100.0f, 100.0f, 100.0f);
	glm::quat textRotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 textScale(1.0f, 1.0f, 1.0f);
	Text text1(textShader, "two is the one", "arial.ttf", textTranslation1, textRotation, textScale, player);

	glm::vec3 textTranslation0(-100.0f, 100.0f, 100.0f);
	Text text0(textShader, "one is the zero", "arial.ttf", textTranslation0, textRotation, textScale, player);

	// create Triangle grid----------------------------------------------------------------------
	Grid grid(rawShader, textShader, camera, 3000, 2, player);
	MotionHandler::AddSolidObject(&grid);

	while (!glfwWindowShouldClose(window))
	{
		// Specify the color of the background GREEN For debug
		// (Skybox Draws over)
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		// Clean the back buffer and depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		double time = glfwGetTime();

		InputHandler::UpdateGamepad();
		player.Update(time);

		grid.Draw();

		//camera.SetSkyboxUniforms(skyboxShader);
		//skybox.Draw();
		camera.SetCameraUniforms(textShader);
		text1.RenderText(textShader,  0.0f, 0.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));

		text0.RenderText(textShader, 0.0f, 0.0f, 1.0f, glm::vec3(1.0f, 1.0f, 0.0f));

		// Swap back with front buffer
		glfwSwapBuffers(window);

		// Process glfw events
		glfwPollEvents();
	}
	// Delete and clean up
	glfwDestroyWindow(window);

	glfwTerminate();
	return 0;
}
