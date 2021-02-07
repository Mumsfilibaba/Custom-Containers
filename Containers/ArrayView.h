#pragma once
#include "Utilities.h"
#include "Iterator.h"

template<typename T>
class TArrayView
{
public:
	typedef T* 		 				  Iterator;
	typedef const T* 				  ConstIterator;
	typedef TReverseIterator<T> 	  ReverseIterator;
	typedef TReverseIterator<const T> ConstReverseIterator;
	typedef UInt32 	 				  SizeType;

	TArrayView() noexcept
		: mArray(nullptr)
		, mSize(0)
	{
	}

	template<typename TArrayType>
	explicit TArrayView(TArrayType& Array) noexcept
		: mArray(Array.Data())
		, mSize(Array.Size())
	{
	}
	
	template<const SizeType N>
	explicit TArrayView(T(&Array)[N]) noexcept
		: mArray(Array)
		, mSize(N)
	{
	}

	template<typename TInput>
	explicit TArrayView(TInput Begin, TInput End) noexcept
		: mArray(Begin)
		, mSize(End - Begin)
	{
	}

	TArrayView(const TArrayView& Other) noexcept
		: mArray(Other.mArray)
		, mSize(Other.mSize)
	{
	}

	TArrayView(TArrayView&& Other) noexcept
		: mArray(Other.mArray)
		, mSize(Other.mSize)
	{
		Other.mArray = nullmArray;
		Other.mSize	 = 0;
	}

	T& Front() noexcept { return mArray[0]; }
	const T& Front() const noexcept { return mArray[0]; }

	T& Back() noexcept { return mArray[LastIndex()]; }
	const T& Back() const noexcept { return mArray[LastIndex()]; }

	T& At(SizeType Index) noexcept
	{
		VALIDATE(Index < mSize);
		return mArray[Index];
	}

	const T& At(SizeType Index) const noexcept
	{
		VALIDATE(Index < mSize);
		return mArray[Index];
	}

	void Swap(TArrayView& Other) noexcept
	{
		TArrayView TempView(::Move(*this));
		*this = ::Move(Other);
		Other = ::Move(TempView);
	}
	
	SizeType LastIndex() const noexcept { return mSize-1; }
	SizeType Size() const noexcept { return mSize; }
	SizeType SizeInBytes() const noexcept { return mSize * sizeof(T); }

	T* Data() noexcept { return mArray; }
	const T* Data() const noexcept { return mArray; }

	T& operator[](SizeType Index) noexcept { return At(Index); }
	const T& operator[](SizeType Index) const noexcept { return At(Index); }

	TArrayView& operator=(const TArrayView& Other) noexcept
	{
		mArray	= Other.mArray;
		mSize	= Other.mSize;
		return *this;
	}

	TArrayView& operator=(TArrayView&& Other) noexcept
	{
		if (this != &Other)
		{
			mArray		 = Other.mArray;
			mSize		 = Other.mSize;
			Other.mArray = nullptr;
			Other.mSize	 = 0;
		}

		return *this;
	}

	// STL iterator functions, Only here so that you can use Range for-loops
public:
	Iterator begin() noexcept { return Elements; } 
	Iterator end() noexcept { return Elements + N; }
	
	ConstIterator begin() const noexcept { return Elements; }
	ConstIterator end() const noexcept { return Elements + N; }
	
	ConstIterator cbegin() const noexcept { return Elements; }
	ConstIterator cend() const noexcept { return Elements + N; }

	ReverseIterator rbegin() noexcept { return ReverseIterator(end()); }
	ReverseIterator rend() noexcept { return ReverseIterator(begin()); }

	ConstReverseIterator rbegin() const noexcept { return ConstReverseIterator(end()); }
	ConstReverseIterator rend() const noexcept { return ConstReverseIterator(begin()); }

	ConstReverseIterator crbegin() const noexcept { return ConstReverseIterator(end()); }
	ConstReverseIterator crend() const noexcept { return ConstReverseIterator(begin()); }

private:
	T* 		 mArray;
	SizeType mSize;
};
