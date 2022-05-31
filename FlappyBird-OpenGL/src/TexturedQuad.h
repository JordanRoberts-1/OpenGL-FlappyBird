#pragma once
#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Texture.h"
#include "Renderer.h"

class TexturedQuad
{
public:
	TexturedQuad(const std::string& texture, const std::string& shader, glm::vec2 pos);

public:
	void SetSceneID(int id);
	int GetSceneID();

	inline glm::vec2 GetPosition() const { return m_Position; }
	inline glm::vec2 GetSize() const { return m_Size; }

	inline Texture& GetTexture() const { return m_Texture; }
	inline Shader& GetShader() const { return m_Shader; }

private:
	int m_SceneID;

	glm::vec2 m_Position;
	glm::vec2 m_Size;

	Texture& m_Texture;
	Shader& m_Shader;
};

