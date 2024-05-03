#pragma once
#include "Component.h"
#include "base/GameObject.h"
#include "input/Input.h"
#include <glm/glm.hpp>
#include <vector>

OCF_BEGIN

class Game;
class Renderer;

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

	enum {
		FLAGS_TRANSFORM_DIRTY		= (1 << 0),
		FLAGS_CONTENT_SIZE_DIRTY	= (1 << 1),
		FLAGS_RENDER_AS_3D			= (1 << 3),

		FLAGS_DIRTY_MASK = (FLAGS_TRANSFORM_DIRTY | FLAGS_CONTENT_SIZE_DIRTY),
	};

	Entity();
	virtual ~Entity();

	virtual bool init();

	virtual void processInput(const InputState& inputState);

	virtual void update(float deltaTime);
	virtual void updateEntity(float deltaTime);
	virtual void updateComponents(float deltaTime);
	virtual void updateEntitices(float deltaTime);
	virtual void updateTransform();

	virtual void draw(Renderer* renderer, const glm::mat4& transform);

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
	virtual size_t getChildCount() const;
	virtual void setParent(Entity* pEntity);
	virtual Entity* getParent() { return m_pParent; }

	void addComponent(Component* pComponent);
	void removeComponent(Component* pComponent);

	const glm::mat4& getEntityToParentTransform();

	virtual void visit(Renderer* pRenderer, const glm::mat4& parentTransform, uint32_t parentFlags);

protected:
	glm::mat4 transform(const glm::mat4& parentTransform);
	uint32_t processParentFlag(const glm::mat4& parentTransform, uint32_t parentFlag);
	
protected:
	State m_state;

	Entity* m_pParent;
	Game* m_pGame;

	glm::vec2 m_position;
	glm::vec2 m_size;

	float m_rotation;
	
	float m_scaleX;
	float m_scaleY;
	float m_scaleZ;

	glm::mat4 m_transform;
	glm::mat4 m_modelVewTransform;
	bool m_transformDirty;
	bool m_transformUpdated;
	bool m_contentSizeDirty;

	std::vector<Entity*> m_entities;
	std::vector<Component*> m_components;
};

OCF_END
