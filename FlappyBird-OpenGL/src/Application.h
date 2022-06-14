#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Rendering/Renderer.h"
#include "Util/Debug.h"

#include <iostream>

//Delete method for GLFWwindows due to them being forward declared in source
struct DestroyglfwWin
{
	void operator()(GLFWwindow* ptr)
	{
		glfwDestroyWindow(ptr);
	}
};

class Application
{
public:
	static Application& GetInstance()
	{
		static Application instance;

		return instance;
	}

	Application(Application const&) = delete;
	void operator=(Application const&) = delete;

	~Application();
	void Run();
	inline GLFWwindow* GetWindow() const { return m_Window.get(); }
	void SetResetBool(bool value);
private:
	Application();
	void CreateContext();
	void SetupImGui(GLFWwindow* window);
	std::unique_ptr<GLFWwindow, DestroyglfwWin> SetupWindow();
	void Update();

private:
	std::unique_ptr<GLFWwindow, DestroyglfwWin> m_Window;
	bool m_isRunning;
	bool m_ShouldReset;

	const double MS_PER_UPDATE = 16.66666666666666;
};
