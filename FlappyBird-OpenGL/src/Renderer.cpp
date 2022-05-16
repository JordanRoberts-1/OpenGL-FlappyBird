#include "Renderer.h"

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