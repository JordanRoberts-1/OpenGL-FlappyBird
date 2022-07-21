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

#include <algorithm>

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
		Entity* player = AddObject(std::move(playerEntity));
		PlayerComponent* playerComponent = player->AddComponent<PlayerComponent>(player);
		PhysicsComponent* playerPhysics = player->AddComponent<PhysicsComponent>(player);
		BoxColliderComponent* playerCollider = player->AddComponent<BoxColliderComponent>(player);
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

		//States: Height off ground, distance to center of pipe gap
		//Actions: Jump or don't jump
		aiComponent->SetStateSize(2);
		aiComponent->SetActionSize(2);
	}
	catch (...) { std::cerr << "Failed to make objects"; }

	//Initialize the entities
	for (const auto& entity : m_Objects)
	{
		entity->Init();
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

Entity* SceneManager::AddObject(std::unique_ptr<Entity> object)
{
	object->Init();

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
	m_Objects.clear();
	m_IDsToRemove.clear();
	m_CurrentSceneID = 0;
}
