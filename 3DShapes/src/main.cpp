#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Cube.h"
#include "Pyramid.h"

#include "Shader.h"
#include "Camera.h"
#include "Utilities.h"


GLFWwindow* OpenGLInit(int a_windowWidth, int a_windowHeight, const char* a_windowName)
{
	// Initialise glfw
	if (glfwInit() != GL_TRUE)
	{
		std::cout << "Unable to initialize GLFW" << std::endl;
		return nullptr;
	}

	// Create the window with glfw
	GLFWwindow* window = glfwCreateWindow(a_windowWidth, a_windowHeight, a_windowName, 0, 0);
	if (window == nullptr)
	{
		std::cout << "Unable to create a GLFW Window" << std::endl;
		glfwTerminate();
		return nullptr;
	}

	// Set it as active
	glfwMakeContextCurrent(window);

	// Load glad
	if (!gladLoadGL()) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return nullptr;
	}

	// Print GL version to console
	std::cout << "=====\nOpenGl:\nVendor: " << glGetString(GL_VENDOR) << "\nRenderer: " << glGetString(GL_RENDERER) << "\nVersion: " << glGetString(GL_VERSION) << "\n=====\n";

	return window;
}

Shape* InitialiseShape()
{
	std::cout << "\n1. Pyramid\n2. Cube\n";

	int option = 0;
	bool validOption = false;

	// Wait until we pick a valid option
	while (!validOption)
	{
		std::cin >> option;
		switch (option)
		{
		case 1:
		{
			return new Pyramid();
			break;
		}
		case 2:
		{
			return new Cube();
			validOption = true;
			break;
		}
		default:
		{
			break;
		}
		}
	}

}

int main()
{
	GLFWwindow* window = OpenGLInit(1920, 1080, "Callum");

	if (window == nullptr)
	{
		return -1;
	}

	Utility::ResetTimer();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shape* shape = InitialiseShape();

	Shader simpleShader = Shader("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");

	Camera cam = Camera();


	unsigned int shaderProgram = simpleShader.GetProgramID();

	float deltaTime = 0;

	while (glfwWindowShouldClose(window) == 0)
	{
		Utility::TickTimer();
		deltaTime = Utility::GetDeltaTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		simpleShader.Bind();
		cam.Draw(shaderProgram);
		shape->Draw(shaderProgram);
		simpleShader.Unbind();

		shape->Rotate(deltaTime, glm::vec3(0, 1, 0));

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete shape;

	glfwDestroyWindow(window);
	glfwTerminate();
}