#pragma once
#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"
#include "Renderer.h"

class TexturedQuad
{

public:
	TexturedQuad(glm::vec2 position, glm::vec2 size, const std::string& texturePath, Shader& shader);
	TexturedQuad(const std::string& texturePath, Shader& shader);
	TexturedQuad(float scaleFactor, const std::string& texturePath, Shader& shader);

	void SetSceneID(int id);
	int GetSceneID();
	inline std::array<float, 16> getVertices() { return m_Vertices; }

private:
	void BuildVertices(glm::vec2& position, glm::vec2& size);
private:
	std::array<float, 16> m_Vertices;
	int m_SceneID;
	Texture m_Texture;
	Shader m_Shader;
};

