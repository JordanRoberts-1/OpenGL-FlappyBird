#pragma once
#include "Component.h"
#include "glm/glm.hpp"

class TransformComponent;

class PhysicsComponent :
	public Component
{
public:
	PhysicsComponent(Entity* parent);
	void Init();
	void Update();
	inline ComponentType GetType() const { return PHYSICSCOMPONENT; }

	void AddForce(glm::vec2 force);
	void SetMass(float mass);
	void SetVelocity(glm::vec2 velocity);
	void SetAcceleration(glm::vec2 acceleration);
	void SetBoolGravity(bool value);
private:
	TransformComponent* m_TransformComponent;

	const float GRAVITY = -1.0f;
	bool m_GravityAffects = true;

	float m_Mass;

	glm::vec2 m_CurrentForce;
	glm::vec2 m_Velocity;
	glm::vec2 m_Acceleration;
};

