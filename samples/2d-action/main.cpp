#include "AppDelegate.h"

using namespace ocf;

int main()
{
    int result = EXIT_SUCCESS;

    AppDelegate app;
    result = Applicaiton::getInstance()->run();

    return result;
}