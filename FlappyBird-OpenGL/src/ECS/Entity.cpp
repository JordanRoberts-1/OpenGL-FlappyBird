#include "Entity.h"
#include "../ResourceManager.h"
#include "TransformComponent.h"

Entity::Entity(const std::string& texture, const std::string& shader, glm::vec2 pos)
	: m_SceneID(0), m_Transform(nullptr),
	m_Texture(ResourceManager::GetInstance().CreateTexture(texture)),
	m_Shader(ResourceManager::GetInstance().CreateShader(shader))
{
	glm::vec2 size;
	size.x = (float)m_Texture->GetWidth();
	size.y = (float)m_Texture->GetHeight();

	m_Transform = std::make_unique<TransformComponent>(this, pos, size);
}

void Entity::SetSceneID(int id)
{
	m_SceneID = id;
}

int Entity::GetSceneID()
{
	return m_SceneID;
}
