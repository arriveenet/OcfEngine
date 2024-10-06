#include "ComponentTest.h"
#include "2d/MoveComponent.h"
#include "base/EventListenerKeyboard.h"

USING_NS_OCF;

ComponentTest::ComponentTest()
{
    setTestCaseName("ComponentTest");
}

ComponentTest::~ComponentTest()
{
}

bool ComponentTest::init()
{
    if (TestCase::init()) {
        auto ship = new Ship();
        this->addChild(ship);

        for (int i = 0; i < 10; i++) {
            auto asteroid = new Asteroid;
            this->addChild(asteroid);
        }

        return true;
    }

    return false;
}

Laser::Laser()
{
    initWithFile("textures\\Laser.png");
    m_pMoveComponent = new MoveComponent(this);
    m_pMoveComponent->setForwardSpeed(800.0f);
    addComponent(m_pMoveComponent);

    m_pCircleComponent = new CircleComponent(this);
    m_pCircleComponent->setRadius(11.0f);
    addComponent(m_pCircleComponent);
}

Laser::~Laser()
{
}

void Laser::updateNode(float deltaTime)
{
    m_deathTimer -= deltaTime;
    if (m_deathTimer <= 0.0f) {
        m_pParent->removeChild(this);
    }
    //else {
    //	for (auto astroid : g_pAsteroid) {
    //		if (intersectCircle(*m_pCircleComponent, *astroid->m_pCircleComponent)) {
    //			m_pParent->removeChild(astroid);

    //			auto iter = std::find(g_pAsteroid.begin(), g_pAsteroid.end(), astroid);
    //			if (iter != g_pAsteroid.end()) {
    //				g_pAsteroid.erase(iter);
    //			}
    //			break;
    //		}
    //	}
    //}
}

Asteroid::Asteroid()
{
    initWithFile("textures\\Asteroid.png");
    m_pMoveComponent = new MoveComponent(this);
    addComponent(m_pMoveComponent);
    m_pCircleComponent = new CircleComponent(this);
    addComponent(m_pCircleComponent);

    m_pCircleComponent->setRadius(40.0f);
    m_pMoveComponent->setForwardSpeed(150.0f);

    glm::vec2 visibleSize = Game::getInstance()->getVisibleSize();
    float rot = static_cast<float>(rand() % 360);
    float x = static_cast<float>(rand() % (int)visibleSize.x);
    float y = static_cast<float>(rand() % (int)visibleSize.y);

    setRotation(rot);
    setPosition(x, y);
}

void Asteroid::updateNode(float deltaTime)
{
    glm::vec2 visibleSize = Game::getInstance()->getVisibleSize();

    if (m_position.x < 0.0f) {
        m_position.x = visibleSize.x;
    }
    if (m_position.x > visibleSize.x) {
        m_position.x = 0.0f;
    }
    if (m_position.y < 0.0f) {
        m_position.y = visibleSize.y;
    }
    if (m_position.y > visibleSize.y) {
        m_position.y = 0.0f;
    }
}

Ship::Ship()
{
    initWithFile("textures\\Ship.png");

    m_pMoveComponent = new MoveComponent(this);
    addComponent(m_pMoveComponent);

    glm::vec2 visibleSize = Game::getInstance()->getVisibleSize();
    setPosition(visibleSize.x / 2, visibleSize.y / 2);

    auto keyboardEvent = EventListenerKeyboard::create();
    keyboardEvent->m_onKeyPressed = [this](Keyboard::KeyCode key, Event* pEvent) {
        float fowardSpeed = 0.0f;
        if (key == Keyboard::KeyCode::KEY_W) {
            fowardSpeed = 300.0f;
        }
        if (key == Keyboard::KeyCode::KEY_S) {
            fowardSpeed = -300.0f;
        }

        float rotation = getRotation();
        if (key == Keyboard::KeyCode::KEY_Q) {
            rotation += 3.0f;
        }
        if (key == Keyboard::KeyCode::KEY_E) {
            rotation -= 3.0f;
        }
        setRotation(rotation);

        m_pMoveComponent->setForwardSpeed(fowardSpeed);
        };

    m_pEventDispatcher->addEventListener(keyboardEvent, this);
}

