#include "BaseTest.h"
#include "MainScene.h"

USING_NS_OCF;

TestCase::TestCase()
    : m_pTitleLabel(nullptr)
{
}

TestCase::~TestCase()
{
}

bool TestCase::init()
{
    if (Scene::init()) {
        glm::vec2 visibleSize = m_pGame->getVisibleSize();

        m_pTitleLabel = Label::create(m_testCaseName);
        m_pTitleLabel->setPosition(visibleSize.x / 2.0f, 0.0f);
        m_pTitleLabel->setScale(2.0f);
        addChild(m_pTitleLabel);

        auto back = ui::Button::create("ButtonNormal.png", "ButtonActive.png");
        back->setText("Back");
        back->setPosition(visibleSize.x - 100.0f, visibleSize.y - 30.0f); 
        back->setOnAction([=]() {
            auto scene = new MainScene();
            scene->init();
            m_pGame->replaceScene(scene);
            });
        addChild(back);

        return true;
    }

    return false;
}
