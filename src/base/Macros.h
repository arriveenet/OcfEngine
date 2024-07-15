#pragma once
#include <assert.h>
#include "base/Console.h"

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

