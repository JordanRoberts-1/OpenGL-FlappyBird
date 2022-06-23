#pragma once
#include "Component.h"

class PlayerComponent;

class DQNAgentComponent :
	public Component
{
	// Inherited via Component
	virtual void Init() override;
	virtual void Update() override;
	virtual ComponentType GetType() const override;

private:
	PlayerComponent* m_PlayerComponent;
};

