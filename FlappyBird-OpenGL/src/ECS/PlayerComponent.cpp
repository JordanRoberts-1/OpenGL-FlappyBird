#include "PlayerComponent.h"

#include "Entity.h"
#include "PhysicsComponent.h"
#include "BoxColliderComponent.h"
#include "../Application.h"

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

	//Jump when created
	Jump();
}

void PlayerComponent::Update()
{
}

void PlayerComponent::OnCollision(BoxColliderComponent* other)
{
	Application::GetInstance().SetResetBool(true);
}

void PlayerComponent::Jump()
{
	m_PhysicsComponent->SetVelocity(JUMP_VELOCITY);
}

void PlayerComponent::HandleEvent()
{
	Jump();
}
