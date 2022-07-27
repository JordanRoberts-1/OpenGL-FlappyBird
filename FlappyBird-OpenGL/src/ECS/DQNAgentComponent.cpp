#include "DQNAgentComponent.h"

#include "TransformComponent.h"
#include "PhysicsComponent.h"
#include "BoxColliderComponent.h"
#include "Entity.h"
#include "../Application.h"
#include "../Util/Random.h"
#include "../Game/SceneManager.h"
#include "ImGUI/imgui_stdlib.h"

#include <algorithm>

DQNAgentComponent::DQNAgentComponent(Entity* parent)
	: Component(parent), m_TransformComponent(nullptr),
	m_PhysicsComponent(nullptr), 
	m_BoxColliderComponent(nullptr),
	m_StateSize(1), m_ActionSize(1), m_QNetwork(),
	m_Memory(), m_Optimizer(STARTING_LEARNING_RATE,LEARNING_RATE_DECAY)
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

	m_QNetwork.AddLayer(m_StateSize, 8);
	m_QNetwork.AddLayer(8, 4);
	m_QNetwork.AddLayer(4, m_ActionSize);

	m_TargetNetwork.AddLayer(m_StateSize, 8);
	m_TargetNetwork.AddLayer(8, 4);
	m_TargetNetwork.AddLayer(4, m_ActionSize);
}

void DQNAgentComponent::Update()
{
	Application& app = Application::GetInstance();
	if (m_TransformComponent->GetPosition().y > app.GetWindowHeight())
	{
		Done();
	}


	Eigen::VectorXf currentState(m_StateSize);
	currentState[0] = m_TransformComponent->GetPosition().y / 960.0f;

	SceneManager& sc = SceneManager::GetInstance();
	glm::vec2 nearestPipe = sc.GetNearestPipeGap();
	currentState[1] = nearestPipe.x / 975.0f;
	currentState[2] = nearestPipe.y / 960.0f;
	currentState[3] = m_PhysicsComponent->GetVelocity().y / 15.0f;

	//If we haven't just reset then continue as normal, otherwise skip updating the
	//previous frames data because there was no previous frame
	if (!m_ResetBool)
	{
		//Set the new values for the previous updates data
		m_CurrentMemory.nextState = currentState;
		m_TotalReward += m_CurrentMemory.reward;
		Remember(m_CurrentMemory);

		//Start this frames memory
		m_CurrentMemory = MemorySlice();

		//Update the model every 8 steps
		m_Steps++;
		if (m_Steps % 8 == 0)
		{
			//m_QNetwork.Print();
			Replay();
		}

		if (m_Steps % 100 == 0)
		{
			//copy the values to the target network
			CopyNN(m_QNetwork, m_TargetNetwork);
		}
	}
	else
	{
		//This is the first frame of the game so create a new memory starting now
		m_CurrentMemory = MemorySlice();
		m_ResetBool = false;
	}

	//start this current frames data
	m_CurrentMemory.state = currentState;
	m_CurrentMemory.reward = .1f;
	m_CurrentMemory.action = Act(currentState);

	if (m_CurrentMemory.action == 0)
	{
		m_PhysicsComponent->Jump();
	}
	//Temp policy
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
	m_CurrentMemory.reward = -5.0f;
	m_LastLoss = Replay();
	m_EpisodeNum = app.GetEpisodeCount();
	m_LastReward = m_TotalReward;

	std::cout << "Q values for state: " << m_CurrentMemory.state << ": " << m_QNetwork.GetQs(m_CurrentMemory.state) << std::endl;

	//if (m_EpisodeNum % 100 == 0)
	//{
	//	std::cout << "\n\n" << "EPISODE: " << m_EpisodeNum << "\n\n";
	//	m_QNetwork.Print();
	//}

	if (m_EpisodeNum % 2500 == 0)
	{
		std::string fileName = "Weights-Episode-" + std::to_string(app.GetEpisodeCount()) + ".txt";
		SaveWeights(fileName);
	}

	/*std::cout << "Episode: " << app.GetEpisodeCount() << ", Reward: " << m_TotalReward << std::endl
		<< "Epsilon: " << m_Epsilon << std::endl << "Loss: " << m_LastLoss << std::endl;*/

	Reset();
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
	//std::cout << "Q values for state: " << state << ": " << m_QNetwork.GetQs(state) << std::endl;

	if (Random::Random01() <= m_Epsilon)
	{
		int action = Random::Random0n(m_ActionSize - 1);
		//std::cout << "Randomly selecting: " << action << std::endl;
		//0 to 1
		return action;
	}

	int action = m_QNetwork.Predict(state);
	if (action == 0)
		m_NumJumpsFromNN++;
	else if (action == 1)
		m_NumNonJumpsFromNN++;
	return action;
}

