#pragma once
#include <array>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "../Texture.h"
#include "../Renderer.h"
#include "Component.h"

class TransformComponent;
enum ComponentType;

class Entity
{
public:
	Entity(const std::string& texture, const std::string& shader, glm::vec2 pos, glm::vec2 scale);

public:
	void SetSceneID(int id);
	int GetSceneID();

	inline Texture* GetTexture() const { return m_Texture; }
	inline Shader* GetShader() const { return m_Shader; }

	template <typename T>
	T* GetComponent(ComponentType type)
	{
		for (uint32_t i = 0; i < m_Components.size(); i++)
		{
			if (type == m_Components[i]->GetType())
			{
				return (T*)m_Components[i].get();
			}
		}
		return nullptr;
	}

	inline TransformComponent* GetTransform() const { return m_Transform; }
	inline const std::vector<std::unique_ptr<Component>>& GetAllComponents() const;

	template <typename T>
	T* AddComponent(Entity* parent)
	{
		return 	(T*)m_Components.emplace_back(new T(parent)).get();
	}

	void Init();
	void Update();

private:
	int m_SceneID;
	std::vector<std::unique_ptr<Component>> m_Components;

	//Always on each entity and used frequently, so cheaper to cache it
	TransformComponent* m_Transform;

	Texture* m_Texture;
	Shader* m_Shader;
};

