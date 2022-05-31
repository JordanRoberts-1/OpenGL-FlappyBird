#pragma once
#include "TexturedQuad.h"

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

	std::vector<TexturedQuad*> GetObjects();

private:
	SceneManager() {};
private:
	std::vector<std::unique_ptr<TexturedQuad>> m_Objects;
};

