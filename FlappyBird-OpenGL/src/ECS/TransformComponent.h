#pragma once
#include "Component.h"
#include "glm/glm.hpp"

class TransformComponent :
	public Component
{
public:
	TransformComponent(Entity* parent, glm::vec2 pos, glm::vec2 scale);
	void Init() {};
	void Update();
	inline ComponentType GetType() const { return TRANSFORMCOMPONENT; }

	inline glm::vec2 GetPosition() const { return m_Position; }
	inline glm::vec2 GetScale() const { return m_Scale; }

	void SetPosition(glm::vec2 pos);
	void SetScale(glm::vec2 scale);

private:
	glm::vec2 m_Position;
	glm::vec2 m_Scale;
};

