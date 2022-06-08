#include "Application.h"
#include "SceneManager.h"
#include "Clock.h"

#include "Input.h"
#include "ECS/BoxColliderComponent.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "PipeGenerator.h"

Application::Application() : m_isRunning(false)
{
	m_Window = SetupWindow();
	SceneManager::GetInstance().BuildScene();
}

Application::~Application()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void Application::Run()
{
	m_isRunning = true;

	double prev = Clock::CurrTimeInMillis();
	double lag = 0.0;
	while (m_isRunning)
	{
		SceneManager::GetInstance().BuildScene();
		m_ShouldReset = false;

		while (!m_ShouldReset)
		{
			double curr = Clock::CurrTimeInMillis();
			double elapsed = curr - prev;
			prev = curr;
			lag += elapsed;

			//std::cout << "Frametime: " << elapsed << "ms" << std::endl;

			while (lag >= MS_PER_UPDATE)
			{
				Update();
				//std::cout << SceneManager::GetInstance().GetObjects().size() << std::endl;
				lag -= MS_PER_UPDATE;
			}

			Renderer::Render();

			/* Swap front and back buffers */
			glfwSwapBuffers(m_Window.get());

			/* Poll for and process events */
			glfwPollEvents();

			if (glfwWindowShouldClose(m_Window.get()))
			{
				m_ShouldReset = true;
				m_isRunning = false;
			}
		}
		SceneManager::GetInstance().ResetScene();
	}
}

void Application::SetResetBool(bool value)
{
	m_ShouldReset = value;
}

void Application::CreateContext()
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
}

void Application::SetupImGui(GLFWwindow* window)
{
	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	ImGui::StyleColorsDark();
}

std::unique_ptr<GLFWwindow, DestroyglfwWin> Application::SetupWindow()
{
	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	CreateContext();

	/* Create a windowed mode window and its OpenGL context */
	std::unique_ptr<GLFWwindow, DestroyglfwWin> window = std::unique_ptr<GLFWwindow, DestroyglfwWin>(glfwCreateWindow(540, 960, "Hello World", NULL, NULL));
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window.get());

	//turn on V-Sync
	glfwSwapInterval(0);

	//Initialize glew
	if (glewInit())
	{
		std::cout << "Error!" << std::endl;
	}

	//Print out the version of OpenGL to the console
	std::cout << glGetString(GL_VERSION) << std::endl;

	//Setup transparencies
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	SetupImGui(window.get());

	glfwSetKeyCallback(window.get(), InputManager::Callback);

	Debug::SetupDebug();
	return window;
}

void Application::Update()
{
	SceneManager& sc = SceneManager::GetInstance();
	sc.ResetCleanupVector();

	const std::vector<std::unique_ptr<Entity>>& objects = sc.GetObjects();

	//Update all the objects
	for (const std::unique_ptr<Entity>& entity : objects)
	{
		entity->Update();
	}

	PipeGenerator::Update();

	//Check for collisions
	const std::vector<BoxColliderComponent*> colliders = sc.GetColliders();

	for (uint32_t i = 0; i < colliders.size() - 1; i++)
	{
		for (uint32_t j = i + 1; j < colliders.size(); j++)
		{
			const auto collider = colliders[i];
			const auto otherCollider = colliders[j];

			if (collider->CollidesWith(otherCollider))
			{
				collider->OnCollision(otherCollider);
				otherCollider->OnCollision(collider);
			}
		}
	}

	sc.CleanUpObjects();
}

