#pragma once
#include "Component.h"

class PhysicsComponent :
	public Component
{
public:
	void Update();
	inline ComponentType GetType() const { return PHYSICSCOMPONENT; }
private:
	const float GRAVITY = .5f;

	float m_Velocity;
	float m_Acceleration;
};

