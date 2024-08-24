#include "MeshRendererTest.h"

USING_NS_OCF;

MeshRendererTest::MeshRendererTest()
    : m_pCamera(nullptr)
{
}

MeshRendererTest::~MeshRendererTest()
{
}

bool MeshRendererTest::init()
{
    if (!TestCase::init()) {
        return false;
    }

    glm::vec2 size = Game::getInstance()->getVisibleSize();
    m_pCamera = Camera::createPerspective(60.0f, size.x / size.y, 0.1f, 1000.0f);
    m_pCamera->setCameraFlag(CameraFlag::User1);
    m_pCamera->setPosition(3, 3, 3);
    addChild(m_pCamera);

   auto mesh = MeshRenderer::create("teapot.obj");
   mesh->setCameraMask((uint16_t)CameraFlag::User1);
   addChild(mesh);

    return true;
}
