#pragma once
#include "Component.h"

class TransformComponent;

class PhysicsComponent :
	public Component
{
public:
	PhysicsComponent(Entity* parent);
	void Update();
	inline ComponentType GetType() const { return PHYSICSCOMPONENT; }
private:
	TransformComponent* m_TransformComponent;

	const float GRAVITY = .5f;

	float m_Velocity;
	float m_Acceleration;
};

