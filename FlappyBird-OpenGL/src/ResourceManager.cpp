#include "ResourceManager.h"

Texture& ResourceManager::CreateTexture(const std::string& filename)
{
	std::unique_ptr<Texture>& ref = m_Textures[filename];

	if (ref) return *ref;

	m_Textures[filename] = std::make_unique<Texture>(m_ResString + "textures/" + filename);
	return *m_Textures[filename];
}

Shader& ResourceManager::CreateShader(const std::string& filename)
{
	std::unique_ptr<Shader>& ref = m_Shaders[filename];

	if (ref) return *ref;

	m_Shaders[filename] = std::make_unique<Shader>(m_ResString + "shaders/" + filename);
	return *m_Shaders[filename];
}
