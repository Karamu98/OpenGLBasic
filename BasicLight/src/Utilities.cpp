#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <Windows.h>
#include <fstream>

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

std::shared_ptr<Shape> Utility::InitShape()
{
	std::cout << "\n1. Pyramid\n2. Cube\n";

	int option = 0;

	// Wait until we pick a valid option
	while (true)
	{
		std::cin >> option;
		switch (option)
		{
		case 1:
		{
			return std::make_shared<Pyramid>();
			break;
		}
		case 2:
		{
			return std::make_shared<Cube>();
			break;
		}
		default:
		{
			break;
		}
		}
	}
	return nullptr;
}

std::string Utility::OpenFileDialog(GLFWwindow* a_window, const std::string& a_filter)
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

std::shared_ptr<std::string> Utility::ReadFile(const std::string& a_file)
{
	// Read in the file
	std::shared_ptr<std::string> result = std::make_shared<std::string>();
	std::ifstream in;
	in.open(a_file, std::ios::in, std::ios::binary);
	if (in.is_open())
	{
		// Grab the size of the file
		in.seekg(0, std::ios::end);
		result->resize(in.tellg());

		// Seek to the beginning and read it all
		in.seekg(0, std::ios::beg);
		in.read(&(*result)[0], result->size());
		in.close();
		return result;
	}
	else
	{
		std::cout << "Couldn't open file " << a_file << std::endl;
		return nullptr;
	}
}

std::string Utility::GetWorkingDir()
{
	char path[MAX_PATH] = "";
	GetModuleFileNameA(NULL, path, MAX_PATH);

	std::string dir(path);
	dir = dir.substr(0, dir.find_last_of("\\/"));
	return dir + '\\';
}

void Utility::GetGLErrors()
{
	while (true)
	{
		const GLenum err = glGetError();
		if (GL_NO_ERROR == err)
			break;

		std::cout << "GL Error: " << GetGLErrorStr(err) << std::endl;
	}
}

const char* Utility::GetGLErrorStr(unsigned int a_error)
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