#include "Application.h"
#include "Game/SceneManager.h"
#include "Util/Clock.h"

#include "Util/Input.h"
#include "ECS/BoxColliderComponent.h"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"
#include "Game/PipeGenerator.h"
#include "Game/Score.h"
#include "Game/UI.h"

Application::Application()
	: m_isRunning(false), m_WindowWidth(540), m_WindowHeight(960), m_Choice(NONE), m_Lag(0.0), m_Prev(0.0)
{
	m_Window = SetupWindow();
}

Application::~Application()
{
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void Application::ShowMenu()
{
	ImGui::Begin("Main Menu");
	if (ImGui::Button("Train"))
	{
		m_Choice = TRAIN;
		SceneManager::GetInstance().BuildTrainingScene();
		m_Prev = Clock::CurrTimeInMillis();
		ImGui::End();
		return;
	}
	if (ImGui::Button("Play"))
	{
		m_Choice = PLAY;
		SceneManager::GetInstance().BuildPlayScene();
		m_Prev = Clock::CurrTimeInMillis();
		ImGui::End();
		return;
	}
	if (ImGui::Button("AI Plays"))
	{
		m_Choice = AIPLAY;
		ImGui::End();
		return;
	}
	ImGui::End();
}

void Application::ShowMainMenu()
{
	Renderer::ClearRendering();
	glfwPollEvents();

	ShowMenu();

	Renderer::RenderGUI();
	glfwSwapBuffers(Application::GetInstance().GetWindow());

	if (glfwWindowShouldClose(m_Window.get()))
	{
		m_isRunning = false;
	}
}

void Application::Run()
{
	m_isRunning = true;

	m_Prev = Clock::CurrTimeInMillis();
	m_Lag = 0.0;

	//Application loop
	while (m_isRunning)
	{
		if (m_Choice == NONE)
		{
			ShowMainMenu();
		}
		else if (m_Choice == TRAIN)
		{
			Train();
		}
		else if(m_Choice == PLAY)
		{
			UserPlayLoop();
		}
		else if (m_Choice == AIPLAY)
		{
			//todo: add ai playing ability
		}

	}
}

void Application::UserPlayLoop()
{
	Score::ResetScore();
	m_ShouldReset = false;

	//this is the loop for each round of the game
	while (!m_ShouldReset)
	{
		Renderer::ClearRendering();
		glfwPollEvents();

		double curr = Clock::CurrTimeInMillis();
		double elapsed = curr - m_Prev;
		m_Prev = curr;
		m_Lag += elapsed;

		//std::cout << "Frametime: " << elapsed << "ms" << std::endl;

		//Keep constant update time regardless of rendering speed
		while (m_Lag >= MS_PER_UPDATE)
		{
			Update();

			m_Lag -= MS_PER_UPDATE;
		}

		UI::RenderScore();

		Renderer::Render();

		if (glfwWindowShouldClose(m_Window.get()))
		{
			m_ShouldReset = true;
			m_isRunning = false;
		}
	}

	//This round is over, so reset
	SceneManager::GetInstance().ResetScene();
}

void Application::Train()
{
	Score::ResetScore();
	m_ShouldReset = false;

	//this is the loop for every frame
	while (!m_ShouldReset)
	{
		Renderer::ClearRendering();
		glfwPollEvents();

		double curr = Clock::CurrTimeInMillis();
		double elapsed = curr - m_Prev;
		m_Prev = curr;
		m_Lag += elapsed;

		//std::cout << "Frametime: " << elapsed << "ms" << std::endl;

		//Keep constant update time regardless of rendering speed
		while (!m_ShouldReset && m_Lag >= TRAIN_MS_PER_UPDATE)
		{
			Update();
			m_Lag -= TRAIN_MS_PER_UPDATE;
		}

		SceneManager::GetInstance().GetAgent().RenderUI();
		Renderer::Render();

		if (glfwWindowShouldClose(m_Window.get()))
		{
			m_ShouldReset = true;
			m_isRunning = false;
		}
	}

	m_EpisodeCount++;
	SceneManager::GetInstance().ResetTrainingScene();
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
	std::unique_ptr<GLFWwindow, DestroyglfwWin> window =
		std::unique_ptr<GLFWwindow, DestroyglfwWin>(glfwCreateWindow(m_WindowWidth, m_WindowHeight, "Hello World", NULL, NULL));

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

	glfwSetKeyCallback(window.get(), InputManager::Callback);
	
	SetupImGui(window.get());


	Debug::SetupDebug();
	return window;
}

void Application::Update()
{
	SceneManager& sc = SceneManager::GetInstance();
	sc.ResetCleanupVector();

	PipeGenerator::Update();

	const std::vector<std::unique_ptr<Entity>>& objects = sc.GetObjects();

	//Update all the objects
	for (const std::unique_ptr<Entity>& entity : objects)
	{
		entity->Update();
	}


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

	//defer deletion of objects so as to not mess up updates
	sc.CleanUpObjects();
}