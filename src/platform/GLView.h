#include <string>
#include <glm/glm.hpp>
#include "base/types.h"
#include "base/GameObject.h"
#include "base/Rect.h"

NS_OCF_BEGIN

class Scene;
class Renderer;

struct GLContextAttributes {
    int redBits;
    int greenBits;
    int blueBits;
    int alphaBits;
    int depthBits;
    bool vsync = true;
};

class GLView : public GameObject {
public:
    GLView();
    virtual ~GLView();

    virtual void end() = 0;

    virtual bool isOpenGLReady() = 0;

    virtual void swapBuffers() = 0;

    virtual bool windowShouldClose() { return false; }

    virtual void pollEvents();

    virtual void setFrameSize(float widht, float height);

    virtual glm::vec2 getVisibleSize() const;

    virtual void setDesignResolutionSize(float width, float height);

    virtual const glm::vec2& getDesignResolutionSize() const;

    virtual void setViewName(std::string_view viewname);

    std::string_view getViewName() const;

    void renderScene(Scene* scene, Renderer* renderer);

    static void setGLContextAttributes(GLContextAttributes& glContextAttribues);

    static GLContextAttributes getGLContextAttributes();

    static GLContextAttributes m_glContextAttributes;

protected:
    void updateDesignResolutionSize();

protected:
    glm::vec2 m_screenSize;             //!< 実際のスクリーンサイズ
    glm::vec2 m_designResolutionSize;   //!< ゲーム内の解像度
    std::string m_viewName;             //!< ビューの名前
};

NS_OCF_END