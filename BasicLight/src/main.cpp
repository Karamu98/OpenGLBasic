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


float gammaCorrection = 1.8;
bool bIsWireframe = false;
float fSpecular = 16.0f;

int main()
{	
	GLFWwindow* window = Utility::OpenGLInit(1920, 1080, "Basic Lighting", false);
	if (window == nullptr)
	{
		return -1;
	}

	Utility::InitImGui(window);
	Utility::ResetTimer();

	glEnable(GL_DEPTH_TEST);
	if (bIsWireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}

	// Creating the FBO
	unsigned int defaultFBO;
	glGenFramebuffers(1, &defaultFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);

	// Create a color attachment texture
	unsigned int defaultColourTex;
	glGenTextures(1, &defaultColourTex);
	glBindTexture(GL_TEXTURE_2D, defaultColourTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1920, 1080, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, defaultColourTex, 0);

	// Create a renderbuffer object for depth attachment (not a texture because we're not sampling)
	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 1920, 1080); // Create the depth render buffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo); // Attach the render buffer object to the FBO depth slot

	// Test that the framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// Create the user picked shape
	Shape* shape = Utility::InitShape();

	// Create shaders
	Shader simpleShader = Shader("resources/shaders/vertex.glsl", "resources/shaders/fragment.glsl");
	Shader lightShader = Shader("resources/shaders/lightVertex.glsl", "resources/shaders/lightFragment.glsl");

	// Create the user camera
	Camera cam = Camera();

	// Creating textures
	Texture newTexture = Texture("resources/textures/test.jpg");

	// Setting up the light 
	Cube light = Cube();
	light.SetPosition(glm::vec3(0, 2, -5));
	light.Scale(glm::vec3(0.2f, 0.2f, 0.2f));
	glm::vec3 lightColour = glm::vec3(1, 0.5f, 1);

	simpleShader.Bind();
	simpleShader.SetInt("gMaterial.texture", 0);

	float deltaTime = 0;
	unsigned int shaderProgram = simpleShader.GetProgramID();
	unsigned int lightProgram = lightShader.GetProgramID();

	while (glfwWindowShouldClose(window) == 0)
	{
		// Set up Dear ImGui for a new frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Draw the scene to our FBO
		glBindFramebuffer(GL_FRAMEBUFFER, defaultFBO);
		Utility::TickTimer();
		deltaTime = Utility::GetDeltaTime();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		/// DRAW
		simpleShader.Bind();
		simpleShader.SetVec3("gLight.pos", light.GetPos());
		simpleShader.SetVec3("gLight.colour", lightColour);
		simpleShader.SetFloat("gMaterial.spec", fSpecular);
		simpleShader.SetFloat("gGamma", gammaCorrection);
		newTexture.Bind(GL_TEXTURE0);
		cam.Draw(shaderProgram);
		shape->Draw(shaderProgram);
		simpleShader.Unbind();

		lightShader.Bind();
		lightShader.SetVec3("gLightColour", lightColour);
		cam.Draw(lightProgram);
		light.Draw(lightProgram);
		lightShader.Unbind();

		/// UPDATE
		shape->Rotate(deltaTime, glm::vec3(0, 1, 0));
		cam.Update(deltaTime);

		// Draw ImGui on the backbuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// IMGUI DRAW
#pragma region ImGui Draw
		ImTextureID texID;

#pragma region DockSpace
		static bool p_open = true;

		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// Dockspace
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
		ImGui::Begin("Viewport");
		auto viewportSize = ImGui::GetContentRegionAvail();
		//m_Framebuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		//m_FinalPresentBuffer->Resize((uint32_t)viewportSize.x, (uint32_t)viewportSize.y);
		//m_Camera.SetProjectionMatrix(glm::perspectiveFov(glm::radians(45.0f), viewportSize.x, viewportSize.y, 0.1f, 10000.0f));
		texID = (void*)(intptr_t)defaultColourTex;
		ImGui::Image(texID, viewportSize, { 0, 1 }, { 1, 0 });
		ImGui::End();
		ImGui::PopStyleVar();

#pragma endregion


		ImGui::Begin("Properties", 0, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar);

		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Light");
		ImGui::Separator();
		ImGui::DragFloat3("Position", glm::value_ptr((*light.GetMatrix())[3]), 0.1f);
		ImGui::ColorEdit3("Colour", glm::value_ptr(lightColour));
		ImGui::NewLine();

		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Material");
		ImGui::Separator();
		texID = (void*)(intptr_t)newTexture.GetID();

		if (ImGui::ImageButton(texID, { 80, 80 }, ImVec2(0, 1), ImVec2(1, 0)))
		{
			newTexture.Reload(Utility::OpenFile(window));
		}
		ImGui::SameLine();
		ImGui::Text("Texture");
		ImGui::DragFloat("Specularity", &fSpecular, 0.05f);
		ImGui::NewLine();

		ImGui::TextColored(ImVec4(0, 1, 0, 1), "Rendering");
		ImGui::Separator();
		if (ImGui::Checkbox("Wireframe", &bIsWireframe))
		{
			if (bIsWireframe)
			{
				glDisable(GL_CULL_FACE);
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
			else
			{
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
				glEnable(GL_CULL_FACE);
				glCullFace(GL_BACK);
			}
		}
		ImGui::DragFloat("Gamma", &gammaCorrection, 0.05f);
		ImGui::Separator();
		ImGui::Text("Application Average: %.3f ms/frame (%.1f FPS)", 1000.f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		if (ImGui::IsMousePosValid())
			ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
		else
			ImGui::Text("Mouse Position: <invalid>");

		ImGui::End();
		ImGui::End();

#pragma endregion
		
		/// IMGUI RENDER
		Utility::ImguiRender();

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
