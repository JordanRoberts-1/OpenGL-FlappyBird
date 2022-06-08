#pragma once
#include "Component.h"

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
};

