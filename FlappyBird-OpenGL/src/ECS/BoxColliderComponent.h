#pragma once
#include "Component.h"

class TransformComponent;

class BoxColliderComponent :
	public Component
{
public:
	BoxColliderComponent(Entity* parent);

	virtual void Init();
	virtual void Update();
	virtual inline ComponentType GetType() const { return BOXCOLLIDERCOMPONENT; };

	bool CollidesWith(BoxColliderComponent* other);

private:
	TransformComponent* m_Transform;
};

