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

void SceneManager::BuildScene()
{
	ResourceManager& rm = ResourceManager::GetInstance();

	//Attempt to create the objects and add components
	try
	{
		std::unique_ptr<Entity>& player = m_Objects.emplace_back(std::make_unique<Entity>(std::string("flappy_bird.png"), std::string("Basic.glsl"), glm::vec2(100.0f, 200.0f), glm::vec2(10.0f)));
		PlayerComponent* playerComponent = player->AddComponent<PlayerComponent>(player.get());
		PhysicsComponent* playerPhysics = player->AddComponent<PhysicsComponent>(player.get());
		BoxColliderComponent* playerCollider = player->AddComponent<BoxColliderComponent>(player.get());

		playerPhysics->SetMass(1.0f);

		std::unique_ptr<Entity>& test2 = m_Objects.emplace_back(
			std::make_unique<Entity>(std::string("flappybirdpipe.png"), std::string("Basic.glsl"), glm::vec2(115.0f, 115.0f), glm::vec2(5.0f))
		);
		test2->AddComponent<BoxColliderComponent>(test2.get());
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

void SceneManager::AddObject(std::unique_ptr<Entity> object)
{
	object->Init();
	m_Objects.push_back(std::move(object));
}
