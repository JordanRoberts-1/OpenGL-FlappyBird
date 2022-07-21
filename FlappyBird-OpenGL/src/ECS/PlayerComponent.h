#pragma once
#include "Component.h"
#include "glm/glm.hpp"

class Application;
class PhysicsComponent;
class TransformComponent;
class BoxColliderComponent;
struct GLFWwindow;

class PlayerComponent :
	public Component
{
public:
	PlayerComponent(Entity* parent);
	void Init();
	void Update();
	inline ComponentType GetType() const { return PLAYERCOMPONENT; }
	void OnCollision(BoxColliderComponent* other);

	void HandleEvent();

private:
	TransformComponent* m_TransformComponent;
	PhysicsComponent* m_PhysicsComponent;
	BoxColliderComponent* m_BoxColliderComponent;
};

