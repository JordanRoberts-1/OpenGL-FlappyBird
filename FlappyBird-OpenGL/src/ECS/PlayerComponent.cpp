#include "PlayerComponent.h"

#include "Entity.h"
#include "../Application.h"
#include "PhysicsComponent.h"

PlayerComponent::PlayerComponent(Entity* parent)
	: Component(parent),
	m_PhysicsComponent((PhysicsComponent*)parent->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT))
{
}

void PlayerComponent::Init()
{
}

void PlayerComponent::Update()
{
}

void PlayerComponent::HandleEvent()
{
	std::cout << "Handling Event" << std::endl;
	m_PhysicsComponent->SetVelocity(glm::vec2(0.0f, 1.0f));
}
