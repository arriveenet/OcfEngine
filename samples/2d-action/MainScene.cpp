#include "MainScene.h"

using namespace ocf;

bool MainScene::init()
{
    Viewport* root = getRoot();

    auto s = Sprite::create("yuka.jpg");
    s->setPosition(glm::vec2(200,200));
    root->addChild(s);

    if (!Scene::init()) {
        return false;
    }
    return true;
}
