#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#ifdef _WIN32
#include <crtdbg.h>
#endif
#include "AppDelegate.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "OcfEngine.lib")

using namespace ocf;

int main(int argc, char** argv)
{
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
    int result = EXIT_SUCCESS;

    // �A�v���P�[�V�����̃C���X�^���X���쐬
    AppDelegate app;
    result = Applicaiton::getInstance()->run();

    return result;
}
