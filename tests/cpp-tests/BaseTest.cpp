#include "BaseTest.h"
#include "MainScene.h"

using namespace ocf;;

TestCase::TestCase()
    : m_pTitleLabel(nullptr)
{
}

TestCase::~TestCase()
{
}

bool TestCase::init()
{
    glm::vec2 visibleSize = m_pGame->getVisibleSize();

    m_pTitleLabel = Label::create(m_testCaseName);
    m_pTitleLabel->setPosition(glm::vec2(visibleSize.x / 2.0f, 0.0f));
    // m_pTitleLabel->setScale(2.0f);
    m_root->addChild(m_pTitleLabel);

    auto back = ui::Button::create("ButtonNormal.png", "ButtonActive.png");
    back->setText("Back");
    back->setPosition(glm::vec2(visibleSize.x - 100.0f, visibleSize.y - 30.0f));
    back->setOnAction([=]() {
        auto scene = new MainScene();
        scene->init();
        m_pGame->replaceScene(scene);
    });
    m_root->addChild(back);

    if (Scene::init()) {
        return true;
    }

    return false;
}
