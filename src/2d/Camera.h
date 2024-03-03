#pragma once
#include "Entity.h"

OCF_BEGIN

class Camera : public Entity {
public:
	enum class Type {
		Perspective,	//!< �������e
		Orthographic	//!< ���s���e
	};

	Camera();
	~Camera();

	bool init() override;

	const glm::mat4 getProjectionMatrix() const;

protected:
	glm::mat4 m_projection;
	glm::mat4 m_view;
};

OCF_END
