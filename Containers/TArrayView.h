#pragma once
#include "TUtilities.h"

/*
 * TArrayView
 */

template<typename T>
class TArrayView
{
	using Iterator		= T*;
	using ConstIterator	= const T*;
	using SizeType		= UInt32;

public:
	FORCEINLINE TArrayView() noexcept
		: ViewPtr(nullptr)
		, ViewSize(0)
	{
	}

	template<typename TArrayType>
	FORCEINLINE explicit TArrayView(TArrayType& Array) noexcept
		: ViewPtr(Array.Data())
		, ViewSize(Array.Size())
	{
	}
	
	template<const SizeType N>
	FORCEINLINE explicit TArrayView(T(&Array)[N]) noexcept
		: ViewPtr(Array)
		, ViewSize(N)
	{
	}

	template<typename TInputIterator>
	FORCEINLINE explicit TArrayView(TInputIterator Begin, TInputIterator End) noexcept
		: ViewPtr(Begin)
		, ViewSize(SizeType(End - Begin))
	{
	}

	FORCEINLINE TArrayView(const TArrayView& Other) noexcept
		: ViewPtr(Other.ViewPtr)
		, ViewSize(Other.ViewSize)
	{
	}

	FORCEINLINE TArrayView(TArrayView&& Other) noexcept
		: ViewPtr(Other.ViewPtr)
		, ViewSize(Other.ViewSize)
	{
		Other.ViewPtr	= nullptr;
		Other.ViewSize	= 0;
	}

	FORCEINLINE T& Front() noexcept
	{
		return ViewPtr[0];
	}

	FORCEINLINE const T& Front() const noexcept
	{
		return ViewPtr[0];
	}

	FORCEINLINE T& Back() noexcept
	{
		return ViewPtr[ViewSize - 1];
	}

	FORCEINLINE const T& Back() const noexcept
	{
		return ViewPtr[ViewSize - 1];
	}

	FORCEINLINE T& At(SizeType Index) noexcept
	{
		VALIDATE(Index < ViewSize);
		return ViewPtr[Index];
	}

	FORCEINLINE const T& At(SizeType Index) const noexcept
	{
		VALIDATE(Index < ViewSize);
		return ViewPtr[Index];
	}

	FORCEINLINE void Swap(TArrayView& Other) noexcept
	{
		TArrayView TempView(::Move(*this));
		*this = ::Move(Other);
		Other = ::Move(TempView);
	}
	
	FORCEINLINE SizeType Size() const noexcept
	{
		return ViewSize;
	}

	FORCEINLINE SizeType SizeInBytes() const noexcept
	{
		return ViewSize * sizeof(T);
	}

	FORCEINLINE T* Data() noexcept
	{
		return ViewPtr;
	}

	FORCEINLINE const T* Data() const noexcept
	{
		return ViewPtr;
	}

	FORCEINLINE T& operator[](SizeType Index) noexcept
	{
		return At(Index);
	}

	FORCEINLINE const T& operator[](SizeType Index) const noexcept
	{
		return At(Index);
	}

	FORCEINLINE TArrayView& operator=(const TArrayView& Other) noexcept
	{
		ViewPtr		= Other.ViewPtr;
		ViewSize	= Other.ViewSize;
		return *this;
	}

	FORCEINLINE TArrayView& operator=(TArrayView&& Other) noexcept
	{
		if (this != &Other)
		{
			ViewPtr		= Other.ViewPtr;
			ViewSize	= Other.ViewSize;
			Other.ViewPtr	= nullptr;
			Other.ViewSize	= 0;
		}

		return *this;
	}

	/*
	* STL iterator functions, Only here so that you can use Range for-loops
	*/

public:
	FORCEINLINE Iterator begin() noexcept 
	{
		return ViewPtr;
	}

	FORCEINLINE Iterator end() noexcept
	{
		return ViewPtr + ViewSize;
	}

	FORCEINLINE ConstIterator begin() const noexcept
	{
		return ViewPtr;
	}

	FORCEINLINE ConstIterator end() const noexcept
	{
		return ViewPtr + ViewSize;
	}

	FORCEINLINE ConstIterator cbegin() const noexcept
	{
		return ViewPtr;
	}

	FORCEINLINE ConstIterator cend() const noexcept
	{
		return ViewPtr + ViewSize;
	}

private:
	T* ViewPtr;
	SizeType ViewSize;
};
