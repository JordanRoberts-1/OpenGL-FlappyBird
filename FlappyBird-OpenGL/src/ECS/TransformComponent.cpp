#include "TransformComponent.h"

TransformComponent::TransformComponent(Entity* parent, glm::vec2 pos, glm::vec2 size, glm::vec2 scale)
	: Component(parent), m_Position(pos), m_Size(size), m_Scale(scale)
{
}

void TransformComponent::Update()
{
}

void TransformComponent::SetPosition(glm::vec2 pos)
{
	m_Position = pos;
}

void TransformComponent::SetScale(glm::vec2 scale)
{
	m_Scale = scale;
}
