#pragma once
#include "Component.h"
#include "../ML/NeuralNetwork.h"
#include <deque>

class TransformComponent;
class PhysicsComponent;
class BoxColliderComponent;

struct MemorySlice
{
	Eigen::VectorXf state;
	int action;
	float reward;
	Eigen::VectorXf nextState;
	bool done;

	MemorySlice() = default;

	MemorySlice(const Eigen::VectorXf& state, int action, float reward,
		const Eigen::VectorXf nextState, bool done)
		: state(state), action(action), reward(reward), nextState(nextState), done(done)
	{
	}
};

class DQNAgentComponent :
	public Component
{
public:
	DQNAgentComponent(Entity* parent);

	// Inherited via Component
	virtual void Init() override;
	virtual void Update() override;
	virtual ComponentType GetType() const override;

	void SetStateSize(int stateSize) { m_StateSize = stateSize; }
	void SetActionSize(int actionSize) { m_ActionSize = actionSize; }
	void SetResetBool(bool value) { m_ResetBool = value; }

	void OnCollision(BoxColliderComponent* other);
	void Done();

	void Remember(const MemorySlice& memory);

	int Act(const Eigen::VectorXf state);
	float Replay(int batchSize = 64);

	void Reset();

	void RenderUI();
	
	void SaveWeights(const std::string& fileName);
	void LoadWeights(const std::string& fileName);


private:
	TransformComponent* m_TransformComponent;
	PhysicsComponent* m_PhysicsComponent;
	BoxColliderComponent* m_BoxColliderComponent;

	bool m_ResetBool = true;
	int m_Steps = 0;

	int m_StateSize, m_ActionSize;
	const float GAMMA = 0.99f;
	float m_Epsilon = 1.0f;
	float m_TotalReward = 0;
	MemorySlice m_CurrentMemory;
	const float EPSILON_DECAY = 0.999995f;
	const float EPSILON_MIN = 0.025f;

	const float STARTING_LEARNING_RATE = 0.5f;
	const float LEARNING_RATE_DECAY = 0.001f;

	NeuralNetwork m_QNetwork;
	NeuralNetwork m_TargetNetwork;
	std::deque<MemorySlice> m_Memory;
	const int MEMORY_MAX = 50000;
	Optimizer_SGD m_Optimizer;

	//DEBUG
	int m_NumJumpsFromNN = 0;
	int m_NumNonJumpsFromNN = 0;
	float m_LastLoss = 0.0f;
	int m_EpisodeNum = 0;
	float m_LastReward = 0.0f;
	int m_WeightFileNumber = 0;
	std::string m_SaveString { "Weights.txt" };
	std::string m_LoadString { "Weights.txt" };

private:
	void CopyNN(NeuralNetwork& source, NeuralNetwork& dest);
};

