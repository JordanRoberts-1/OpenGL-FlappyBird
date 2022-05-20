#include "Application.h"
#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

Application::Application() : m_isRunning(false) {
	m_Window = SetupWindow();
}

Application::~Application() {
	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}

void Application::Run() {
	m_isRunning = true;
	while (m_isRunning) {
		Renderer::ClearRendering();

		//RenderGeometry(vao, ib, shader);

		Renderer::RenderGUI();

		/* Swap front and back buffers */
		glfwSwapBuffers(m_Window.get());

		/* Poll for and process events */
		glfwPollEvents();

		if (glfwWindowShouldClose(m_Window.get())) m_isRunning = false;
	}
}

//int main(void)
//{
//	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
//	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
//	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
//	glm::mat4 mvp = proj;
//
//	Shader shader("res/shaders/Basic.glsl");
//	shader.Bind();
//	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
//	shader.SetUniformMat4f("u_MVP", mvp);
//
//	Texture texture("res/textures/dickbutt.png");
//	texture.Bind();
//	shader.SetUniform1i("u_Texture", 0);
//
//	std::vector<float> vertices;
//	std::vector<unsigned int> indices;
//	std::vector<Renderable> quads;
//	std::array<float, 16> quadVertices;
//
//	for (size_t i = 0; i < 5; i++)
//	{
//		//int i = 0;
//		float width = texture.GetWidth() / 5.0f;
//		float height = texture.GetHeight() / 5.0f;
//		Renderable quad(glm::vec2(width * i, height * i), glm::vec2(width, height));
//		quadVertices = quad.getVertices();
//		vertices.insert(vertices.end(), begin(quadVertices), end(quadVertices));
//
//		indices.push_back(i * 4 + 0);
//		indices.push_back(i * 4 + 1);
//		indices.push_back(i * 4 + 2);
//		indices.push_back(i * 4 + 2);
//		indices.push_back(i * 4 + 3);
//		indices.push_back(i * 4 + 0);
//
//		quads.push_back(quad);
//	}
//
//	VertexArray vao;
//	VertexBuffer vb(&vertices[0], vertices.size() * sizeof(float));
//	vb.Bind();
//	IndexBuffer ib(&indices[0], indices.size());
//
//	VertexBufferLayout vbLayout;
//	vbLayout.Push<float>(2);
//	vbLayout.Push<float>(2);
//
//	vao.AddBuffer(vb, vbLayout);
//
//
//
//
//	return 0;
//}

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
	std::unique_ptr<GLFWwindow, DestroyglfwWin> window = std::unique_ptr<GLFWwindow, DestroyglfwWin>(glfwCreateWindow(960, 540, "Hello World", NULL, NULL));
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window.get());

	//turn on V-Sync
	glfwSwapInterval(1);

	//Initialize glew
	if (glewInit())
	{
		std::cout << "Error!" << std::endl;
	}

	//Print out the version of OpenGL to the console
	std::cout << glGetString(GL_VERSION) << std::endl;

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	SetupImGui(window.get());

	Debug::SetupDebug();
	return window;
}