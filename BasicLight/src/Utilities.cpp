#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <Windows.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Utilities.h"
#include "Cube.h"
#include "Pyramid.h"

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

static double s_prevTime = 0;
static float s_totalTime = 0;
static float s_deltaTime = 0;

void Utility::ResetTimer()
{
	s_prevTime = glfwGetTime();
	s_totalTime = 0;
	s_deltaTime = 0;
}

float Utility::TickTimer()
{
	double currentTime = glfwGetTime();
	s_deltaTime = (float)(currentTime - s_prevTime);
	s_totalTime += s_deltaTime;
	s_prevTime = currentTime;
	return s_deltaTime;
}

float Utility::GetDeltaTime()
{
	return s_deltaTime;
}

float Utility::GetTotalTime()
{
	return s_totalTime;
}

Shape* Utility::InitShape()
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

std::string Utility::OpenFile(GLFWwindow* a_window, const std::string& a_filter)
{
	OPENFILENAMEA ofn;       // common dialog box structure
	CHAR szFile[260] = { 0 };       // if using TCHAR macros

	// Initialize OPENFILENAME
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = glfwGetWin32Window(a_window);
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	ofn.lpstrFilter = "All\0*.*\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		return ofn.lpstrFile;
	}
	return std::string();
}

GLFWwindow* Utility::OpenGLInit(int a_windowWidth, int a_windowHeight, const char* a_windowName, bool a_isFullscreen)
{
	// Initialise glfw
	if (glfwInit() != GL_TRUE)
	{
		std::cout << "Unable to initialize GLFW" << std::endl;
		return nullptr;
	}

	// Create the window with glfw

	GLFWwindow* window = glfwCreateWindow(a_windowWidth, a_windowHeight, a_windowName, (a_isFullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
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

void Utility::CheckGLErrors()
{
	while (true)
	{
		const GLenum err = glGetError();
		if (GL_NO_ERROR == err)
			break;

		std::cout << "GL Error: " << GetGLErrorStr(err) << std::endl;
	}
}

void Utility::InitImGui(GLFWwindow* a_window)
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

void Utility::ImguiRender()
{
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
}

const char* Utility::GetGLErrorStr(GLenum a_error)
{
	switch (a_error)
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
