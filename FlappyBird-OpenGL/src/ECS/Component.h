#pragma once

class Entity;

enum ComponentType
{
	TRANSFORMCOMPONENT,
	PHYSICSCOMPONENT
};

class Component
{
public:
	Component(Entity* parent) : m_Parent(parent) {};
	virtual void Update() = 0;
	virtual inline ComponentType GetType() const = 0;

	inline Entity* GetParent() const { return m_Parent; }
protected:
	Entity* m_Parent;
};
