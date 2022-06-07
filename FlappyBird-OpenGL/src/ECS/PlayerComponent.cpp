#include "PlayerComponent.h"

#include "Entity.h"
#include "../Application.h"
#include "PhysicsComponent.h"
#include "BoxColliderComponent.h"

PlayerComponent::PlayerComponent(Entity* parent)
	: Component(parent),
	m_PhysicsComponent(nullptr),
	m_BoxColliderComponent(nullptr)
{
}

void PlayerComponent::Init()
{

	//!Potential errors if components aren't found
	m_PhysicsComponent = (PhysicsComponent*)m_Parent->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT);
	m_BoxColliderComponent = (BoxColliderComponent*)m_Parent->GetComponent<BoxColliderComponent>(BOXCOLLIDERCOMPONENT);
	std::function<void(BoxColliderComponent*)> fun = [this](BoxColliderComponent* other) {
		this->OnCollision(other);
	};
	m_BoxColliderComponent->SubscribeOnCollision(fun);
}

void PlayerComponent::Update()
{
}

void PlayerComponent::OnCollision(BoxColliderComponent* other)
{
	std::cout << "This is being called! on entity:" << m_Parent << std::endl;
}

void PlayerComponent::HandleEvent()
{
	m_PhysicsComponent->SetVelocity(glm::vec2(0.0f, 20.0f));
}
