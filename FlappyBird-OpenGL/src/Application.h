#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Debug.h"

#include <iostream>

//Delete method for GLFWwindows due to them being forward declared in source
struct DestroyglfwWin {
	void operator()(GLFWwindow* ptr) {
		glfwDestroyWindow(ptr);
	}
};

class Application {
public:
	Application();
	~Application();
	void Run();
private:
	void CreateContext();
	void SetupImGui(GLFWwindow* window);
	std::unique_ptr<GLFWwindow, DestroyglfwWin> SetupWindow();

private:
	std::unique_ptr<GLFWwindow, DestroyglfwWin> m_Window;
	bool m_isRunning;
};
