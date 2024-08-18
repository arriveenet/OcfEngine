#include "Console.h"
#include <string>
#include <stdio.h>
#include <stdarg.h>
#include <Windows.h>

NS_OCF_BEGIN

static std::string vformat(const char* format, va_list ap)
{
    std::string buf(512, '\0');

    va_list args;
    va_copy(args, ap);
    int ret = vsnprintf(&buf.front(), buf.length() + 1, format, args);
    va_end(args);

    return buf;
}

static void print_impl(std::string buf)
{
    buf.push_back('\0');

    OutputDebugStringA(buf.c_str());

    auto hStdout = ::GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdout) {
        DWORD ch = static_cast<DWORD>(buf.size());
        ::WriteConsoleA(hStdout, buf.c_str(), ch, nullptr, 0);
    }
}

void print(const char* format, ...)
{
    va_list args;

    va_start(args, format);
    auto buf = vformat(format, args);
    va_end(args);

    print_impl(buf);
}

NS_OCF_END
