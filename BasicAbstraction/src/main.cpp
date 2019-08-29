#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Cube.h"
#include "Pyramid.h"

#include "Shader.h"
#include "Camera.h"
#include "Utilities.h"

#include "Texture.h"




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

void InitImGui(GLFWwindow* a_window)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}
	style.Colors[ImGuiCol_WindowBg] = ImVec4(0.15f, 0.15f, 0.15f, style.Colors[ImGuiCol_WindowBg].w);

	// Setup Platform/Renderer bindings, with our main app window
	ImGui_ImplGlfw_InitForOpenGL(a_window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}



GLFWwindow* OpenGLInit(int a_windowWidth, int a_windowHeight, const char* a_windowName)
{
	// Initialise glfw
	if (glfwInit() != GL_TRUE)
	{
		std::cout << "Unable to initialize GLFW" << std::endl;
		return nullptr;
	}

	// Create the window with glfw
	
	GLFWwindow* window = glfwCreateWindow(a_windowWidth, a_windowHeight, a_windowName, nullptr, nullptr);
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

	InitImGui(window);

	Utility::ResetTimer();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	Shape* shape = InitialiseShape();

	Shader simpleShader = Shader("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");
	Shader lightShader = Shader("resources/shaders/lightVertex.glsl", "resources/shaders/lightFragment.glsl");

	Camera cam = Camera();

	Texture newTexture = Texture("resources/textures/test3.jpg");

	glm::vec3 lightPos = glm::vec3(0, 2, -5);
	Cube light = Cube();
	light.SetPosition(lightPos);
	light.Scale(glm::vec3(0.2f, 0.2f, 0.2f));

	simpleShader.Bind();
	simpleShader.SetInt("gTexture", 0);
	simpleShader.SetVec3("gLightPos", lightPos);
	simpleShader.SetFloat("gGamma", gammaCorrection);

	float deltaTime = 0;
	unsigned int shaderProgram = simpleShader.GetProgramID();
	unsigned int lightProgram = lightShader.GetProgramID();

	char textDirBuf[128] = "resources/textures/test3.jpg";

	while (glfwWindowShouldClose(window) == 0)
	{
		// Set up Dear ImGui for a new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();


		Utility::TickTimer();
		deltaTime = Utility::GetDeltaTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		/// DRAW
		simpleShader.Bind();
		simpleShader.SetVec3("gLightPos", lightPos);
		newTexture.Bind(GL_TEXTURE0);
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
		light.SetPosition(lightPos);


		// IMGUI DRAW
		ImGui::Begin("Properties");
		ImGui::DragFloat3("Light Position", glm::value_ptr(lightPos), 0.05f);
		ImGui::NewLine();
		ImGui::InputText("Texture Path", textDirBuf, IM_ARRAYSIZE(textDirBuf));
		if (ImGui::Button("Reload Texture"))
		{
			newTexture.Reload(textDirBuf);
		}

		ImGui::End();


		
		/// IMGUI RENDER
		// Grab IO and make sure size is correct
		ImGuiIO& io = ImGui::GetIO();
		
		//io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());

		// Rendering
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		// Viewports rendering
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		// PRESENT
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete shape;

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
}