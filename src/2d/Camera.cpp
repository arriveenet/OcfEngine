#include "Camera.h"
#include <assert.h>
#include <glm/gtc/matrix_transform.hpp>
#include "platform/Application.h"
#include "base/Game.h"

OCF_BEGIN

Camera* Camera::s_pVisitingCamera = nullptr;

Camera::Camera()
	: m_cameraFlag(CameraFlag::Default)
	, m_projection(1.0f)
	, m_view(1.0f)
	, m_zNear(0.0f)
	, m_zFar(0.0f)
	, m_type(Type::Orthographic)
{
}

Camera* Camera::createPerspective(float fovy, float aspect, float zNear, float zFar)
{
	Camera* pCamera = new Camera();
	pCamera->initPerspective(fovy, aspect, zNear, zFar);

	return pCamera;
}

Camera* Camera::createOrthographic(float left, float right, float bottom, float top, float zNear /*=-1.0f*/, float zFar /*= 1.0f*/)
{
	Camera* pCamera = new Camera();
	pCamera->initOrthographic(left, right, bottom, top, zNear, zFar);

	return pCamera;
}

Camera* Camera::getDefaultCamera()
{
	Scene* pScene = Game::getInstance()->getCurrentScene();
	assert(pScene != nullptr);

	return pScene->getDefaultCamera();
}

Camera* Camera::getVisitingCamera()
{
	return s_pVisitingCamera;
}

Camera::~Camera()
{
}

bool Camera::init()
{
	m_type = Camera::Type::Perspective;

	glm::ivec2 size = Applicaiton::getInstance()->getWindowSize();
	switch (m_type) {
		case ocf::Camera::Type::Perspective:
		{
			float zEye = 10.0f;
			m_zNear = 0.5f;
			m_zFar = 100.0f;
			initPerspective(60.0f, (float)size.x / size.y, m_zNear, m_zFar);
			glm::vec3 eye(size.x / 2.0f, size.y / 2.0f, zEye);
			glm::vec3 center(size.x / 2.0f, size.y / 2.0f, 0.0f);
			setPosition(eye);
			lookAt(center);
			break;
		}
		case ocf::Camera::Type::Orthographic:
		{
			m_zNear = -1024.0f;
			m_zFar  = 1024.0f;
			const float halfWidth = static_cast<float>(size.x / 2);
			const float halfHeight = static_cast<float>(size.y / 2);

			initOrthographic(-halfWidth, halfWidth, -halfHeight, halfHeight, m_zNear, m_zFar);
			break;
		}
	}

	return true;
}

bool Camera::initPerspective(float fovy, float aspect, float zNear, float zFar)
{
	m_type = Type::Perspective;

	m_projection = glm::perspective(fovy, aspect, zNear, zFar);

	return true;
}

bool Camera::initOrthographic(float left, float right, float bottom, float top, float zNear /*=-1.0f*/, float zFar /*= 1.0f*/)
{
	m_type = Type::Orthographic;

	m_projection = glm::ortho(left, right, bottom, top, zNear, zFar);
	m_view = glm::lookAt(glm::vec3(right, top, 1), glm::vec3(right, top, 0), glm::vec3(0, 1, 0));

	return true;
}

void Camera::lookAt(const glm::vec3& center, const glm::vec3& up)
{
	m_view = glm::lookAt(m_position, center, up);
}

const glm::mat4 Camera::getProjectionMatrix() const
{
	return m_projection;
}

const glm::mat4 Camera::getViewMatrix() const
{
	return m_view;
}

const glm::mat4 Camera::getViewProjectionMatrix() const
{
	return m_projection * m_view;
}

OCF_END
