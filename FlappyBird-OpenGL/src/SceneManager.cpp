#include "SceneManager.h"

#include "ResourceManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Texture.h"
#include "ECS/PhysicsComponent.h"
#include "ECS/Entity.h"

void SceneManager::BuildScene()
{
	ResourceManager& rm = ResourceManager::GetInstance();

	//Attempt to create the objects and add components
	try
	{
		std::unique_ptr<Entity>& player = m_Objects.emplace_back(std::make_unique<Entity>(std::string("dickbutt.png"), std::string("Basic.glsl"), glm::vec2(0.0f, 0.0f)));
		player->AddComponent<PhysicsComponent>(player.get());

		std::unique_ptr<Entity>& test2 = m_Objects.emplace_back(std::make_unique<Entity>(std::string("dr_minion.jpg"), std::string("Basic.glsl"), glm::vec2(300.0f, 200.0f)));
	}
	catch (...) { std::cerr << "Failed to make objects"; }
}

//const std::vector<std::unique_ptr<Entity>>& SceneManager::GetObjects() const
//{
//	std::vector<Entity*> ptr_vec;
//	ptr_vec.reserve(m_Objects.size());
//	for (size_t i = 0; i < m_Objects.size(); i++)
//	{
//		Entity* ptr = m_Objects[i].get();
//		ptr_vec.push_back(ptr);
//	}
//
//	return ptr_vec;
//}
