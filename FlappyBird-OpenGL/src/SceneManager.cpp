#include "SceneManager.h"

SceneManager::SceneManager()
{
	m_Objects = std::vector<TexturedQuad>();
}

void SceneManager::AddObject(TexturedQuad& object)
{
	m_Objects[m_CurrentID] = object;
	object.SetSceneID(m_CurrentID);
	m_CurrentID++;
}

void SceneManager::CreateNewQuad(const std::string& texturePath, Shader& shader)
{
	TexturedQuad newQuad(texturePath, shader);

	m_Objects[m_CurrentID] = newQuad;
	newQuad.SetSceneID(m_CurrentID);
	m_CurrentID++;
}
