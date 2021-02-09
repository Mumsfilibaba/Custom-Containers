#pragma once
#include "Types.h"

#include <cstdlib>

struct Mallocator
{
    void* Allocate(UInt32 Size)
    {
        return ::malloc(Size);
    }

    void Free(void* Ptr)
    {
        ::free(Ptr);
    }
};