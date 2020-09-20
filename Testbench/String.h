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
	
	inline String(const Char* InStr)
	{
		const SizeType Len = static_cast<SizeType>(strlen(InStr));
		if (Len < STACK_BYTES)
		{
			strcpy(Characters, InStr);
			Left = STACK_BYTES - Len;
		}
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
