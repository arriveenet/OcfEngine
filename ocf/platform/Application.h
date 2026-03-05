#pragma once
#include <glm/glm.hpp>
#include "ApplicationBase.h"

namespace ocf {

class Applicaiton : public ApplicationBase {
public:
    static Applicaiton* getInstance();

    Applicaiton();
    ~Applicaiton();

    bool init();

    int run();

private:
    int m_windowWidth;
    int m_windowHeight;
};

} // namespace ocf
