#include "TiledMapTest.h"
#include "2d/TMXTiledMap.h"

USING_NS_OCF;

TiledMapTest::TiledMapTest()
{
}

TiledMapTest::~TiledMapTest()
{
}

bool TiledMapTest::init()
{
	if (TestCase::init()) {

		auto tiledMap = TMXTiledMap::create("tiledmap\\test.tmx");
		addChild(tiledMap);
		return true;
	}
	return false;
}
