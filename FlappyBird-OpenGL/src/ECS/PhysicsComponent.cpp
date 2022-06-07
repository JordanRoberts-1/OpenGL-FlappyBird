#include "PhysicsComponent.h"
#include "Entity.h"
#include "TransformComponent.h"

PhysicsComponent::PhysicsComponent(Entity* parent)
	: Component(parent), m_TransformComponent(nullptr), m_Mass(1.0f), m_CurrentForce(0.0f), m_Velocity(0.0f), m_Acceleration(0.0f)
{

}

void PhysicsComponent::Init()
{
	m_TransformComponent = m_Parent->GetTransform();

	//Add gravity if this object is affected by gravity
	if (m_GravityAffects)
	{
		AddForce(glm::vec2(0.0f, GRAVITY));
	}
}

void PhysicsComponent::Update()
{
	m_Acceleration = m_CurrentForce / m_Mass;

	m_Velocity += m_Acceleration;
	glm::vec2 currPos = m_TransformComponent->GetPosition();
	m_TransformComponent->SetPosition(currPos + m_Velocity);
}

void PhysicsComponent::AddForce(glm::vec2 force)
{
	m_CurrentForce += force;
}

void PhysicsComponent::SetMass(float mass)
{
	m_Mass = mass;
}

void PhysicsComponent::SetVelocity(glm::vec2 velocity)
{
	m_Velocity = velocity;
}

void PhysicsComponent::SetAcceleration(glm::vec2 acceleration)
{
	m_Acceleration = acceleration;
}

void PhysicsComponent::SetBoolGravity(bool value)
{
	m_GravityAffects = value;
}
