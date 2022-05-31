#include "SceneManager.h"

#include "ResourceManager.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Shader.h"
#include "Texture.h"

void SceneManager::BuildScene()
{
	ResourceManager& rm = ResourceManager::GetInstance();

	m_Objects.emplace_back(std::make_unique<TexturedQuad>(std::string("dickbutt.png"), std::string("Basic.glsl"), glm::vec2(0.0f, 0.0f)));
	TexturedQuad* test = m_Objects.back().get();

	Shader& shader = test->GetShader();
	shader.Bind();

	Texture& texture = test->GetTexture();
	texture.Bind();
	shader.SetUniform1i("u_Texture", 0);


	//m_Shaders.push_back(std::make_unique<Shader>("res/shaders/Basic.glsl"));
	//m_Shaders.back()->Bind();
	//m_Shaders.back()->SetUniformMat4f("u_MVP", mvp);

	//m_Textures.emplace_back(std::make_unique<Texture>("res/textures/dickbutt.png"););
	//m_Textures.back()->Bind();

	//m_Objects.emplace_back(m_Textures.back(), m_Shaders.back(), glm::vec2(0.0f, 0.0f));
}

std::vector<TexturedQuad*> SceneManager::GetObjects()
{
	std::vector<TexturedQuad*> ptr_vec;
	ptr_vec.reserve(m_Objects.size());
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
		TexturedQuad* ptr = m_Objects[i].get();
		ptr_vec.push_back(ptr);
	}

	return ptr_vec;
}
