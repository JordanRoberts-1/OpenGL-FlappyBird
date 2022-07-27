#include "Pipes.h"
#include "PhysicsComponent.h"
#include "TransformComponent.h"
#include "Entity.h"
#include "../Game/Score.h"
#include "../Game/PipeGenerator.h"

TopPipeComponent::TopPipeComponent(Entity* parent)
	: Component(parent), m_HasScored(false), m_GapPosition(0.0f),
	m_StartingX(), m_PhysicsComponent(nullptr), m_TransformComponent(nullptr),
	m_BottomPipe(nullptr)
{

}

void TopPipeComponent::Init()
{
	m_PhysicsComponent = m_Parent->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT);
	m_TransformComponent = m_Parent->GetTransform();
	m_PhysicsComponent->SetVelocity(glm::vec2(PIPE_VELOCITY));

	//Move it out of the way so that it can be used later
	m_TransformComponent->SetPosition(glm::vec3(-2000.0f));
}

void TopPipeComponent::Update()
{
	glm::vec2 pos = m_TransformComponent->GetPosition();
	if (!m_HasScored && pos.x < SCORE_X)
	{
		Score::IncrementScore();
		m_HasScored = true;
	}

	m_GapPosition.x = pos.x;
}

bool TopPipeComponent::CheckReset() const
{
	bool result = m_TransformComponent->GetPosition().x < -m_TransformComponent->GetScaledSize().x;
	return result;
}

void TopPipeComponent::ResetLocation()
{
	glm::vec2 m_GapPosition = glm::vec2(PipeGenerator::PIPE_SPAWN_X, std::rand() % PipeGenerator::PIPE_RANDOM_SPAWN_Y + PipeGenerator::PIPE_OFFSET_Y);
	m_TransformComponent->SetPosition(glm::vec3(m_GapPosition.x, m_GapPosition.y + PipeGenerator::GAP_RADIUS, 1));

	if (!m_BottomPipe)
	{
		std::cerr << "YOU DIDN'T SET THE BOTTOM COMPONENT" << std::endl;
		return;
	}

	m_BottomPipe->SetLocation(m_GapPosition);
}

//When a round is over, just move this out of the way
void TopPipeComponent::Reset()
{
	m_TransformComponent->SetPosition(glm::vec3(-2000.0f));
	m_BottomPipe->Reset();
	m_HasScored = false;
}

//----------------Bottom Pipe-------------------------------------------

BottomPipeComponent::BottomPipeComponent(Entity* parent)
	: Component(parent), m_PhysicsComponent(nullptr), m_TransformComponent(nullptr),
	m_GapPosition(0.0f), m_StartingX(0.0f)
{
}

void BottomPipeComponent::Init()
{
	m_PhysicsComponent = m_Parent->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT);
	m_TransformComponent = m_Parent->GetTransform();
	m_PhysicsComponent->SetVelocity(glm::vec2(PIPE_VELOCITY));

	//Move it out of the way so that it can be used later
	m_TransformComponent->SetPosition(glm::vec3(-2000.0f));
}

void BottomPipeComponent::SetLocation(glm::vec2 gapPosition)
{
	glm::vec3 bottomPosition = glm::vec3(gapPosition.x, gapPosition.y - PipeGenerator::GAP_RADIUS - m_TransformComponent->GetScaledSize().y, 1);
	m_TransformComponent->SetPosition(bottomPosition);
}

void BottomPipeComponent::Reset()
{
	m_TransformComponent->SetPosition(glm::vec3(-2000.0f));
}
