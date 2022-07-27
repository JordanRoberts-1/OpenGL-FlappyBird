#include "DQNAgentComponent.h"

#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "BoxColliderComponent.h"
#include "Entity.h"
#include "../Application.h"
#include "../Util/Random.h"
#include "../Game/SceneManager.h"
#include "ImGUI/imgui_stdlib.h"

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

	Reset();

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

	//If we haven't just reset then continue as normal, otherwise skip updating the
	//previous frames data because there was no previous frame
	if (!m_ResetBool)
	{
		//Set the new values for the previous updates data
		m_CurrentMemory.nextState = currentState;
		m_TotalReward += m_CurrentMemory.reward;
		Remember(m_CurrentMemory);
	}
	else
	{
		m_ResetBool = false;
	}

	//reset the memory and start this current frames data
	m_CurrentMemory = MemorySlice();
	m_CurrentMemory.state = currentState;
	m_CurrentMemory.reward = 1;
	m_CurrentMemory.action = Act(currentState);

	if (m_CurrentMemory.action == 0) m_PhysicsComponent->Jump();

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

	std::cout << "Episode: " << app.GetEpisodeCount() << ", Reward: " << m_TotalReward << std::endl 
		<< "Epsilon: " << m_Epsilon << std::endl;
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
		int action = Random::Random0n(m_ActionSize - 1);
		std::cout << "Randomly selecting: " << action << std::endl;
		//0 to 1
		return action;
	}

	int action = m_NN.Predict(state);
	std::cout << "Using NN to choose: " << action << std::endl;
	return action;
}

void DQNAgentComponent::Replay(int batchSize = 44)
{
	if (m_Memory.size() <= batchSize) return;

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

		m_NN.Fit(memory.state.transpose(), targetQs.transpose(), m_Optimizer);
	}

	if (m_Epsilon > EPSILON_MIN) m_Epsilon *= EPSILON_DECAY;
}

void DQNAgentComponent::Reset()
{
	m_TransformComponent->SetPosition(glm::vec3(100.0f, 500.0f, 1.0f));
	m_PhysicsComponent->Jump();
	m_TotalReward = 0;
	m_ResetBool = true;
}

void DQNAgentComponent::RenderUI()
{
	ImGui::Begin("Training Info");
	ImGui::Text("Episode: %d", m_EpisodeNum);
	ImGui::Text("Epsilon: %f, Decay: %f, Min: %f", m_Epsilon, EPSILON_DECAY, EPSILON_MIN);
	ImGui::Text("Learning Rate: %f", LEARNING_RATE);
	ImGui::Text("Memory Size: %d/%d", m_Memory.size(), MEMORY_MAX);
	ImGui::Text("Num Jumps: %d, Num NON Jumps: %d", m_NumJumpsFromNN, m_NumNonJumpsFromNN);
	ImGui::Text("Last Reward: %f, Last Loss: %f", m_LastReward, m_LastLoss);

	if (ImGui::Button("Save Weights"))
	{
		SaveWeights(m_SaveString);
	}
	ImGui::SameLine();
	ImGui::InputText("Save Weights Input", &m_SaveString);

	if (ImGui::Button("Load Weights")) 
	{
		LoadWeights(m_LoadString);
	}
	ImGui::SameLine();
	ImGui::InputText("Load Weights Input", &m_LoadString);
	ImGui::End();
}

void DQNAgentComponent::SaveWeights(const std::string& fileName)
{
	m_NN.SaveWeights(fileName);
}

void DQNAgentComponent::LoadWeights(const std::string& fileName)
{
	m_NN.LoadWeights(fileName);
}
