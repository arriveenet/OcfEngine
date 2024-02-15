#pragma once
#include "GameObject.h"
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>

class Entity : public GameObject {
public:
	enum State {
		Active,
		Dead
	};

	Entity();
	virtual ~Entity();

	virtual bool init();

	virtual void update(float deltaTime);
	virtual void updateComponents(float deltaTime);
	virtual void updateEntity(float deltaTime);

	void setState(State state) { m_state = state; }
	State getState() const { return m_state; }

	virtual void setPosition(const glm::vec2& position);
	virtual void setPosition(float x, float y);
	virtual glm::vec2 getPosition() const;

	virtual void addChild(Entity* pEntity);
	virtual void removeChild(Entity* pEntity);

	void addComponent(Component* pComponent);
	void removeComponent(Component* pComponent);
	
protected:
	State m_state;
	glm::vec2 m_position;
	glm::vec2 m_size;
	std::vector<Entity*> m_entities;
	std::vector<Component*> m_components;
};