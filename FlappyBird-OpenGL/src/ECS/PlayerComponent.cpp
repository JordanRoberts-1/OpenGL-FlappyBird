#include "PlayerComponent.h"

#include "Entity.h"
#include "PhysicsComponent.h"
#include "BoxColliderComponent.h"
#include "../Application.h"
#include "TransformComponent.h"

PlayerComponent::PlayerComponent(Entity* parent)
	: Component(parent),
	m_TransformComponent(nullptr),
	m_PhysicsComponent(nullptr),
	m_BoxColliderComponent(nullptr)
{
}

void PlayerComponent::Init()
{
	//!Potential errors if components aren't found
	m_TransformComponent = (TransformComponent*)m_Parent->GetTransform();
	m_PhysicsComponent = (PhysicsComponent*)m_Parent->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT);
	m_BoxColliderComponent = (BoxColliderComponent*)m_Parent->GetComponent<BoxColliderComponent>(BOXCOLLIDERCOMPONENT);
	std::function<void(BoxColliderComponent*)> fun = [this](BoxColliderComponent* other) {
		this->OnCollision(other);
	};
	m_BoxColliderComponent->SubscribeOnCollision(fun);

	//Jump when created
	m_PhysicsComponent->Jump();
}

void PlayerComponent::Update()
{
	Application& app = Application::GetInstance();
	if (m_TransformComponent->GetPosition().y > app.GetWindowHeight())
	{
		app.SetResetBool(true);
	}
}

void PlayerComponent::OnCollision(BoxColliderComponent* other)
{
	Application::GetInstance().SetResetBool(true);
}

void PlayerComponent::HandleEvent()
{
	m_PhysicsComponent->Jump();
}
