#include "PhysicsComponent.h"
#include "Entity.h"
#include "TransformComponent.h"

PhysicsComponent::PhysicsComponent(Entity* parent)
	: Component(parent), m_Velocity(0.0f), m_Acceleration(0.0f)
{
	m_TransformComponent = parent->GetTransform();
}

void PhysicsComponent::Update()
{
	m_Velocity += m_Acceleration;
	glm::vec2 currPos = m_TransformComponent->GetPosition();
	m_TransformComponent->SetPosition(currPos + m_Velocity);
}
