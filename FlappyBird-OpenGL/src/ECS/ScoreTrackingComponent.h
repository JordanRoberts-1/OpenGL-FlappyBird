#pragma once
#include "Component.h"

class ScoreTrackingComponent : public Component
{
public:
	ScoreTrackingComponent(Entity* parent);
	void Init();
	void Update();
	ComponentType GetType() const
	{
		return SCORETRACKINGCOMPONENT;
	}

private:
	bool m_HasScored;
};
