#pragma once
#include "ECS/Entity.h"

class BoxColliderComponent;

class SceneManager
{
public:
	void BuildScene();

	static SceneManager& GetInstance()
	{
		static SceneManager instance;

		return instance;
	}

	SceneManager(SceneManager const&) = delete;
	void operator=(SceneManager const&) = delete;

	const std::vector<std::unique_ptr<Entity>>& GetObjects() const { return m_Objects; };
	std::vector<BoxColliderComponent*> GetColliders() const;

	Entity* AddObject(std::unique_ptr<Entity> object);
	void RemoveObject(int ID);
	void CleanUpObjects();
	void ResetCleanupVector();

private:
	SceneManager() {};
private:
	std::vector<std::unique_ptr<Entity>> m_Objects;
	std::vector<uint32_t> m_IDsToRemove;
	uint32_t m_CurrentSceneID = 0;
};

