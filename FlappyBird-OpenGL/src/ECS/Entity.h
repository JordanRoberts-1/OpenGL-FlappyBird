#pragma once
#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../Texture.h"
#include "../Renderer.h"
#include "Component.h"
#include "TransformComponent.h"

class Entity
{
public:
	Entity(const std::string& texture, const std::string& shader, glm::vec2 pos);

public:
	void SetSceneID(int id);
	int GetSceneID();

	inline glm::vec2 GetPosition() const { return m_Transform->GetPosition(); }
	inline glm::vec2 GetSize() const { return m_Transform->GetScale(); }

	inline Texture* GetTexture() const { return m_Texture; }
	inline Shader* GetShader() const { return m_Shader; }

	virtual void Update() = 0;

protected:
	int m_SceneID;
	std::vector<std::unique_ptr<Component>> m_Components;

	//Always on each entity and used frequently, so cheaper to cache it
	std::unique_ptr<TransformComponent> m_Transform;

	Texture* m_Texture;
	Shader* m_Shader;
};

