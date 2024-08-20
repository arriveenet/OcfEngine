#include "MeshRendererTest.h"

USING_NS_OCF;

MeshRendererTest::MeshRendererTest()
{
}

MeshRendererTest::~MeshRendererTest()
{
}

bool MeshRendererTest::init()
{
    if (!TestCase::init()) {
        return false;
    }

   auto mesh = MeshRenderer::create("teapot.obj");
   addChild(mesh);

    return true;
}
