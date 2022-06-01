#pragma once
#include "Entity.h"

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

	std::vector<Entity*> GetObjects() const;

private:
	SceneManager() {};
private:
	std::vector<std::unique_ptr<Entity>> m_Objects;
};

