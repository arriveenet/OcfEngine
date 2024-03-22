#pragma once

#define OCF_BEGIN namespace ocf {
#define OCF_END   }
#define USING_OCF using namespace ocf;

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
