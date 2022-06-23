#pragma once
#include "Component.h"
#include "../ML/NeuralNetwork.h"
#include <deque>

class PhysicsComponent;
class BoxColliderComponent;

struct MemorySlice
{
	Eigen::VectorXf state;
	int action;
	float reward;
	Eigen::VectorXf nextState;
	bool done;

	MemorySlice(const Eigen::VectorXf& state, int action, float reward,
		const Eigen::VectorXf nextState, bool done)
		: state(state), action(action), reward(reward), nextState(nextState), done(done)
	{
	}
};

class DQNAgentComponent :
	public Component
{
	DQNAgentComponent(Entity* parent, int stateSize, int actionSize);

	// Inherited via Component
	virtual void Init() override;
	virtual void Update() override;
	virtual ComponentType GetType() const override;

	void OnCollision(BoxColliderComponent* other);

	void Remember(const Eigen::VectorXf& state, int action, float reward,
		const Eigen::VectorXf& nextState, bool done);

	int Act(const Eigen::VectorXf state);
	void Replay(int batchSize);


private:
	PhysicsComponent* m_PhysicsComponent;
	BoxColliderComponent* m_BoxColliderComponent;

	int m_StateSize, m_ActionSize;
	const float GAMMA = 0.95;
	float m_Epsilon = 1.0f;
	const float EPSILON_DECAY = 0.995f;
	const float EPSILON_MIN = 0.01f;

	const float LEARNING_RATE = 0.01f;

	NeuralNetwork m_NN;
	std::deque<MemorySlice> m_Memory;
	const int MEMORY_MAX = 2000;
	Optimizer_SGD m_Optimizer;
};

