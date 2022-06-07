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
	glm::vec2 thisSize = m_Transform->GetScaledSize();
	glm::vec2 otherPosition = other->GetParent()->GetTransform()->GetPosition();
	glm::vec2 otherSize = other->GetParent()->GetTransform()->GetScaledSize();

	//Check the bounds of each box against each other
	if (thisPosition.x + thisSize.x >= otherPosition.x &&
		thisPosition.x <= otherPosition.x + otherSize.x &&
		thisPosition.y + thisSize.y >= otherPosition.y &&
		thisPosition.y <= otherPosition.y + otherSize.y)
	{
		return true;
	}

	return false;
}
