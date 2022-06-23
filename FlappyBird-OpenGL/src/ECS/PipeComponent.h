#pragma once
#include "Component.h"
#include "glm/glm.hpp"

class PhysicsComponent;

class PipeComponent :
	public Component
{
public:
	PipeComponent(Entity* parent);

	virtual void Init();
	virtual void Update();
	virtual inline ComponentType GetType() const { return PIPECOMPONENT; };

private:
	PhysicsComponent* m_PhysicsComponent;

	const glm::vec2 PIPE_VELOCITY = glm::vec2(-5.0f, 0.0f);
	const float MIN_X = -100.0f;
};

