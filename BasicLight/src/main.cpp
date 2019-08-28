#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Cube.h"
#include "Pyramid.h"

#include "Shader.h"
#include "Camera.h"
#include "Utilities.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#pragma region GLErrors

const char* GetGLErrorStr(GLenum err)
{
	switch (err)
	{
	case GL_NO_ERROR:          return "No error";
	case GL_INVALID_ENUM:      return "Invalid enum";
	case GL_INVALID_VALUE:     return "Invalid value";
	case GL_INVALID_OPERATION: return "Invalid operation";
	case GL_STACK_OVERFLOW:    return "Stack overflow";
	case GL_STACK_UNDERFLOW:   return "Stack underflow";
	case GL_OUT_OF_MEMORY:     return "Out of memory";
	default:                   return "Unknown error";
	}
}

void CheckGLError()
{
	while (true)
	{
		const GLenum err = glGetError();
		if (GL_NO_ERROR == err)
			break;

		std::cout << "GL Error: " << GetGLErrorStr(err) << std::endl;
	}
}

#pragma endregion



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

float gammaCorrection = 1.2;

int main()
{
	GLFWwindow* window = OpenGLInit(1920, 1080, "Callum");

	if (window == nullptr)
	{
		return -1;
	}

	Utility::ResetTimer();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shape* shape = InitialiseShape();

	Shader simpleShader = Shader("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");
	Shader lightShader = Shader("resources/shaders/lightVertex.glsl", "resources/shaders/lightFragment.glsl");

	Camera cam = Camera();

	// Create and bind texture
	unsigned int texture1;

	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load image, create texture and generate mipmaps
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load("resources/textures/test3.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	unsigned int shaderProgram = simpleShader.GetProgramID();
	unsigned int lightProgram = lightShader.GetProgramID();

	glm::vec3 lightPos = glm::vec3(0, 2, -5);
	Cube light = Cube();
	light.SetPosition(lightPos);
	light.Scale(glm::vec3(0.2f, 0.2f, 0.2f));

	simpleShader.Bind();
	simpleShader.SetInt("gTexture", 0);
	simpleShader.SetVec3("gLightPos", lightPos);
	simpleShader.SetFloat("gGamma", gammaCorrection);

	float deltaTime = 0;

	while (glfwWindowShouldClose(window) == 0)
	{
		Utility::TickTimer();
		deltaTime = Utility::GetDeltaTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		/// DRAW
		simpleShader.Bind();
		cam.Draw(shaderProgram);
		shape->Draw(shaderProgram);
		simpleShader.Unbind();

		lightShader.Bind();
		cam.Draw(lightProgram);
		light.Draw(lightProgram);
		lightShader.Unbind();


		/// UPDATE
		shape->Rotate(deltaTime, glm::vec3(0, 1, 0));

		cam.Update(deltaTime);


		// PRESENT
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete shape;

	glfwDestroyWindow(window);
	glfwTerminate();
}