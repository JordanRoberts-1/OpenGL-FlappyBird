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

enum MenuChoice
{
	NONE,
	TRAIN,
	PLAY,
	AIPLAY
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

	void ShowMenu();
	void ShowMainMenu();
	void Run();
	void UserPlayLoop(double& prev, double& lag);
	void Train(double& prev, double& lag);
	inline GLFWwindow* GetWindow() const { return m_Window.get(); }
	inline float GetWindowWidth() const { return (float)m_WindowWidth; }
	inline float GetWindowHeight() const { return (float)m_WindowHeight; }
	inline int GetEpisodeCount() const { return m_EpisodeCount; }
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

	MenuChoice m_Choice;

	int m_EpisodeCount = 0;

	const int m_WindowWidth;
	const int m_WindowHeight;

	const double MS_PER_UPDATE = 16.66666666666666;
	const double TRAIN_MS_PER_UPDATE = 5.0f;
	const int NUM_EPISODES = 500;
};
