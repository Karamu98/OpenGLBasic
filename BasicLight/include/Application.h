#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Window.h"

class Application
{
public:
	Application();
	virtual ~Application() = default;

	bool CreateApp(const char* a_name, int a_width, int a_height, bool a_bFullscreen);
	void Run(const char* a_name, int a_width, int a_height, bool a_bFullscreen);
	void Quit() { m_isRunning = false; }
	
protected:

	virtual bool OnCreate() = 0;
	virtual void Update(float a_deltaTime) = 0;
	virtual void Draw() = 0;
	virtual void Destroy() = 0;

	AppWindow* m_window;
	bool m_isRunning;
};

#endif