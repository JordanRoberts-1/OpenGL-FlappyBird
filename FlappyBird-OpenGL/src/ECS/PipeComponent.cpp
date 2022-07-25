#include "PipeComponent.h"

#include "Entity.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "../Game/SceneManager.h"
#include "../Game/PipeGenerator.h"

PipeComponent::PipeComponent(Entity* parent)
	: Component(parent), m_PhysicsComponent(nullptr), m_BottomPipe(nullptr), m_IsTop(false)
{
}

void PipeComponent::Init()
{
	m_PhysicsComponent = m_Parent->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT);
	m_PhysicsComponent->SetVelocity(PIPE_VELOCITY);

	m_TransformComponent = m_Parent->GetTransform();
	Reset();
}

void PipeComponent::Update()
{
	if (m_Parent->GetTransform()->GetPosition().x < MIN_X)
	{
		SceneManager::GetInstance().RemoveObject(m_Parent->GetSceneID());
		std::cout << "Deleting" << std::endl;
	}

	m_GapPosition.x = m_Parent->GetTransform()->GetPosition().x;
}

void PipeComponent::SetGapPosition(glm::vec2 gapPosition)
{
	m_GapPosition = gapPosition;
	m_StartingX = gapPosition.x;
}

void PipeComponent::Reset()
{
	if (!m_IsTop) return;

	glm::vec2 gapPosition = glm::vec2(m_StartingX, std::rand() % PipeGenerator::PIPE_RANDOM_SPAWN_Y + PipeGenerator::PIPE_OFFSET_Y);
	m_TransformComponent->SetPosition(glm::vec3(gapPosition.x, gapPosition.y + PipeGenerator::GAP_RADIUS, 1));
	m_BottomPipe->Reset(gapPosition);
}

void PipeComponent::Reset(glm::vec2 yGap)
{
	glm::vec3 bottomPosition = glm::vec3(yGap.x, yGap.y - PipeGenerator::GAP_RADIUS - m_TransformComponent->GetScaledSize().y, 1);
	m_TransformComponent->SetPosition(bottomPosition);
}


