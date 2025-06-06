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
    : m_buttonPosY(600.0f)
{
}

MainScene::~MainScene()
{
}

bool MainScene::init()
{
    if (!Scene::init()) {
        return false;
    }

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

    auto button = Button::create("ButtonNormal.png", "ButtonActive.png");
    button->setText("Exit");
    button->setPosition(visibleSize.x - 100.0f, 30.0f);
    button->setOnClickCallback([=]() {
        m_pGame->exit();
        });
    addChild(button);

    return true;
}

void MainScene::addTest(std::string_view testName, std::function<TestCase*()> callback)
{
    auto button = Button::create("ButtonNormal.png", "ButtonActive.png");
    button->setText(testName.data());
    button->setPosition(120.0f, m_buttonPosY);
    button->setOnClickCallback([=]() {
        auto test = callback();
        test->init();
        m_pGame->replaceScene(test);
        });
    addChild(button);

    m_buttonPosY -= 60.0f;
}

