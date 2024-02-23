#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Application.h"

OCF_BEGIN

Camera::Camera()
	: m_projection(1.0f)
	, m_view(1.0f)
{
}

Camera::~Camera()
{
}

bool Camera::init()
{
	glm::ivec2 windowSize = Applicaiton::getInstance()->getWindowSize();

	m_projection = glm::ortho(0.0f, static_cast<float>(windowSize.x), static_cast<float>(windowSize.y), 0.0f);

	return false;
}

const glm::mat4 Camera::getProjectionMatrix() const
{
	return m_projection;
}

OCF_END
