#pragma once
#include "../ECS/Entity.h"
#include "../ECS/DQNAgentComponent.h"

class BoxColliderComponent;

class SceneManager
{
public:

	static SceneManager& GetInstance()
	{
		static SceneManager instance;

		return instance;
	}

	SceneManager(SceneManager const&) = delete;
	void operator=(SceneManager const&) = delete;

	void BuildPlayScene();
	void BuildTrainingScene();
	const std::vector<std::unique_ptr<Entity>>& GetObjects() const { return m_Objects; };
	DQNAgentComponent& GetAgent() const;
	std::vector<BoxColliderComponent*> GetColliders() const;
	glm::vec2 GetNearestPipeGap() const;

	Entity* AddObject(std::unique_ptr<Entity> object);
	void RemoveObject(int ID);
	void CleanUpObjects();
	void ResetCleanupVector();
	void ResetScene();

private:
	SceneManager() {};
private:
	std::vector<std::unique_ptr<Entity>> m_Objects;
	std::vector<uint32_t> m_IDsToRemove;
	uint32_t m_CurrentSceneID = 0;

private:
	const glm::vec3 BACKGROUND_POSITION = glm::vec3(0.0f, 0.0f, -1.0f);
	const glm::vec2 BACKGROUND_SCALE = glm::vec2(1.5f);

	const glm::vec3 GROUND_POSITION = glm::vec3(0.0f, -50.0f, -1.0f);
	const glm::vec2 GROUND_SCALE = glm::vec2(0.4f);

	const glm::vec3 PLAYER_POSITION = glm::vec3(100.0f, 500.0f, 1.0f);
	const glm::vec2 PLAYER_SCALE = glm::vec2(3.0f);
};

