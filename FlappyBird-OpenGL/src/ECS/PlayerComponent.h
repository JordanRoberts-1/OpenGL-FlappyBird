#pragma once
#include "Component.h"

class Application;
class PhysicsComponent;
struct GLFWwindow;

class PlayerComponent :
	public Component
{
public:
	PlayerComponent(Entity* parent);
	void Init();
	void Update();
	inline ComponentType GetType() const { return PLAYERCOMPONENT; }

	void HandleEvent();

private:
	PhysicsComponent* m_PhysicsComponent;
};

