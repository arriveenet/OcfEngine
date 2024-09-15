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
    m_pCamera->setPosition(5, 3, 8);
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

    return true;
}

void MeshRendererTest::processInput(const ocf::InputState& inputState)
{
    static float angle = 0.0f;
    if (inputState.mouse.getButtonState(Mouse::MouseButton::Left) == ButtonState::Hold) {
        angle += 1.2f;
        angle = fmodf(angle, 360.0f);
        float x = cosf(glm::radians(angle)) * 5.0f;
        float z = sinf(glm::radians(angle)) * 5.0f;
        m_pCamera->setPosition(x ,4.5f ,z);
    }

    Scene::processInput(inputState);
}
