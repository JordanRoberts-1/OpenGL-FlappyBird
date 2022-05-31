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
	SceneManager& sc = SceneManager::GetInstance();
	const std::vector<TexturedQuad*> objects = sc.GetObjects();

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));

	VertexBufferLayout vbLayout = VertexBufferLayout();
	vbLayout.Push<float>(2);
	vbLayout.Push<float>(2);

	unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
	IndexBuffer ib(indices, 6);

	for (const TexturedQuad* object : objects)
	{
		VertexArray vao;

		const float* vertices = m_TexturedQuadVertices;
		VertexBuffer vb(vertices, 16 * sizeof(float));

		vb.Bind();

		vao.AddBuffer(vb, vbLayout);

		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(object->GetPosition(), 0.0f));
		glm::mat4 model = glm::scale(transMatrix, glm::vec3(object->GetSize(), 1));
		glm::mat4 mvp = proj * view * model;

		Shader& shader = object->GetShader();
		shader.Bind();
		shader.SetUniformMat4f("u_MVP", mvp);

		Texture& texture = object->GetTexture();
		texture.Bind(texture.GetRendererID());
		shader.SetUniform1i("u_Texture", texture.GetRendererID());

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