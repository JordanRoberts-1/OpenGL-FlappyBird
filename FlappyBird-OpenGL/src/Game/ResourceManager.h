#pragma once
#include "../Rendering/Texture.h"
#include "../Rendering/Shader.h"
#include <unordered_map>


class ResourceManager
{
public:
	static ResourceManager& GetInstance()
	{
		static ResourceManager instance;

		return instance;
	}

	ResourceManager(ResourceManager const&) = delete;
	void operator=(ResourceManager const&) = delete;

	Texture* CreateTexture(const std::string& filename);
	Shader* CreateShader(const std::string& filename);
private:
	ResourceManager() : m_ResString("res/") {};
private:
	std::unordered_map<std::string, std::unique_ptr<Texture>> m_Textures;
	std::unordered_map<std::string, std::unique_ptr<Shader>> m_Shaders;

	std::string m_ResString;
};

