#include "PipeComponent.h"

#include "Entity.h"
#include "PhysicsComponent.h"

PipeComponent::PipeComponent(Entity* parent)
	: Component(parent), m_PhysicsComponent(nullptr)
{
	m_PhysicsComponent = m_Parent->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT);
}

void PipeComponent::Init()
{
	//m_PhysicsComponent->SetVelocity(glm::vec2(-1.0f, 0.0f));
}
