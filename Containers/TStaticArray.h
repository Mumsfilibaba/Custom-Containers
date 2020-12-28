#pragma once
#include "TUtilities.h"

/*
 * TStaticArray - A static array
 */

template<typename T, Int32 N>
struct TStaticArray
{
	using Iterator 		= T*;
	using ConstIterator	= const T*;
	
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
	
	FORCEINLINE void Swap(TStaticArray& Other)
	{
		TStaticArray TempArray(::Move(*this));
		*this = ::Move(Other);
		Other = ::Move(TempArray);
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
	
	/*
	 * STL Iterators for range-based for loops
	 */
	
	FORCEINLINE Iterator begin()
	{
		return Array;
	}
	
	FORCEINLINE Iterator end()
	{
		return Array + N;
	}
	
	FORCEINLINE ConstIterator begin() const
	{
		return Array;
	}
	
	FORCEINLINE ConstIterator end() const
	{
		return Array + N;
	}
	
	FORCEINLINE ConstIterator cbegin() const
	{
		return Array;
	}
	
	FORCEINLINE ConstIterator cend() const
	{
		return Array + N;
	}
	
	T Array[N];
};
