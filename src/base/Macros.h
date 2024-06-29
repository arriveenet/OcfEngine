#pragma once

#define NS_OCF_BEGIN namespace ocf {
#define NS_OCF_END   }
#define USING_NS_OCF using namespace ocf
#define NS_OCF ::ocf

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
