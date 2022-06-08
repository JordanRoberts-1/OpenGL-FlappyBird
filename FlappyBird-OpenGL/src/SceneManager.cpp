#include "SceneManager.h"

#include "ResourceManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Texture.h"
#include "ECS/PhysicsComponent.h"
#include "ECS/Entity.h"
#include "ECS/PlayerComponent.h"
#include "ECS/BoxColliderComponent.h"

#include <algorithm>

void SceneManager::BuildScene()
{
	ResourceManager& rm = ResourceManager::GetInstance();

	//Attempt to create the objects and add components
	try
	{
		//Setup background
		std::unique_ptr<Entity> background = std::make_unique<Entity>(std::string("background.png"), std::string("Basic.glsl"), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec2(1.5f));
		AddObject(std::move(background));

		//Setup Ground plane
		std::unique_ptr<Entity> ground = std::make_unique<Entity>(std::string("ground.png"), std::string("Basic.glsl"), glm::vec3(0.0f, -50.0f, -1.0f), glm::vec2(0.4f));
		ground->AddComponent<BoxColliderComponent>(ground.get());
		AddObject(std::move(ground));


		//Setup player
		std::unique_ptr<Entity> playerEntity = std::make_unique<Entity>(std::string("flappy_bird.png"), std::string("Basic.glsl"), glm::vec3(100.0f, 500.0f, 1.0f), glm::vec2(3.0f));
		Entity* player = AddObject(std::move(playerEntity));
		PlayerComponent* playerComponent = player->AddComponent<PlayerComponent>(player);
		PhysicsComponent* playerPhysics = player->AddComponent<PhysicsComponent>(player);
		BoxColliderComponent* playerCollider = player->AddComponent<BoxColliderComponent>(player);
		playerPhysics->SetMass(1.0f);
		playerPhysics->SetVelocity(glm::vec2(0.0f, 20.0f));

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
	std::sort(m_IDsToRemove.begin(), m_IDsToRemove.end());
	for (int i = m_IDsToRemove.size() - 1; i >= 0; --i)
	{
		m_Objects.erase(m_Objects.begin() + m_IDsToRemove[i]);
	}

	for (int i = 0; i < m_Objects.size(); i++)
	{
		m_Objects[i]->SetSceneID(i);
	}

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
