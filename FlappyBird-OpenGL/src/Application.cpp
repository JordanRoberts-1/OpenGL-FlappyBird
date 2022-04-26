#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderable.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

GLFWwindow* SetupWindow();

void ClearRendering();

void RenderGeometry(VertexArray& vao, IndexBuffer& ib, Shader& shader);

void RenderGUI();

int main(void)
{
	GLFWwindow* window = SetupWindow();
	if (window == nullptr) return -1;

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
	glm::mat4 mvp = proj;

	Shader shader("res/shaders/Basic.glsl");
	shader.Bind();
	shader.SetUniform4f("u_Color", 0.8f, 0.3f, 0.8f, 1.0f);
	shader.SetUniformMat4f("u_MVP", mvp);

	Texture texture("res/textures/dickbutt.png");
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	std::vector<float> vertices;
	std::vector<unsigned int> indices;
	std::vector<Renderable> quads;
	std::array<float, 16> quadVertices;

	for (size_t i = 0; i < 5; i++)
	{
		//int i = 0;
		float width = texture.GetWidth() / 5.0f;
		float height = texture.GetHeight() / 5.0f;
		Renderable quad(glm::vec2(width * i, height * i), glm::vec2(width, height));
		quadVertices = quad.getVertices();
		vertices.insert(vertices.end(), begin(quadVertices), end(quadVertices));

		indices.push_back(i * 4 + 0);
		indices.push_back(i * 4 + 1);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 2);
		indices.push_back(i * 4 + 3);
		indices.push_back(i * 4 + 0);

		quads.push_back(quad);
	}

	VertexArray vao;
	VertexBuffer vb(&vertices[0], vertices.size() * sizeof(float));
	vb.Bind();
	IndexBuffer ib(&indices[0], indices.size());

	VertexBufferLayout vbLayout;
	vbLayout.Push<float>(2);
	vbLayout.Push<float>(2);

	vao.AddBuffer(vb, vbLayout);

	//vao.Unbind();
	//vb.Unbind();
	//ib.Unbind();
	//shader.Unbind();

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		ClearRendering();

		RenderGeometry(vao, ib, shader);

		RenderGUI();

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}

void RenderGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void RenderGeometry(VertexArray& vao, IndexBuffer& ib, Shader& shader)
{
	Renderer::Draw(vao, ib, shader);
}

void ClearRendering()
{
	/* Render here */
	Renderer::Clear();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

GLFWwindow* SetupWindow()
{
	/* Initialize the library */
	if (!glfwInit())
		return nullptr;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return nullptr;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//turn on V-Sync
	glfwSwapInterval(1);

	//Initialize glew
	if (glewInit())
	{
		std::cout << "Error!" << std::endl;
	}

	//Print out the version of OpenGL to the console
	std::cout << glGetString(GL_VERSION) << std::endl;

	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
	GLCall(glEnable(GL_BLEND));

	ImGui::CreateContext();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init((char*)glGetString(GL_NUM_SHADING_LANGUAGE_VERSIONS));

	ImGui::StyleColorsDark();
	return window;
}