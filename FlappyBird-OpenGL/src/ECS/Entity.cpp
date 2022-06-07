#include "Entity.h"
#include "../ResourceManager.h"
#include "TransformComponent.h"
#include "Component.h"

Entity::Entity(const std::string& texture, const std::string& shader, glm::vec2 pos, glm::vec2 scale)
	: m_SceneID(0), m_Transform(nullptr),
	m_Texture(ResourceManager::GetInstance().CreateTexture(texture)),
	m_Shader(ResourceManager::GetInstance().CreateShader(shader))
{
	glm::vec2 size;
	size.x = (float)m_Texture->GetWidth();
	size.y = (float)m_Texture->GetHeight();

	//Add the transform component to each entity
	m_Transform = (TransformComponent*)m_Components.emplace_back(std::make_unique<TransformComponent>(this, pos, size, scale)).get();
}

void Entity::SetSceneID(int id)
{
	m_SceneID = id;
}

int Entity::GetSceneID()
{
	return m_SceneID;
}

void Entity::Update()
{
	for (uint32_t i = 0; i < m_Components.size(); i++)
	{
		m_Components[i]->Update();
	}
}

void Entity::Init()
{
	for (const auto& component : m_Components)
	{
		component->Init();
	}
}

inline const std::vector<std::unique_ptr<Component>>& Entity::GetAllComponents() const { return m_Components; };

