#include "PipeComponent.h"

#include "Entity.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "../Game/SceneManager.h"

PipeComponent::PipeComponent(Entity* parent)
	: Component(parent), m_PhysicsComponent(nullptr)
{
	m_PhysicsComponent = m_Parent->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT);
}

void PipeComponent::Init()
{
	m_PhysicsComponent->SetVelocity(PIPE_VELOCITY);
}

void PipeComponent::Update()
{
	if (m_Parent->GetTransform()->GetPosition().x < MIN_X)
	{
		SceneManager::GetInstance().RemoveObject(m_Parent->GetSceneID());
		std::cout << "Deleting" << std::endl;
	}
}
