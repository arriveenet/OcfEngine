#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "platform/Application.h"

OCF_BEGIN

Camera::Camera()
	: m_projection(1.0f)
	, m_view(1.0f)
	, m_type(Type::Orthographic)
{
}

Camera::~Camera()
{
}

bool Camera::init()
{
	glm::ivec2 windowSize = Applicaiton::getInstance()->getWindowSize();

	const float halfWidth = static_cast<float>(windowSize.x / 2);
	const float halfHeight = static_cast<float>(windowSize.y / 2);

	m_projection = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight);
	//m_projection = glm::ortho(0.0f, static_cast<float>(windowSize.x), 0.0f, static_cast<float>(windowSize.y));

	m_view = glm::lookAt(glm::vec3(halfWidth, halfHeight, 1), glm::vec3(halfWidth, halfHeight, 0), glm::vec3(0, 1, 0));

	return true;
}

const glm::mat4 Camera::getProjectionMatrix() const
{
	return m_projection;
}

const glm::mat4 Camera::getViewMatrix() const
{
	return m_view;
}

OCF_END
