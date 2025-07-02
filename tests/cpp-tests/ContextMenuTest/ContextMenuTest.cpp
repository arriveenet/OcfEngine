#include "ContextMenuTest.h"
#include "ui/UIContextMenu.h"
#include "ui/UIMenuItem.h"
#include "ui/UIButton.h"
#include "2d/Label.h"

USING_NS_OCF;
using namespace ocf::ui;

ContextMenuTest::ContextMenuTest()
    : m_contextMenu(nullptr)
{
}

ContextMenuTest::~ContextMenuTest()
{
}

bool ContextMenuTest::init()
{
    if (!TestCase::init()) {
        return false;
    }

    setupUI();
    return true;
}

void ContextMenuTest::setupUI()
{
    const glm::vec2 visibleSize = m_pGame->getVisibleSize();

    // Add title
    auto titleLabel = Label::createWithTTF("fonts/NotoSansJP-Regular.ttf", "Context Menu Test", 24);
    titleLabel->setPosition(visibleSize.x * 0.5f, visibleSize.y - 50.0f);
    addChild(titleLabel);

    // Add instruction
    auto instructionLabel = Label::createWithTTF("fonts/NotoSansJP-Regular.ttf", "Right-click widgets to show context menu", 16);
    instructionLabel->setPosition(visibleSize.x * 0.5f, visibleSize.y - 100.0f);
    addChild(instructionLabel);

    // Create context menu
    m_contextMenu = ContextMenu::create();
    if (m_contextMenu) {
        // Add menu items
        auto item1 = MenuItem::create("New File");
        item1->setOnAction([this]() {
            OCFLOG("New File selected");
            m_contextMenu->hide();
        });

        auto item2 = MenuItem::create("Open");
        item2->setOnAction([this]() {
            OCFLOG("Open selected");
            m_contextMenu->hide();
        });

        auto item3 = MenuItem::create("Save");
        item3->setOnAction([this]() {
            OCFLOG("Save selected");
            m_contextMenu->hide();
        });
        
        auto separator = MenuItem::create("---");
        separator->setSeparator(true);
        separator->setEnabled(false);

        auto item4 = MenuItem::create("Exit");
        item4->setOnAction([this]() {
            OCFLOG("Exit selected");
            m_contextMenu->hide();
        });

        m_contextMenu->addMenuItem(item1);
        m_contextMenu->addMenuItem(item2);
        m_contextMenu->addMenuItem(item3);
        m_contextMenu->addMenuItem(separator);
        m_contextMenu->addMenuItem(item4);

        addChild(m_contextMenu);
    }

    // Add test button that shows context menu
    auto testButton = Button::create("Show Context Menu");
    testButton->setPosition(visibleSize.x * 0.5f, visibleSize.y * 0.5f);
    testButton->setOnAction([this, visibleSize]() {
        showContextMenu(glm::vec2(visibleSize.x * 0.5f, visibleSize.y * 0.4f));
    });
    // Set context menu for the button to demonstrate widget integration
    testButton->setContextMenu(m_contextMenu);
    addChild(testButton);

    // Add another widget to show context menu functionality
    auto widget1 = Button::create("Widget 1 (Right-click me)");
    widget1->setPosition(visibleSize.x * 0.3f, visibleSize.y * 0.3f);
    widget1->setContextMenu(m_contextMenu);
    addChild(widget1);

    auto widget2 = Button::create("Widget 2 (Right-click me)");
    widget2->setPosition(visibleSize.x * 0.7f, visibleSize.y * 0.3f);
    widget2->setContextMenu(m_contextMenu);
    addChild(widget2);
}

void ContextMenuTest::showContextMenu(const glm::vec2& position)
{
    if (m_contextMenu) {
        m_contextMenu->showAt(position);
    }
}