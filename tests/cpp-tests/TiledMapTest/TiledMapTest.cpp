#include "TiledMapTest.h"
#include "2d/TMXTiledMap.h"

USING_NS_OCF;

TiledMapTest::TiledMapTest()
{
}

TiledMapTest::~TiledMapTest()
{
}

bool TiledMapTest::init()
{
    if (TestCase::init()) {
        float width = 256.0f;
        float height = 240.0f;
        m_camera = Camera::createOrthographic(-128.0f, 128.0f, -120.0f, 120.0f);
        m_camera->setPosition(width / 2.0f, height / 2.0f, 0.0f);
        m_camera->setCameraFlag(CameraFlag::User1);
        addChild(m_camera);

        auto tiledMap = TMXTiledMap::create("tiledmap\\test.tmx");
        tiledMap->setCameraMask(2);
        addChild(tiledMap);
        return true;
    }
    return false;
}

void TiledMapTest::processInput(const InputState& inputState)
{
    auto pos = m_camera->getPosition();

    if (inputState.keyboard.getKeyState(Keyboard::KeyCode::KEY_W) == ButtonState::Hold) {
        m_camera->setPosition(pos.x, pos.y + 2.0f);
    }
    if (inputState.keyboard.getKeyState(Keyboard::KeyCode::KEY_S) == ButtonState::Hold) {
        m_camera->setPosition(pos.x, pos.y - 2.0f);
    }
    if (inputState.keyboard.getKeyState(Keyboard::KeyCode::KEY_A) == ButtonState::Hold) {
        m_camera->setPosition(pos.x + 2.0f, pos.y);
    }
    if (inputState.keyboard.getKeyState(Keyboard::KeyCode::KEY_D) == ButtonState::Hold) {
        m_camera->setPosition(pos.x - 2.0f, pos.y);
    }
}
