#include "MainScene.h"

#include "ActionTest/ActionTest.h"
#include "AudioEngineTest/AudioEngineTest.h"
#include "ComponentTest/ComponentTest.h"
#include "DrawShapeTest/DrawShapeTest.h"
#include "EventTest/EventTest.h"
#include "FontTest/FontTest.h"
#include "MeshRendererTest/MeshRendererTest.h"
#include "SpriteTest/SpriteTest.h"
#include "TiledMapTest/TiledMapTest.h"
#include <ocf/core/Engine.h>
#include <ocf/ui/UIButton.h>

using namespace ocf;
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
    addTest("ActionTest", []() {return new ActionTest(); });
    addTest("AudioTest", []() {return new AudioEngineTest(); });
    addTest("CompTest", []() {return new ComponentTest(); });
    addTest("DrawShapeTest", []() {return new DrawShapeTest(); });
    addTest("EventTest", []() {return new EventTest(); });
    addTest("FontTest", []() {return new FontTest(); });
    addTest("MeshTest", []() {return new MeshRendererTest(); });
    addTest("SpriteTest", []() {return new SpriteTestDemo(); });
    addTest("TiledMapTest", []() {return new TiledMapTest(); });

    glm::vec2 visibleSize = m_pGame->getVisibleSize();

    auto button = Button::create("Exit");
    button->setPosition(glm::vec2(visibleSize.x - 100.0f, visibleSize.y - 30.0f));
    button->setOnAction([=, this]() {
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
    button->setOnAction([=, this]() {
        auto test = callback();
        test->init();
        m_pGame->replaceScene(test);
        });
    m_root->addChild(button);

    m_buttonPosY += 32.0f;
}

