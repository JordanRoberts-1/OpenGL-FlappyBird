#include "Renderer.h"
#include "SceneManager.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "TexturedQuad.h"

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	shader.Bind();

	//Bind all of the buffers and stuff
	va.Bind();
	ib.Bind();

	glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr);
}

void Renderer::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::RenderGUI()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Renderer::RenderGeometry()
{
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));
	//glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(200, 200, 0));
	glm::mat4 mvp = proj;

	SceneManager& sc = SceneManager::GetInstance();
	const std::vector<TexturedQuad*> objects = sc.GetObjects();

	VertexBufferLayout vbLayout = VertexBufferLayout();
	vbLayout.Push<float>(2);
	vbLayout.Push<float>(2);

	unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
	IndexBuffer ib(indices, 6);

	for (const TexturedQuad* object : objects)
	{
		VertexArray vao;

		const std::array<float, 16>& vertices = BuildVertices(*object);
		VertexBuffer vb(&vertices, vertices.size() * sizeof(float));
		vb.Bind();

		vao.AddBuffer(vb, vbLayout);

		Shader& shader = object->GetShader();
		shader.SetUniformMat4f("u_MVP", mvp);

		Renderer::Draw(vao, ib, shader);
	}
}

void Renderer::ClearRendering()
{
	/* Render here */
	Renderer::Clear();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

std::array<float, 16> Renderer::BuildVertices(TexturedQuad object)
{
	std::array<float, 16> vertices;
	glm::vec2 position = object.GetPosition();
	glm::vec2 size = object.GetSize();

	//Build the vertice array from the given arguments
	vertices[0] = position.x;
	vertices[1] = position.y;
	vertices[2] = 0.0f;
	vertices[3] = 0.0f;

	vertices[4] = position.x + size.x;
	vertices[5] = position.y;
	vertices[6] = 1.0f;
	vertices[7] = 0.0f;

	vertices[8] = position.x + size.x;
	vertices[9] = position.y + size.y;
	vertices[10] = 1.0f;
	vertices[11] = 1.0f;

	vertices[12] = position.x;
	vertices[13] = position.y + size.y;
	vertices[14] = 0.0f;
	vertices[15] = 1.0f;

	return vertices;
}