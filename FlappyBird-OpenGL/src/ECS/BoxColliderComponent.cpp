#include "BoxColliderComponent.h"
#include "Entity.h"
#include "TransformComponent.h"

BoxColliderComponent::BoxColliderComponent(Entity* parent)
	: Component(parent), m_Transform(nullptr)
{
}

void BoxColliderComponent::Init()
{
	m_Transform = m_Parent->GetComponent<TransformComponent>(TRANSFORMCOMPONENT);
}

void BoxColliderComponent::Update()
{
}

bool BoxColliderComponent::CollidesWith(BoxColliderComponent* other)
{
	glm::vec2 thisPosition = m_Transform->GetPosition();
	glm::vec2 thisWidth = m_Transform->GetScale();
	glm::vec2 otherPosition = other->GetParent()->GetTransform()->GetPosition();

	return false;
}
