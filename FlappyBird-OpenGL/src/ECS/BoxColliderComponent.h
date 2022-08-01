#pragma once
#include "Component.h"
#include <functional>
#include <vector>

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
	void OnCollision(BoxColliderComponent* other);

	void SubscribeOnCollision(std::function<void(BoxColliderComponent*)> fun)
	{
		m_Subscribers.push_back(fun);
	}

private:
	TransformComponent* m_Transform;
	std::vector<std::function<void(BoxColliderComponent*)>> m_Subscribers;
};

