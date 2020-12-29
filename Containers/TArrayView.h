#pragma once
#if 0
#include "TUtilities.h"

/*
 * TArrayView
 */

template<typename T>
class TArrayView
{
public:
	template<typename TArrayType>
	FORCEINLINE TArrayView(const TArrayType& Array)
		: Begin(Array.Data())
		, End(Array.Data() + Array.Size())
		, Size(Array.Size())
	{
	}
	
	template<const UInt32 N>
	FORCEINLINE TArrayView(T(&Array)[N])
	{
	}
	
private:
	T* Begin;
	T* End;
	UInt32 Size;
};

#endif
