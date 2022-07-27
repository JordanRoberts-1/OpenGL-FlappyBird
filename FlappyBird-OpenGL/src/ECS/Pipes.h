#pragma once
#include "Component.h"
#include "glm/glm.hpp"

class PhysicsComponent;
class TransformComponent;

const glm::vec2 PIPE_VELOCITY = glm::vec2(-5.0f, 0.0f);
const float MIN_X = -100.0f;

class BottomPipeComponent :
	public Component
{
public:
	BottomPipeComponent(Entity* parent);

	virtual void Init();
	virtual void Update() {};
	virtual inline ComponentType GetType() const { return BOTTOMPIPECOMPONENT; };

	void SetLocation(glm::vec2 gapPosition);
	void Reset();

private:
	PhysicsComponent* m_PhysicsComponent;
	TransformComponent* m_TransformComponent;

	glm::vec2 m_GapPosition = glm::vec2(0.0f);
	float m_StartingX;
};

class TopPipeComponent : public Component
{
public:
	TopPipeComponent(Entity* parent);
	virtual void Init();
	virtual void Update();
	virtual inline ComponentType GetType() const { return TOPPIPECOMPONENT; }

	bool CheckReset() const;
	void ResetLocation();
	void Reset();
	void SetBottomPipe(BottomPipeComponent* bottom) { m_BottomPipe = bottom; }
	glm::vec2 GetGapPosition() const { return m_GapPosition; }

private:
	PhysicsComponent* m_PhysicsComponent;
	TransformComponent* m_TransformComponent;
	BottomPipeComponent* m_BottomPipe;

	glm::vec2 m_GapPosition;
	float m_StartingX;

	bool m_HasScored;
	const float SCORE_X = 100.0f;
};

