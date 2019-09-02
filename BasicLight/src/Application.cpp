#include "Application.h"
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <iostream>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Utilities.h"

Application::Application() : m_isRunning(false)
{
}

bool Application::CreateApp(const char* a_name, int a_width, int a_height, bool a_bFullscreen)
{
	m_window.width = a_width, m_window.height = a_height, m_window.isFullscreen = a_bFullscreen, m_window.name = a_name;

	// Initialise glfw
	if (glfwInit() != GL_TRUE)
	{
		std::cout << "Unable to initialize GLFW" << std::endl;
		return false;
	}

	// Create the window with glfw
	GLFWwindow* window = glfwCreateWindow(a_width, a_height, a_name, (a_bFullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	if (window == nullptr)
	{
		std::cout << "Unable to create a GLFW Window" << std::endl;
		glfwTerminate();
		return false;
	}

	// Set it as active
	glfwMakeContextCurrent(window);

	// Load glad
	if (!gladLoadGL()) {
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	// Print GL version to console
	std::cout << "=====\nOpenGl:\nVendor: " << glGetString(GL_VENDOR) << "\nRenderer: " << glGetString(GL_RENDERER) << "\nVersion: " << glGetString(GL_VERSION) << "\n=====\n";

	m_window.pWindow = window;

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
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	return OnCreate();
}

void Application::Run(const char* a_name, int a_width, int a_height, bool a_bFullscreen)
{
	if (CreateApp(a_name, a_width, a_height, a_bFullscreen))
	{
		Utility::ResetTimer();
		m_isRunning = true;
		do
		{
			float deltaTime = Utility::TickTimer();

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			Update(deltaTime);

			Draw();

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


			glfwSwapBuffers(m_window.pWindow);
			glfwPollEvents();

		} while (m_isRunning == true && glfwWindowShouldClose(m_window.pWindow) == 0);

		Destroy();
	}
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(m_window.pWindow);
	glfwTerminate();
}