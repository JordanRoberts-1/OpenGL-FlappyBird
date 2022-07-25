#pragma once
#include "Component.h"
#include "glm/glm.hpp"

class PhysicsComponent;
class TransformComponent;

class PipeComponent :
	public Component
{
public:
	PipeComponent(Entity* parent);

	virtual void Init();
	virtual void Update();
	virtual inline ComponentType GetType() const { return PIPECOMPONENT; };

	void SetGapPosition(glm::vec2 gapPosition);
	glm::vec2 GetGapPosition() const { return m_GapPosition; };
	void SetStartingX(float startingX) { m_StartingX = startingX; }
	void SetChildPipe(PipeComponent* child) { m_BottomPipe = child; m_IsTop = true; }
	void Reset();
	void Reset(glm::vec2 y);

private:
	PhysicsComponent* m_PhysicsComponent;
	TransformComponent* m_TransformComponent;

	glm::vec2 m_GapPosition = glm::vec2(0.0f);
	float m_StartingX;

	bool m_IsTop;
	PipeComponent* m_BottomPipe;

	const glm::vec2 PIPE_VELOCITY = glm::vec2(-5.0f, 0.0f);
	const float MIN_X = -100.0f;
};

