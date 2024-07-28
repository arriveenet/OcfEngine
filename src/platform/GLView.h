#include <string>
#include <glm/glm.hpp>
#include "base/types.h"
#include "base/GameObject.h"
#include "base/Rect.h"

NS_OCF_BEGIN

class Scene;
class Renderer;

class GLView : public GameObject {
public:
	GLView();
	virtual ~GLView();

	virtual bool isOpenGLReady() = 0;

	virtual void swapBuffers() = 0;

	virtual bool windowShouldClose() { return false; }

	virtual void pollEvents();

	virtual glm::vec2 getVisibleSize() const;

	virtual void setViewName(std::string_view viewname);

	std::string_view getViewName() const;

	void renderScene(Scene* scene, Renderer* renderer);

protected:
	glm::vec2 m_screenSize;
	glm::vec2 m_resolutionSize;
	std::string m_viewName;
};

NS_OCF_END