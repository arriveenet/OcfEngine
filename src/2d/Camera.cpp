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
	glm::ivec2 windowSize = Applicaiton::getInstance()->getWindowSize();

	const float halfWidth = static_cast<float>(windowSize.x / 2);
	const float halfHeight = static_cast<float>(windowSize.y / 2);

	initOrthographic(-halfWidth, halfWidth, -halfHeight, halfHeight);

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

void Camera::lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up)
{
	m_view = glm::lookAt(eye, center, up);
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
	return m_view * m_projection;
}

OCF_END
