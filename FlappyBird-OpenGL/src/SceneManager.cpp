#include "SceneManager.h"

#include "ResourceManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Texture.h"
#include "ECS/PhysicsComponent.h"
#include "ECS/Entity.h"
#include "ECS/PlayerComponent.h"

void SceneManager::BuildScene()
{
	ResourceManager& rm = ResourceManager::GetInstance();

	//Attempt to create the objects and add components
	try
	{
		std::unique_ptr<Entity>& player = m_Objects.emplace_back(std::make_unique<Entity>(std::string("dickbutt.png"), std::string("Basic.glsl"), glm::vec2(0.0f, 0.0f)));
		PhysicsComponent* playerPhysics = player->AddComponent<PhysicsComponent>(player.get());
		PlayerComponent* playerComponent = player->AddComponent<PlayerComponent>(player.get());
		playerPhysics->SetMass(1.0f);

		//std::unique_ptr<Entity>& test2 = m_Objects.emplace_back(std::make_unique<Entity>(std::string("dr_minion.jpg"), std::string("Basic.glsl"), glm::vec2(300.0f, 200.0f)));
	}
	catch (...) { std::cerr << "Failed to make objects"; }
}
