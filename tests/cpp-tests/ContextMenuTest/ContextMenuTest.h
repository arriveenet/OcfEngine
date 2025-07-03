#pragma once
#include "BaseTest.h"

class ContextMenuTest : public TestCase {
public:
    ContextMenuTest();
    ~ContextMenuTest() override;

    bool init() override;

private:
    void setupUI();
    void showContextMenu(const glm::vec2& position);

private:
    ocf::ui::ContextMenu* m_contextMenu;
};