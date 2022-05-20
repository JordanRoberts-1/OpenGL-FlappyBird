#pragma once
#include "TexturedQuad.h"

#include <vector>
#include <unordered_map>

class SceneManager
{
public:
	SceneManager();
	void AddObject(TexturedQuad& object);
	void RemoveQuad();
	void CreateNewQuad(const std::string& texturePath, Shader& shader);
private:
	std::unordered_map<int, TexturedQuad> m_Objects;
	int m_CurrentID;
};

