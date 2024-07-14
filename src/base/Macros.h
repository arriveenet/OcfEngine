#pragma once
#include <assert.h>

#define OCF_ASSERT(expression)    assert(expression)

#ifndef OCFASSERT
#define OCFASSERT(expression, msg)    \
    do                                \
    {                                 \
        OCF_ASSERT(expression);       \
    }                                 \
    while(0)
#endif // !OCFASSERT

#include "base/Config.h"

#define OCF_SAFE_DELETE(p) \
    do                    \
    {                     \
        delete (p);       \
        (p) = nullptr;    \
    } while (0)

#define OCF_SAFE_DELETE_ARRAY(p) \
    do                          \
    {                           \
        if (p)                  \
        {                       \
            delete[](p);        \
            (p) = nullptr;      \
        }                       \
    } while (0)

#define OCF_SAFE_RELEASE(p) \
    do                      \
    {                       \
        if(p)               \
        {                   \
            (p)->release(); \
        }                   \
    } while (0);            \

#define OCF_SAFE_RETAIN(p) \
    do                      \
    {                       \
        if(p)               \
        {                   \
            (p)->retain();  \
        }                   \
    } while (0);            \
