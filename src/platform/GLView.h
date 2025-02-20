#include <string>
#include <glm/glm.hpp>
#include "base/Types.h"
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

    virtual void setViewport(float x, float y, float w, float h);

    virtual glm::vec2 getWindowSize() const;

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
    glm::vec2 m_windowSize;             //!< ウィンドウサイズ
    glm::vec2 m_designResolutionSize;   //!< ゲーム内の解像度
    std::string m_viewName;             //!< ビューの名前
    Rect m_viewportRect;                //!< ビューポートの矩形
    float m_scaleX;
    float m_scaleY;
};

NS_OCF_END
