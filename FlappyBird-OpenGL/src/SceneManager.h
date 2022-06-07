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

private:
	SceneManager() {};
private:
	std::vector<std::unique_ptr<Entity>> m_Objects;
};

