#include "MeshRendererTest.h"
#include "base/EventKeyboard.h"
#include "base/EventListenerKeyboard.h"
#include "3d/Skybox.h"

USING_NS_OCF;

MeshRendererTest::MeshRendererTest()
    : m_pCamera(nullptr)
    , m_pStatusLabel(nullptr)
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
    m_pCamera = FirstPersonCamera::create(60.0f, size.x / size.y, 0.1f, 1000.0f);
    m_pCamera->setCameraFlag(CameraFlag::User1);
    m_pCamera->setPosition(-5, 3, -8);
    addChild(m_pCamera);

    DrawShape* shape = DrawShape::create();
    shape->drawLine(glm::vec3(0, 0, 0), glm::vec3(100, 0, 0), Color4f::RED);
    shape->drawLine(glm::vec3(0, 0, 0), glm::vec3(0, 100, 0), Color4f::GREEN);
    shape->drawLine(glm::vec3(0, 0, 0), glm::vec3(0, 0, 100), Color4f::BLUE);
    shape->setCameraMask((uint16_t)CameraFlag::User1);
    addChild(shape);

   auto mesh = MeshRenderer::create("model/teapot.obj");
   mesh->setCameraMask((uint16_t)CameraFlag::User1);
   addChild(mesh);

   m_pStatusLabel = Label::create("position: 0.000, 0.000, 0.000");
   m_pStatusLabel->setPosition(0.0f, 550.0f);
   m_pStatusLabel->setGlobalZOrder(-100.0f);
   addChild(m_pStatusLabel);

   auto keyboardListener = EventListenerKeyboard::create();
   keyboardListener->m_onKeyPressed = [this](Keyboard::KeyCode key, Event*) {
           if (key == Keyboard::KeyCode::KEY_E) {
               const bool flag = m_pCamera->getCameraControl() ^ true;
               m_pCamera->setCameraControl(flag);
           }
       };

   m_pGame->getEventDispatcher()->addEventListener(keyboardListener, this);

   Skybox* skybox = Skybox::create("skybox/right.png",
                                   "skybox/left.png",
                                   "skybox/top.png",
                                   "skybox/bottom.png",
                                   "skybox/front.png",
                                   "skybox/back.png");
   skybox->setCameraMask((uint16_t)CameraFlag::User1);
   addChild(skybox);

    return true;
}

void MeshRendererTest::updateNode(float /*deltaTime*/)
{
    const auto pos = m_pCamera->getPosition();
    const float yow = m_pCamera->getYaw();
    const float pitch = m_pCamera->getPitch();
    char str[256];
    snprintf(str, sizeof(str), "position: %.3f, %.3f, %.3f\n yow: %f \n pitch: %f",
        pos.x, pos.y, pos.z, yow, pitch);
    m_pStatusLabel->setString(str);
}