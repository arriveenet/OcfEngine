#pragma once
#include "Entity.h"

OCF_BEGIN

class Camera : public Entity {
public:
	enum class Type {
		Perspective,	//!< “§Ž‹“Š‰e
		Orthographic	//!< •½s“Š‰e
	};

	static Camera* createOrthographic(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
	static Camera* getDefaultCamera();

	Camera();
	~Camera();

	bool init() override;
	bool initOrthographic(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

	const glm::mat4 getProjectionMatrix() const;
	const glm::mat4 getViewMatrix() const;
	const glm::mat4 getViewProjectionMatrix() const;

	Camera::Type getType() const { return m_type; }

protected:
	glm::mat4 m_projection;
	glm::mat4 m_view;
	Type m_type;
};

OCF_END
