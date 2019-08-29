#ifndef __UTILITIES_H_
#define __UTILITIES_H_

#include <glm/glm.hpp>

// Class declarations
class GLFWwindow;
class Shape;

// A utility class with static helper methods
class Utility
{
public:

	// Utilities for timing; Get() updates timers and returns time since last Get call
	static void		ResetTimer();
	static float	TickTimer();
	static float	GetDeltaTime();
	static float	GetTotalTime();

	static std::string OpenFile(GLFWwindow* a_window, const std::string& a_filter = "");

	static Shape* InitShape();

	// OpenGL
	static GLFWwindow* OpenGLInit(int a_windowWidth, int a_windowHeight, const char* a_windowName, bool a_isFullscreen);
	static void CheckGLErrors();

	// ImGui
	static void InitImGui(GLFWwindow* a_window);
	static void ImguiRender();

private:

	static const char* GetGLErrorStr(GLenum a_error);

};

#endif // __UTILITIES_H_