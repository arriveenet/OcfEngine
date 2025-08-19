#include "MeshRendererTest.h"
#include "base/EventKeyboard.h"
#include "base/EventListenerKeyboard.h"
#include "3d/Skybox.h"

using namespace ocf;;

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
    Viewport* root = getRoot();
    glm::vec2 size = Game::getInstance()->getVisibleSize();
    m_pCamera = FirstPersonCamera::create(60.0f, size.x / size.y, 0.1f, 1000.0f);
    m_pCamera->setCameraFlag(CameraFlag::User1);
    m_pCamera->setPosition(glm::vec3(- 5, 3, -8));
    root->addChild(m_pCamera);

    Skybox* skybox = Skybox::create("skybox/right.png",
        "skybox/left.png",
        "skybox/top.png",
        "skybox/bottom.png",
        "skybox/front.png",
        "skybox/back.png");
    skybox->setCameraMask(static_cast<uint16_t>(CameraFlag::User1));
    root->addChild(skybox);

    auto mesh = MeshRenderer::create("model/teapot.obj");
    mesh->setCameraMask(static_cast<uint16_t>(CameraFlag::User1));
    root->addChild(mesh);

    m_pStatusLabel = Label::create("position: 0.000, 0.000, 0.000");
    m_pStatusLabel->setPosition(glm::vec2(0.0f, 100.0f));
    root->addChild(m_pStatusLabel);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->m_onKeyPressed = [this](Keyboard::KeyCode key, Event*) {
           if (key == Keyboard::KeyCode::KEY_E) {
               const bool flag = m_pCamera->getCameraControl() ^ true;
               m_pCamera->setCameraControl(flag);
           }
       };

    m_pGame->getEventDispatcher()->addEventListener(keyboardListener, m_pCamera);

    return TestCase::init();
}

void MeshRendererTest::updateScene(float /*deltaTime*/)
{
    const auto pos = m_pCamera->getPosition();
    const float yow = m_pCamera->getYaw();
    const float pitch = m_pCamera->getPitch();
    char str[256];
    snprintf(str, sizeof(str), "position: %.3f, %.3f, %.3f\nyow: %f \npitch: %f",
        pos.x, pos.y, pos.z, yow, pitch);
    m_pStatusLabel->setString(str);
}