float DQNAgentComponent::Replay(int batchSize)
{
	if (m_Memory.size() <= batchSize) return -1;

	//std::cout << "Replaying Memories" << std::endl;

	auto& rng = Random::RandomDevice();
	std::vector<MemorySlice> minibatch;
	std::sample(m_Memory.begin(), m_Memory.end(),
		std::back_inserter(minibatch), batchSize,
		std::mt19937{ rng()});

	std::shuffle(std::begin(minibatch), std::end(minibatch), rng);

	Eigen::MatrixXf currentStates(minibatch.size(),4);
	Eigen::MatrixXf newCurrentStates(minibatch.size(), 4);

	for (int i = 0; i < minibatch.size(); i++)
	{
		currentStates.row(i) = minibatch[i].state;
		newCurrentStates.row(i) = minibatch[i].nextState;
	}
	Eigen::MatrixXf currentQsList = m_QNetwork.GetQs(currentStates);
	Eigen::MatrixXf futureQsList = m_QNetwork.GetQs(newCurrentStates);

	Eigen::MatrixXf X(minibatch.size(), 4);
	Eigen::MatrixXf y(minibatch.size(), 2);
	for (int i = 0; i < minibatch.size(); i++)
	{
		MemorySlice memory = minibatch[i];
		float maxFutureQ = memory.reward;

		if (!memory.done)
		{
			maxFutureQ = (memory.reward + GAMMA * futureQsList.row(i).maxCoeff());
		}

		Eigen::VectorXf currentQs = currentQsList.row(i);
		currentQs[memory.action] = (1 - GAMMA) * currentQs[memory.action] + GAMMA * maxFutureQ;

		X.row(i) = memory.state;
		y.row(i) = currentQs;
	}

	//std::cout << "X: " << X << std::endl << "y: " << y << std::endl;

	m_QNetwork.Fit(X, y, m_Optimizer);
	float loss = m_QNetwork.CalculateLoss(y);

	if (m_Epsilon > EPSILON_MIN) m_Epsilon *= EPSILON_DECAY;
	

	//for (auto& memory : minibatch)
	//{
	//	/*std::cout << "state: " << memory.state << ", action: " << memory.action << ", reward: " << memory.reward
	//		<< "next State: " << memory.nextState << ", done: " << memory.done << std::endl;*/
	//	float target = memory.reward;

	//	if (!memory.done)
	//	{
	//		target = (memory.reward + GAMMA * m_QNetwork.Predict(memory.nextState));
	//	}

	//	Eigen::VectorXf targetQs = m_QNetwork.GetQs(memory.state);
	//	//targetQs.
	//	targetQs[memory.action] = target;

	//	m_QNetwork.Fit(memory.state.transpose(), targetQs.transpose(), m_Optimizer);
	//}

	return loss;
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
	ImGui::Text("Learning Rate: %f", m_Optimizer.GetLearningRate());
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
	m_QNetwork.SaveWeights(fileName);
}

void DQNAgentComponent::LoadWeights(const std::string& fileName)
{
	m_QNetwork.LoadWeights(fileName);
}

void DQNAgentComponent::CopyNN(NeuralNetwork& source, NeuralNetwork& dest)
{
	std::cout << "Copying weights to Target Network" << std::endl;

	for (int i = 0; i < source.GetNumLayers(); i++)
	{
		const Layer& sourceLayer = source.GetLayer(i);
		Layer& destLayer = dest.GetLayer(i);
		Eigen::MatrixXf weights = sourceLayer.GetWeights();
		destLayer.SetWeightMatrix(weights);
	}
}
