#include "TransformComponent.h"

TransformComponent::TransformComponent(Entity* parent, glm::vec2 pos, glm::vec2 scale)
	: Component(parent), m_Position(pos), m_Scale(scale)
{
}

void TransformComponent::Update()
{
	m_Position += 0.05;
}
