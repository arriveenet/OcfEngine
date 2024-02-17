#pragma once
#include "GameObject.h"
#include "Component.h"
#include <glm/glm.hpp>
#include <vector>

/**
 * @brief エンティティクラス
 */
class Entity : public GameObject {
public:
	/**
	 * @brief エンティティの状態
	 */
	enum State {
		Active,	//!< 生存中
		Dead	//!< 死亡
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

	virtual void setSize(float width, float height);
	virtual glm::vec2 getSize() const;

	virtual void setRotation(float rotation);
	virtual float getRotation() const;

	virtual void setScale(float scale);
	virtual void setScaleX(float scaleX);
	virtual void setScaleY(float scaleY);
	virtual void setScaleZ(float scaleZ);
	virtual float getScale() const;

	virtual void addChild(Entity* pEntity);
	virtual void removeChild(Entity* pEntity);

	void addComponent(Component* pComponent);
	void removeComponent(Component* pComponent);
	
protected:
	State m_state;

	glm::vec2 m_position;
	glm::vec2 m_size;

	float m_rotation;
	
	float m_scaleX;
	float m_scaleY;
	float m_scaleZ;

	std::vector<Entity*> m_entities;
	std::vector<Component*> m_components;
};