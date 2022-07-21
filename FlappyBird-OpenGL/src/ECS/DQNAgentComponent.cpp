#include "DQNAgentComponent.h"

#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "BoxColliderComponent.h"
#include "Entity.h"
#include "../Application.h"
#include "../Util/Random.h"

DQNAgentComponent::DQNAgentComponent(Entity* parent)
	: Component(parent), m_TransformComponent(nullptr),
	m_PhysicsComponent(nullptr), 
	m_BoxColliderComponent(nullptr),
	m_StateSize(1), m_ActionSize(1), m_NN(),
	m_Memory(), m_Optimizer(LEARNING_RATE)
{
}

void DQNAgentComponent::Init()
{
	m_TransformComponent = (TransformComponent*)m_Parent->GetTransform();
	m_PhysicsComponent = (PhysicsComponent*)m_Parent->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT);
	m_BoxColliderComponent = (BoxColliderComponent*)m_Parent->GetComponent<BoxColliderComponent>(BOXCOLLIDERCOMPONENT);
	std::function<void(BoxColliderComponent*)> fun = [this](BoxColliderComponent* other) {
		this->OnCollision(other);
	};
	m_BoxColliderComponent->SubscribeOnCollision(fun);

	//Jump when created
	m_PhysicsComponent->Jump();

	m_NN.AddLayer(m_StateSize, 16);
	m_NN.AddLayer(16, 16);
	m_NN.AddLayer(16, m_ActionSize);
}

void DQNAgentComponent::Update()
{
	Application& app = Application::GetInstance();
	if (m_TransformComponent->GetPosition().y > app.GetWindowHeight())
	{
		app.SetResetBool(true);
	}

	if (Random::Random01() > 0.01f)
	{
		m_PhysicsComponent->Jump();
	}
}

ComponentType DQNAgentComponent::GetType() const
{
	return DQNAGENTCOMPONENT;
}

void DQNAgentComponent::OnCollision(BoxColliderComponent* other)
{
	Application::GetInstance().SetResetBool(true);
}

void DQNAgentComponent::Remember(const Eigen::VectorXf& state, int action, float reward, const Eigen::VectorXf& nextState, bool done)
{
	if (m_Memory.size() < MEMORY_MAX)
	{
		m_Memory.emplace_back(state, action, reward, nextState, done);
	}
	else
	{
		m_Memory.pop_front();
		m_Memory.emplace_back(state, action, reward, nextState, done);
	}
}

int DQNAgentComponent::Act(const Eigen::VectorXf state)
{
	if (Random::Random01() <= m_Epsilon)
	{
		//0 to 1
		return Random::Random0n(m_ActionSize - 1);
	}

	int action = m_NN.Predict(state);
	return action;
}

void DQNAgentComponent::Replay(int batchSize)
{
	std::vector<MemorySlice> minibatch;
	std::sample(m_Memory.begin(), m_Memory.end(),
		std::back_inserter(minibatch), batchSize,
		std::mt19937{ std::random_device{}() });

	for (auto& memory : minibatch)
	{
		float target = memory.reward;

		if (!memory.done)
		{
			target = (memory.reward + GAMMA * m_NN.Predict(memory.nextState));
		}

		Eigen::VectorXf targetQs = m_NN.GetQs(memory.state);
		//targetQs.
		targetQs[memory.action] = target;

		m_NN.Fit(memory.state, targetQs, m_Optimizer);
	}

	if (m_Epsilon > EPSILON_MIN) m_Epsilon *= EPSILON_DECAY;
}