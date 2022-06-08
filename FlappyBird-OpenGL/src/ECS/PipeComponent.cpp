#include "PipeComponent.h"

#include "Entity.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "../SceneManager.h"

PipeComponent::PipeComponent(Entity* parent)
	: Component(parent), m_PhysicsComponent(nullptr)
{
	m_PhysicsComponent = m_Parent->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT);
}

void PipeComponent::Init()
{
	m_PhysicsComponent->SetVelocity(glm::vec2(-10.0f, 0.0f));
}

void PipeComponent::Update()
{
	if (m_Parent->GetTransform()->GetPosition().x < -100.0f)
	{
		SceneManager::GetInstance().RemoveObject(m_Parent->GetSceneID());
	}
}
