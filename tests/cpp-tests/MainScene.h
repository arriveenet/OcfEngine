#include "OcfEngine.h"

class TestCase;

class MainScene : public ocf::Scene {
public:
    MainScene();
    virtual ~MainScene();

    bool init() override;

    void addTest(std::string_view testName, std::function<TestCase*()> callback);

protected:
    float m_buttonPosY;
};

