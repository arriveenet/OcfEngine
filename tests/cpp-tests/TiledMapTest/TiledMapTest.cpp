#include "TiledMapTest.h"
#include "2d/TMXTiledMap.h"

USING_NS_OCF;

TiledMapTest::TiledMapTest()
    : m_camera(nullptr)
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
        m_camera->setPosition(glm::vec2(width / 2.0f, height / 2.0f));
        m_camera->setCameraFlag(CameraFlag::User1);
        addChild(m_camera);

        auto tiledMap = TMXTiledMap::create("tiledmap/test.tmx");
        addChild(tiledMap);
        return true;
    }
    return false;
}
