#pragma once
#include "TUtilities.h"

/*
 * TStaticArray - A static array
 */

template<typename T, Int32 N>
class TStaticArray
{
public:
	FORCEINLINE TStaticArray() noexcept
		: Array()
	{
	}
	
	FORCEINLINE TStaticArray(const TStaticArray& Other) noexcept
		: Array()
	{
		for (UInt32 i = 0; i < N; i++)
		{
			Array[i] = Other.Array[i];
		}
	}
	
	FORCEINLINE T& Front()
	{
		return Array[0];
	}
	
	FORCEINLINE const T& Front() const
	{
		return Array[0];
	}
	
	FORCEINLINE T& Back()
	{
		return Array[N-1];
	}
	
	FORCEINLINE const T& Back() const
	{
		return Array[N-1];
	}
	
	FORCEINLINE T& At(UInt32 Index)
	{
		VALIDATE(Index < N);
		return Array[Index];
	}
	
	FORCEINLINE const T& At(UInt32 Index) const
	{
		VALIDATE(Index < N);
		return Array[Index];
	}
	
	FORCEINLINE void Fill(const T& Value)
	{
		for (UInt32 i = 0; i < N; i++)
		{
			Array[i] = Value;
		}
	}
	
	FORCEINLINE UInt32 Size() const
	{
		return N;
	}
	
	FORCEINLINE T* Data()
	{
		return Array;
	}
	
	FORCEINLINE const T* Data() const
	{
		return Array;
	}
	
	FORCEINLINE T& operator[](UInt32 Index)
	{
		return At(Index);
	}
	
	FORCEINLINE const T& operator[](UInt32 Index) const
	{
		return At(Index);
	}
	
	FORCEINLINE TStaticArray& operator=(const TStaticArray& Other)
	{
		if (this != &Other)
		{
			for (UInt32 i = 0; i < N; i++)
			{
				Array[i] = Other.Array[i];
			}
		}
		
		return *this;
	}
	
private:
	T Array[N];
};
