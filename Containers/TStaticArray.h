#pragma once
#include "TUtilities.h"

/*
 * TStaticArray - A static array
 */

template<typename T, Int32 N>
struct TStaticArray
{
public:
	using Iterator 		= T*;
	using ConstIterator	= const T*;
	using SizeType		= UInt32;

	FORCEINLINE T& Front() noexcept
	{
		return Array[0];
	}
	
	FORCEINLINE const T& Front() const noexcept
	{
		return Array[0];
	}
	
	FORCEINLINE T& Back() noexcept
	{
		return Array[N-1];
	}
	 
	FORCEINLINE const T& Back() const noexcept
	{
		return Array[N-1];
	}
	
	FORCEINLINE T& At(SizeType Index) noexcept
	{
		VALIDATE(Index < N);
		return Array[Index];
	}
	
	FORCEINLINE const T& At(SizeType Index) const noexcept
	{
		VALIDATE(Index < N);
		return Array[Index];
	}
	
	FORCEINLINE void Fill(const T& Value) noexcept
	{
		for (UInt32 i = 0; i < N; i++)
		{
			Array[i] = Value;
		}
	}
	
	FORCEINLINE void Swap(TStaticArray& Other) noexcept
	{
		TStaticArray TempArray(::Move(*this));
		*this = ::Move(Other);
		Other = ::Move(TempArray);
	}
	
	FORCEINLINE constexpr SizeType Size() const noexcept
	{
		return N;
	}

	FORCEINLINE constexpr SizeType SizeInBytes() const noexcept
	{
		return N * sizeof(T);
	}
	
	FORCEINLINE T* Data() noexcept
	{
		return Array;
	}
	
	FORCEINLINE const T* Data() const noexcept
	{
		return Array;
	}
	
	FORCEINLINE T& operator[](SizeType Index) noexcept
	{
		return At(Index);
	}
	
	FORCEINLINE const T& operator[](SizeType Index) const noexcept
	{
		return At(Index);
	}
	
	/*
	 * STL Iterators for range-based for loops
	 */

public:
	FORCEINLINE Iterator begin() noexcept
	{
		return Array;
	}
	
	FORCEINLINE Iterator end() noexcept
	{
		return Array + N;
	}
	
	FORCEINLINE ConstIterator begin() const noexcept
	{
		return Array;
	}
	
	FORCEINLINE ConstIterator end() const noexcept
	{
		return Array + N;
	}
	
	FORCEINLINE ConstIterator cbegin() const noexcept
	{
		return Array;
	}
	
	FORCEINLINE ConstIterator cend() const noexcept
	{
		return Array + N;
	}

public:
	T Array[N];
};
