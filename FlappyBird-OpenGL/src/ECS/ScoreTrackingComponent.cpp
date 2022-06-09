#include "ScoreTrackingComponent.h"
#include "Entity.h"
#include "TransformComponent.h"
#include "../Score.h"

ScoreTrackingComponent::ScoreTrackingComponent(Entity* parent)
	: Component(parent), m_HasScored(false)
{

}

void ScoreTrackingComponent::Init()
{
}

void ScoreTrackingComponent::Update()
{
	if (!m_HasScored && m_Parent->GetTransform()->GetPosition().x < 100.0f)
	{
		Score::IncrementScore();
		m_HasScored = true;
	}
}
