#include "TexturedQuad.h"

TexturedQuad::TexturedQuad(glm::vec2 position, glm::vec2 size, const std::string& texturePath, Shader& shader)
	: m_SceneID(-1), m_Texture(texturePath), m_Shader(shader)
{
	BuildVertices(position, size);

	shader.Bind();
	m_Texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	shader.Unbind();
}

TexturedQuad::TexturedQuad(const std::string& texturePath, Shader& shader)
	: m_SceneID(-1), m_Texture(texturePath), m_Shader(shader)
{
	float width = m_Texture.GetWidth();
	float height = m_Texture.GetHeight();

	glm::vec2 position(0.0f, 0.0f);
	glm::vec2 size(width, height);

	BuildVertices(position, size);

	shader.Bind();
	m_Texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	shader.Unbind();
}

TexturedQuad::TexturedQuad(float scaleFactor, const std::string& texturePath, Shader& shader)
	: m_SceneID(-1), m_Texture(texturePath), m_Shader(shader)
{
	float width = m_Texture.GetWidth() * scaleFactor;
	float height = m_Texture.GetHeight() * scaleFactor;

	glm::vec2 position(0.0f, 0.0f);
	glm::vec2 size(width, height);

	BuildVertices(position, size);

	shader.Bind();
	m_Texture.Bind();
	shader.SetUniform1i("u_Texture", 0);

	shader.Unbind();
}

void TexturedQuad::BuildVertices(glm::vec2& position, glm::vec2& size)
{
	//Build the vertice array from the given arguments
	m_Vertices[0] = position.x;
	m_Vertices[1] = position.y;
	m_Vertices[2] = 0.0f;
	m_Vertices[3] = 0.0f;

	m_Vertices[4] = position.x + size.x;
	m_Vertices[5] = position.y;
	m_Vertices[6] = 1.0f;
	m_Vertices[7] = 0.0f;

	m_Vertices[8] = position.x + size.x;
	m_Vertices[9] = position.y + size.y;
	m_Vertices[10] = 1.0f;
	m_Vertices[11] = 1.0f;

	m_Vertices[12] = position.x;
	m_Vertices[13] = position.y + size.y;
	m_Vertices[14] = 0.0f;
	m_Vertices[15] = 1.0f;
}

void TexturedQuad::SetSceneID(int id)
{
	m_SceneID = id;
}

int TexturedQuad::GetSceneID()
{
	return m_SceneID;
}
