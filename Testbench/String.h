#pragma once
#include "TypeUtilities.h"

class String
{
public:
    using SizeType = Uint32;

    inline String()
    {
        Left = STACK_BYTES;
    }

    inline SizeType Size() const
    {
        return STACK_BYTES - Left;
    }

private:
    

    inline static constexpr SizeType STACK_BYTES = 32;
    union
    {
        Char StackBuffer[STACK_BYTES];
        struct
        {
            Char Characters[31];
            Byte Left;
        };
    };
    
};