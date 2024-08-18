#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include "AppDelegate.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "OcfEngine.lib")

using namespace ocf;

int main(int argc, char** argv)
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    int result = EXIT_SUCCESS;

    // アプリケーションのインスタンスを作成
    AppDelegate app;
    result = Applicaiton::getInstance()->run();

    return result;
}