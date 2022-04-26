#include "Renderer.h"

//Clear the list of errors
void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

//Checks for error in a previous function call
bool GLLogCall(const char* function, const char* file, int line)
{
	//If we find an error, output it tot the console
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << error << ")" << function << " " << file << ":" << line << std::endl;
		return false; //There was an error
	}
	return true; //There was no error
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader)
{
	shader.Bind();

	//Bind all of the buffers and stuff
	va.Bind();
	ib.Bind();

	GLCall(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr));
}

void Renderer::Clear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
}