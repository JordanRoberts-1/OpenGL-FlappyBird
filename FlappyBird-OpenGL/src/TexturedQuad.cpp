#include "TexturedQuad.h"
#include "ResourceManager.h"

TexturedQuad::TexturedQuad(const std::string& texture, const std::string& shader, glm::vec2 pos)
	: m_SceneID(0), m_Position(pos.x, pos.y), m_Size(0.0f, 0.0f),
	m_Texture(ResourceManager::GetInstance().CreateTexture(texture)),
	m_Shader(ResourceManager::GetInstance().CreateShader(shader))
{
	m_Size.x = (float)m_Texture->GetWidth();
	m_Size.y = (float)m_Texture->GetHeight();
}

void TexturedQuad::SetSceneID(int id)
{
	m_SceneID = id;
}

int TexturedQuad::GetSceneID()
{
	return m_SceneID;
}
