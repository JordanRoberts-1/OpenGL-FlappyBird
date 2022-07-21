#include "DQNAgentComponent.h"

#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "BoxColliderComponent.h"
#include "Entity.h"
#include "../Application.h"
#include "../Util/Random.h"
#include "../Game/SceneManager.h"

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
		Done();
	}

	Eigen::VectorXf currentState(m_StateSize);
	currentState[0] = m_TransformComponent->GetPosition().y;
	
	SceneManager& sc = SceneManager::GetInstance();
	glm::vec2 nearestPipe = sc.GetNearestPipeGap();
	currentState[1] = nearestPipe.x;
	currentState[2] = nearestPipe.y;

	//Set the new values for the previous updates data
	m_CurrentMemory.nextState = currentState;
	m_totalReward += m_CurrentMemory.reward;
	Remember(m_CurrentMemory);

	//reset the memory and start this current frames data
	m_CurrentMemory = MemorySlice();
	m_CurrentMemory.state = currentState;
	m_CurrentMemory.reward = 1;
	m_CurrentMemory.action = Act(currentState);

	if (m_CurrentMemory.action = 0) m_PhysicsComponent->Jump();

	////Temp policy
	//if (Random::Random01() > 0.95f)
	//{
	//	m_PhysicsComponent->Jump();
	//}
}


ComponentType DQNAgentComponent::GetType() const
{
	return DQNAGENTCOMPONENT;
}

void DQNAgentComponent::OnCollision(BoxColliderComponent* other)
{
	Done();
}

void DQNAgentComponent::Done()
{
	Application& app = Application::GetInstance();
	app.SetResetBool(true);
	m_CurrentMemory.done = true;
	m_CurrentMemory.reward = -10;

	std::cout << "Episode: " << app.GetEpisodeCount() << ", Reward: " << m_totalReward << std::endl;
}
void DQNAgentComponent::Remember(const MemorySlice& memory)
{
	if (m_Memory.size() < MEMORY_MAX)
	{
		m_Memory.push_back(memory);
	}
	else
	{
		m_Memory.pop_front();
		m_Memory.push_back(memory);
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

void DQNAgentComponent::Replay(int batchSize = 32)
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