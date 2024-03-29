#include "SceneManager.h"

#include "ResourceManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../Rendering/Shader.h"
#include "../Rendering/Texture.h"
#include "../ECS/PhysicsComponent.h"
#include "../ECS/Entity.h"
#include "../ECS/PlayerComponent.h"
#include "../ECS/BoxColliderComponent.h"
#include "../ECS/DQNAgentComponent.h"
#include "../ECS/TransformComponent.h"

#include <algorithm>
#include "PipeGenerator.h"
#include "../ECS/Pipes.h"

void SceneManager::BuildPlayScene()
{
	ResourceManager& rm = ResourceManager::GetInstance();

	//Attempt to create the objects and add components
	try
	{
		//Setup background
		std::unique_ptr<Entity> background = std::make_unique<Entity>(std::string("background.png"), std::string("Basic.glsl"), BACKGROUND_POSITION, BACKGROUND_SCALE);
		AddObject(std::move(background));

		//Setup Ground plane
		std::unique_ptr<Entity> ground = std::make_unique<Entity>(std::string("ground.png"), std::string("Basic.glsl"), GROUND_POSITION, GROUND_SCALE);
		ground->AddComponent<BoxColliderComponent>(ground.get());
		AddObject(std::move(ground));

		//Setup player
		std::unique_ptr<Entity> playerEntity = std::make_unique<Entity>(std::string("flappy_bird.png"), std::string("Basic.glsl"), PLAYER_POSITION, PLAYER_SCALE);
		PlayerComponent* playerComponent = playerEntity->AddComponent<PlayerComponent>(playerEntity.get());
		PhysicsComponent* playerPhysics = playerEntity->AddComponent<PhysicsComponent>(playerEntity.get());
		BoxColliderComponent* playerCollider = playerEntity->AddComponent<BoxColliderComponent>(playerEntity.get());
		AddObject(std::move(playerEntity));

		PipeGenerator::SpawnPipes();
	}
	catch (...) { std::cerr << "Failed to make objects"; }

	//Initialize the entities
	for (const auto& entity : m_Objects)
	{
		entity->Init();
	}
}

void SceneManager::BuildTrainingScene()
{
	ResourceManager& rm = ResourceManager::GetInstance();

	//Attempt to create the objects and add components
	try
	{
		//Setup background
		std::unique_ptr<Entity> background = std::make_unique<Entity>(std::string("background.png"), std::string("Basic.glsl"), BACKGROUND_POSITION, BACKGROUND_SCALE);
		AddObject(std::move(background));

		//Setup Ground plane
		std::unique_ptr<Entity> ground = std::make_unique<Entity>(std::string("ground.png"), std::string("Basic.glsl"), GROUND_POSITION, GROUND_SCALE);
		ground->AddComponent<BoxColliderComponent>(ground.get());
		AddObject(std::move(ground));

		//Setup player
		std::unique_ptr<Entity> trainingEntity = std::make_unique<Entity>(std::string("flappy_bird.png"), std::string("Basic.glsl"), PLAYER_POSITION, PLAYER_SCALE);
		Entity* ai = AddObject(std::move(trainingEntity));
		DQNAgentComponent* aiComponent = ai->AddComponent<DQNAgentComponent>(ai);
		PhysicsComponent* playerPhysics = ai->AddComponent<PhysicsComponent>(ai);
		BoxColliderComponent* playerCollider = ai->AddComponent<BoxColliderComponent>(ai);

		//States: Height off ground, position of center of pipe gap
		//Actions: Jump or don't jump
		aiComponent->SetStateSize(4);
		aiComponent->SetActionSize(2);

		PipeGenerator::SpawnPipes();
	}
	catch (...) { std::cerr << "Failed to make objects"; }

	//Initialize the entities
	for (const auto& entity : m_Objects)
	{
		entity->Init();
	}

	PipeGenerator::MovePipeSet();
}

DQNAgentComponent& SceneManager::GetAgent() const
{
	for (const auto& object : m_Objects)
	{
		DQNAgentComponent* agent = object->GetComponent<DQNAgentComponent>(DQNAGENTCOMPONENT);
		if (agent)
			return *agent;
	}
}

std::vector<BoxColliderComponent*> SceneManager::GetColliders() const
{
	std::vector<BoxColliderComponent*> result;

	//Loop through all the entities and add any found box colliders to the returned list
	for (const auto& object : m_Objects)
	{
		BoxColliderComponent* boxCollider = object->GetComponent<BoxColliderComponent>(BOXCOLLIDERCOMPONENT);
		if (boxCollider) result.push_back(boxCollider);
	}

	return result;
}

glm::vec2 SceneManager::GetNearestPipeGap() const
{
	glm::vec2 closest = glm::vec2(std::numeric_limits<float>::max());
	std::cout << "Getting nearest Pipe" << std::endl;
	for (const auto& object : m_Objects)
	{
		TopPipeComponent* pipe = object->GetComponent<TopPipeComponent>(TOPPIPECOMPONENT);

		if (!pipe)
			continue;

		glm::vec2 gapPosition = pipe->GetGapPosition();
		if (gapPosition.x > 100.0f && gapPosition.x < closest.x)
		{
			closest = pipe->GetGapPosition();
		}
	}
	return closest;
}

Entity* SceneManager::AddObject(std::unique_ptr<Entity> object)
{
	//object->Init();

	//Set the scene ID for each object
	object->SetSceneID(m_CurrentSceneID);
	m_CurrentSceneID++;

	m_Objects.push_back(std::move(object));
	return m_Objects.back().get();
}

void SceneManager::RemoveObject(int ID)
{
	m_IDsToRemove.push_back(ID);
}

void SceneManager::CleanUpObjects()
{
	//Sort the ids then remove them starting from back to front
	std::sort(m_IDsToRemove.begin(), m_IDsToRemove.end());
	for (int i = m_IDsToRemove.size() - 1; i >= 0; --i)
	{
		m_Objects.erase(m_Objects.begin() + m_IDsToRemove[i]);
	}

	//Update each objects current SceneID
	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->SetSceneID(i);
	}

	//Set the next scene ID to be handed out
	m_CurrentSceneID = m_Objects.size();
}

void SceneManager::ResetCleanupVector()
{
	m_IDsToRemove.clear();
}

void SceneManager::ResetScene()
{
	for (const auto& object : m_Objects)
	{
		TopPipeComponent* pipe = object->GetComponent<TopPipeComponent>(TOPPIPECOMPONENT);
		if (pipe)
		{
			pipe->Reset();
		}

		PlayerComponent* player = object->GetComponent<PlayerComponent>(PLAYERCOMPONENT);
		if (player)
		{
			player->GetParent()->GetTransform()->SetPosition(PLAYER_POSITION);
			player->GetParent()->GetComponent<PhysicsComponent>(PHYSICSCOMPONENT)->Jump();
		}
	}
}

void SceneManager::ResetTrainingScene()
{
	DQNAgentComponent& agent = GetAgent();
	agent.Reset();

	for (const auto& object : m_Objects)
	{
		TopPipeComponent* pipe = object->GetComponent<TopPipeComponent>(TOPPIPECOMPONENT);

		if (pipe)
		{
			pipe->Reset();
		}
	}

	PipeGenerator::s_UpdateCounter = 1;
	PipeGenerator::MovePipeSet();
}
