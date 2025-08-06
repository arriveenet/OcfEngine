#include "DrawShapeTest.h"

using namespace ocf;

void DrawShapeTest::onEnter()
{
    Viewport* root = getRoot();
    m_pDrawShape = DrawShape::create();
    root->addChild(m_pDrawShape);

    // Draw Point
    m_pDrawShape->drawPoint(glm::vec2(500, 500), Color4f::WHITE);
    m_pDrawShape->setPointSize(5.0f);

    // Draw Line
    m_pDrawShape->drawLine(glm::vec2(480, 480), glm::vec2(600, 200), Color4f::BLUE);
    m_pDrawShape->setLineWidth(5.0f);

    // Draw Triangle
    m_pDrawShape->drawFillTriangle(glm::vec2(400, 100), glm::vec2(600, 100),
                                   glm::vec2(500, 250), Color4f::YELLOW);

    // Draw Filled Circle
    m_pDrawShape->drawFillCircle(glm::vec2(100, 100), 50, Color4f::RED);

    // Draw Filled Rect
    m_pDrawShape->drawFillRect(glm::vec2(400, 400), glm::vec2(450, 450), Color4f::GREEN);

    // Draw Polygon
    std::vector<glm::vec2> vert2 = {
        {200, 300},
        {200, 150},
        {350, 150},
        {350, 200},
        {250, 200},
        {250, 250},
        {350, 250},
        {350, 300},
    };
    m_pDrawShape->drawPolygon(vert2, Color4f::ORANGE);
}
