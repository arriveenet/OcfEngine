#include "DrawShapeTest.h"

using namespace ocf;

void DrawShapeTest::onEnter()
{
    m_pDrawShape = DrawShape::create();
    addChild(m_pDrawShape);

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
