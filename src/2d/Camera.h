#pragma once
#include "Entity.h"

OCF_BEGIN

class Scene;

enum class CameraFlag : uint16_t {
	Default = 1,
	User1  = 1 << 1,
	User2  = 1 << 2,
	User3  = 1 << 3,
	User4  = 1 << 4,
};

class Camera : public Entity {
	friend class Scene;

public:
	enum class Type {
		Perspective,	//!< “§Ž‹“Š‰e
		Orthographic	//!< •½s“Š‰e
	};

	static Camera* createPerspective(float fovy, float aspect, float zNear, float zFar);
	static Camera* createOrthographic(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);
	static Camera* getDefaultCamera();

	static Camera* getVisitingCamera();

	Camera();
	~Camera();

	bool init() override;
	bool initPerspective(float fovy, float aspect, float zNear, float zFar);
	bool initOrthographic(float left, float right, float bottom, float top, float zNear = -1.0f, float zFar = 1.0f);

	virtual void lookAt(const glm::vec3& eye, const glm::vec3& center, const glm::vec3& up = glm::vec3(0, 1, 0));

	const glm::mat4 getProjectionMatrix() const;
	const glm::mat4 getViewMatrix() const;
	const glm::mat4 getViewProjectionMatrix() const;

	Camera::Type getType() const { return m_type; }

	CameraFlag getCameraFlag() const { return m_cameraFlag; }
	void setCameraFlag(CameraFlag flag) { m_cameraFlag = flag; }

protected:
	static Camera* s_pVisitingCamera;
	CameraFlag m_cameraFlag;
	glm::mat4 m_projection;
	glm::mat4 m_view;
	Type m_type;
};

OCF_END
