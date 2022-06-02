#include "Entity.h"
#include "../ResourceManager.h"
#include "TransformComponent.h"
#include "Component.h"

Entity::Entity(const std::string& texture, const std::string& shader, glm::vec2 pos)
	: m_SceneID(0), m_Transform(nullptr),
	m_Texture(ResourceManager::GetInstance().CreateTexture(texture)),
	m_Shader(ResourceManager::GetInstance().CreateShader(shader))
{
	glm::vec2 size;
	size.x = (float)m_Texture->GetWidth();
	size.y = (float)m_Texture->GetHeight();

	//Add the transform component to each entity
	m_Transform = (TransformComponent*)m_Components.emplace_back(std::make_unique<TransformComponent>(this, pos, size)).get();
}

void Entity::SetSceneID(int id)
{
	m_SceneID = id;
}

int Entity::GetSceneID()
{
	return m_SceneID;
}

template <typename T>
T* Entity::GetComponent(ComponentType type)
{
	for (uint32_t i = 0; i < m_Components.size(); i++)
	{
		if (type == m_Components[i]->GetType()) return m_Components[i].get();
	}
}

void Entity::Update()
{
	for (uint32_t i = 0; i < m_Components.size(); i++)
	{
		m_Components[i]->Update();
	}
}

inline const std::vector<std::unique_ptr<Component>>& Entity::GetAllComponents() const { return m_Components; };

