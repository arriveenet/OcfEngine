#include "MainScene.h"

#include "SpriteTest/SpriteTest.h"
#include "ActionTest/ActionTest.h"
#include "ComponentTest/ComponentTest.h"
#include "TiledMapTest/TiledMapTest.h"
#include "AudioEngineTest/AudioEngineTest.h"
#include "MeshRendererTest/MeshRendererTest.h"
#include "EventTest/EventTest.h"
#include "FontTest/FontTest.h"
#include "DrawShapeTest/DrawShapeTest.h"

USING_NS_OCF;

using namespace ocf::ui;

MainScene::MainScene()
    : m_buttonPosY(150.0f)
{
}

MainScene::~MainScene()
{
}

bool MainScene::init()
{


    addTest("SpriteTest", []() {return new SpriteTestDemo(); });
    addTest("ActionTest", []() {return new ActionTest(); });
    addTest("CompTest", []() {return new ComponentTest(); });
    addTest("TiledMapTest", []() {return new TiledMapTest(); });
    addTest("AudioTest", []() {return new AudioEngineTest(); });
    addTest("MeshTest", []() {return new MeshRendererTest(); });
    addTest("EventTest", []() {return new EventTest(); });
    addTest("FontTest", []() {return new FontTest(); });
    addTest("DrawShapeTest", []() {return new DrawShapeTest(); });

    glm::vec2 visibleSize = m_pGame->getVisibleSize();

    auto button = Button::create("Exit");
    button->setPosition(glm::vec2(visibleSize.x - 100.0f, visibleSize.y - 30.0f));
    button->setOnAction([=]() {
        m_pGame->exit();
        });
    m_root->addChild(button);

    if (!Scene::init()) {
        return false;
    }

    return true;
}

void MainScene::addTest(std::string_view testName, std::function<TestCase*()> callback)
{
    auto button = Button::create(testName);
    button->setPosition(glm::vec2(90, m_buttonPosY));
    button->setOnAction([=]() {
        auto test = callback();
        test->init();
        m_pGame->replaceScene(test);
        });
    m_root->addChild(button);

    m_buttonPosY += 32.0f;
}

