#define _CRTDBG_MAP_ALLOC
#include <cstdlib>
#include <crtdbg.h>
#include "Application.h"

#pragma comment(lib, "opengl32.lib")

int main(int argc, char** argv)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	int result = EXIT_SUCCESS;

	Applicaiton* app = Applicaiton::getInstance();
	result = app->run();

	_CrtDumpMemoryLeaks();

	return result;
}