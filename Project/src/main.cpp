#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Square.h"
#include "Triangle.h"


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

int main()
{
	GLFWwindow* window = OpenGLInit(1920, 1080, "Callum");

	if (window == nullptr)
	{
		return -1;
	}

	Shape* shape = nullptr;

	std::cout << "\n1. Triangle\n2. Square\n";

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
			shape = new Triangle();
			validOption = true;
			break;
		}
		case 2:
		{
			shape = new Square();
			validOption = true;
			break;
		}
		default:
		{
			break;
		}
		}
	}


	while (glfwWindowShouldClose(window) == 0)
	{

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		shape->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
}