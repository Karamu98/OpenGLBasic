#ifndef __UTILITIES_H_
#define __UTILITIES_H_

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
	static void GetGLErrors();

private:
	static const char* GetGLErrorStr(unsigned int a_error);

};

#endif // __UTILITIES_H_