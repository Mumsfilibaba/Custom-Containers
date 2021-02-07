#pragma once
#include "Utilities.h"
#include "Iterator.h"

#include <initializer_list>
#include <iterator>

template<typename T>
class TArray
{
public:
	typedef T* 		 				  Iterator;
	typedef const T* 				  ConstIterator;
	typedef TReverseIterator<T> 	  ReverseIterator;
	typedef TReverseIterator<const T> ConstReverseIterator;
	typedef UInt32 	 				  SizeType;

	TArray() noexcept
		: mPtr(nullptr)
		, mSize(0)
		, mCapacity(0)
	{
	}

	explicit TArray(SizeType Size) noexcept
		: mPtr(nullptr)
		, mSize(0)
		, mCapacity(0)
	{
		InternalConstruct(Size);
	}

	explicit TArray(SizeType Size, const T& Value) noexcept
		: mPtr(nullptr)
		, mSize(0)
		, mCapacity(0)
	{
		InternalConstruct(Size, Value);
	}

	template<typename TInput>
	explicit TArray(TInput InBegin, TInput InEnd) noexcept
		: mPtr(nullptr)
		, mSize(0)
		, mCapacity(0)
	{
		InternalConstruct(InBegin, InEnd);
	}

	TArray(std::initializer_list<T> IList) noexcept
		: mPtr(nullptr)
		, mSize(0)
		, mCapacity(0)
	{
		InternalConstruct(IList.begin(), IList.end());
	}

	TArray(const TArray& Other) noexcept
		: mPtr(nullptr)
		, mSize(0)
		, mCapacity(0)
	{
		InternalConstruct(Other.begin(), Other.end());
	}

	TArray(TArray&& Other) noexcept
		: mPtr(nullptr)
		, mSize(0)
		, mCapacity(0)
	{
		InternalMove(::Move(Other));
	}

	~TArray() 
	{
		Clear();
		InternalReleaseData();
		mCapacity = 0;
	}

	void Clear() noexcept
	{
		InternalDestructRange(mPtr, mPtr + mSize);
		mSize = 0;
	}

	void Assign(SizeType Size) noexcept
	{
		Clear();
		InternalConstruct(Size);
	}

	void Assign(SizeType Size, const T& Value) noexcept
	{
		Clear();
		InternalConstruct(Size, Value);
	}

	template<typename TInput>
	void Assign(TInput InBegin, TInput InEnd) noexcept
	{
		Clear();
		InternalConstruct(InBegin, InEnd);
	}

	void Assign(std::initializer_list<T> IList) noexcept
	{
		Clear();
		InternalConstruct(IList.begin(), IList.end());
	}

	void Fill(const T& Value) noexcept
	{
		T* ArrayBegin	= mPtr;
		T* ArrayEnd		= ArrayBegin + mSize;

		while (ArrayBegin != ArrayEnd)
		{
			*ArrayBegin = Value;
			ArrayBegin++;
		}
	}

	void Fill(T&& Value) noexcept
	{
		T* ArrayBegin = mPtr;
		T* ArrayEnd = ArrayBegin + mSize;

		while (ArrayBegin != ArrayEnd)
		{
			*ArrayBegin = ::Move(Value);
			ArrayBegin++;
		}
	}

	void Resize(SizeType InSize) noexcept
	{
		if (InSize > mSize)
		{
			if (InSize >= mCapacity)
			{
				const SizeType NewCapacity = InternalGetResizeFactor(InSize);
				InternalRealloc(NewCapacity);
			}

			InternalDefaultConstructRange(mPtr + mSize, mPtr + InSize);
		}
		else if (InSize < mSize)
		{
			InternalDestructRange(mPtr + InSize, mPtr + mSize);
		}

		mSize = InSize;
	}

	void Resize(SizeType InSize, const T& Value) noexcept
	{
		if (InSize > mSize)
		{
			if (InSize >= mCapacity)
			{
				const SizeType NewCapacity = InternalGetResizeFactor(InSize);
				InternalRealloc(NewCapacity);
			}

			InternalCopyEmplace(InSize - mSize, Value, mPtr + mSize);
		}
		else if (InSize < mSize)
		{
			InternalDestructRange(mPtr + InSize, mPtr + mSize);
		}

		mSize = InSize;
	}

	void Reserve(SizeType InCapacity) noexcept
	{
		if (InCapacity != mCapacity)
		{
			SizeType OldSize = mSize;
			if (InCapacity < mSize)
			{
				mSize = InCapacity;
			}

			T* TempData = InternalAllocateElements(InCapacity);
			InternalMoveEmplace(mPtr, mPtr + mSize, TempData);
			InternalDestructRange(mPtr, mPtr + OldSize);

			InternalReleaseData();
			mPtr = TempData;
			mCapacity = InCapacity;
		}
	}

	template<typename... TArgs>
	T& EmplaceBack(TArgs&&... Args) noexcept
	{
		if (mSize >= mCapacity)
		{
			const SizeType NewCapacity = InternalGetResizeFactor();
			InternalRealloc(NewCapacity);
		}

		T* DataEnd = mPtr + mSize;
		new(reinterpret_cast<Void*>(DataEnd)) T(Forward<TArgs>(Args)...);
		mSize++;
		return (*DataEnd);
	}

	T& PushBack(const T& Element) noexcept
	{
		return EmplaceBack(Element);
	}

	T& PushBack(T&& Element) noexcept
	{
		return EmplaceBack(::Move(Element));
	}

	template<typename... TArgs>
	Iterator Emplace(ConstIterator Pos, TArgs&&... Args) noexcept
	{
		// Emplace back
		if (Pos == cend())
		{
			const SizeType OldSize = mSize;
			EmplaceBack(Forward<TArgs>(Args)...);
			return Iterator(mPtr + OldSize);
		}

		// Emplace
		const SizeType Index = InternalIndex(Pos);
		T* DataBegin = mPtr + Index;
		if (mSize >= mCapacity)
		{
			const SizeType NewCapacity = InternalGetResizeFactor();
			InternalEmplaceRealloc(NewCapacity, DataBegin, 1);
			DataBegin = mPtr + Index;
		}
		else
		{
			// Construct the range so that we can move to It
			T* DataEnd = mPtr + mSize;
			InternalDefaultConstructRange(DataEnd, DataEnd + 1);
			InternalMemmoveForward(DataBegin, DataEnd, DataEnd);
			InternalDestruct(DataBegin);
		}

		new (reinterpret_cast<Void*>(DataBegin)) T(Forward<TArgs>(Args)...);
		mSize++;
		return Iterator(DataBegin);
	}

	Iterator Insert(Iterator Pos, const T& Value) noexcept 
	{ 
		return Emplace(ConstIterator(Pos.Ptr), Value); 
	}

	Iterator Insert(Iterator Pos, T&& Value) noexcept 
	{ 
		return Emplace(ConstIterator(Pos.Ptr), ::Move(Value)); 
	}

	Iterator Insert(ConstIterator Pos, const T& Value) noexcept 
	{ 
		return Emplace(Pos, Value); 
	}

	Iterator Insert(ConstIterator Pos, T&& Value) noexcept 
	{ 	
		return Emplace(Pos, ::Move(Value)); 
	}

	Iterator Insert(Iterator Pos, std::initializer_list<T> IList) noexcept 
	{ 
		return Insert(ConstIterator(Pos.Ptr), IList);
	}

	Iterator Insert(ConstIterator Pos, std::initializer_list<T> IList) noexcept
	{
		// Insert at InEnd
		if (Pos == cend())
		{
			const SizeType OldSize = mSize;
			for (const T& Value : IList)
			{
				EmplaceBack(::Move(Value));
			}

			return Iterator(mPtr + OldSize);
		}

		// Insert
		const SizeType ListSize = static_cast<SizeType>(IList.size());
		const SizeType NewSize	= mSize + ListSize;
		const SizeType Index	= InternalIndex(Pos);

		T* RangeBegin = mPtr + Index;
		if (NewSize >= mCapacity)
		{
			const SizeType NewCapacity = InternalGetResizeFactor(NewSize);
			InternalEmplaceRealloc(NewCapacity, RangeBegin, ListSize);
			RangeBegin = mPtr + Index;
		}
		else
		{
			// Construct the range so that we can move to It
			T* DataEnd = mPtr + mSize;
			T* NewDataEnd = mPtr + mSize + ListSize;
			T* RangeEnd = RangeBegin + ListSize;
			InternalDefaultConstructRange(DataEnd, NewDataEnd);
			InternalMemmoveForward(RangeBegin, DataEnd, NewDataEnd - 1);
			InternalDestructRange(RangeBegin, RangeEnd);
		}

		// TODO: Get rid of const_cast
		InternalMoveEmplace(const_cast<T*>(IList.begin()), const_cast<T*>(IList.end()), RangeBegin);
		mSize = NewSize;
		return Iterator(RangeBegin);
	}

	template<typename TInput>
	Iterator Insert(Iterator Pos, TInput InBegin, TInput InEnd) noexcept
	{
		return Insert(ConstIterator(Pos.Ptr), InBegin, InEnd);
	}

	template<typename TInput>
	Iterator Insert(ConstIterator Pos, TInput InBegin, TInput InEnd) noexcept
	{
		// Insert at InEnd
		if (Pos == cend())
		{
			const SizeType OldSize = mSize;
			for (TInput It = InBegin; It != InEnd; It++)
			{
				EmplaceBack(*It);
			}

			return Iterator(mPtr + OldSize);
		}

		// Insert
		const SizeType rangeSize = InternalDistance(InBegin, InEnd);
		const SizeType NewSize = mSize + rangeSize;
		const SizeType Index = InternalIndex(Pos);

		T* RangeBegin = mPtr + Index;
		if (NewSize >= mCapacity)
		{
			const SizeType NewCapacity = InternalGetResizeFactor(NewSize);
			InternalEmplaceRealloc(NewCapacity, RangeBegin, rangeSize);
			RangeBegin = mPtr + Index;
		}
		else
		{
			// Construct the range so that we can move to it
			T* DataEnd = mPtr + mSize;
			T* NewDataEnd = mPtr + mSize + rangeSize;
			T* RangeEnd = RangeBegin + rangeSize;
			InternalDefaultConstructRange(DataEnd, NewDataEnd);
			InternalMemmoveForward(RangeBegin, DataEnd, NewDataEnd - 1);
			InternalDestructRange(RangeBegin, RangeEnd);
		}

		InternalCopyEmplace(InBegin.Ptr, InEnd.Ptr, RangeBegin);
		mSize = NewSize;
		return Iterator(RangeBegin);
	}

	void PopBack() noexcept
	{
		if (mSize > 0)
		{
			InternalDestruct(mPtr + (--mSize));
		}
	}

	Iterator Erase(Iterator Pos) noexcept
	{
		return Erase(ConstIterator(Pos.Ptr));
	}

	Iterator Erase(ConstIterator Pos) noexcept
	{
		VALIDATE(InternalIsIteratorOwner(Pos));

		// Erase at InEnd
		if (Pos == cend())
		{
			PopBack();
			return End();
		}

		// Erase
		const SizeType Index = InternalDistance(cbegin(), Pos);
		T* dataBegin = mPtr + Index;
		T* DataEnd = mPtr + mSize;
		InternalMemmoveBackwards(dataBegin + 1, DataEnd, dataBegin);
		InternalDestruct(DataEnd - 1);

		mSize--;
		return Iterator(dataBegin);
	}

	Iterator Erase(Iterator InBegin, Iterator InEnd) noexcept
	{
		return Erase(ConstIterator(InBegin.Ptr), ConstIterator(InEnd.Ptr));
	}

	Iterator Erase(ConstIterator InBegin, ConstIterator InEnd) noexcept
	{
		VALIDATE(InBegin < InEnd);
		VALIDATE(InternalIsRangeOwner(InBegin, InEnd));

		T* dataBegin = mPtr + InternalIndex(InBegin);
		T* DataEnd = mPtr + InternalIndex(InEnd);

		const SizeType elementCount = InternalDistance(dataBegin, DataEnd);
		if (InEnd >= cend())
		{
			InternalDestructRange(dataBegin, DataEnd);
		}
		else
		{
			T* realEnd = mPtr + mSize;
			InternalMemmoveBackwards(DataEnd, realEnd, dataBegin);
			InternalDestructRange(realEnd - elementCount, realEnd);
		}

		mSize -= elementCount;
		return Iterator(dataBegin);
	}

	void Swap(TArray& Other) noexcept
	{
		if (this != std::addressof(Other))
		{
			TArray TempArray(::Move(*this));
			*this = ::Move(Other);
			Other = ::Move(TempArray);
		}
	}

	void ShrinkToFit() noexcept
	{
		if (mCapacity > mSize)
		{
			InternalRealloc(mSize);
		}
	}

	Bool IsEmpty() const noexcept
	{
		return (mSize == 0);
	}

	T& Front() noexcept
	{
		VALIDATE(!IsEmpty());
		return mPtr[0];
	}

	Iterator Begin() noexcept
	{
		return Iterator(mPtr);
	}

	Iterator End() noexcept
	{
		return Iterator(mPtr + mSize);
	}

	ConstIterator Begin() const noexcept
	{
		return Iterator(mPtr);
	}

	ConstIterator End() const noexcept
	{
		return Iterator(mPtr + mSize);
	}

	const T& Front() const noexcept
	{
		VALIDATE(mSize > 0);
		return mPtr[0];
	}

	T& Back() noexcept
	{
		VALIDATE(mSize > 0);
		return mPtr[mSize - 1];
	}

	const T& Back() const noexcept
	{
		VALIDATE(mSize > 0);
		return mPtr[mSize - 1];
	}

	T* Data() noexcept { return mPtr; }
	const T* Data() const noexcept { return mPtr; }

	SizeType Size() const noexcept { return mSize; }
	SizeType SizeInBytes() const noexcept { return mSize * sizeof(T); }

	SizeType Capacity() const noexcept { return mCapacity; }
	SizeType CapacityInBytes() const noexcept { return mCapacity * sizeof(T); }

	T& At(SizeType Index) noexcept
	{
		VALIDATE(Index < mSize);
		return mPtr[Index];
	}

	const T& At(SizeType Index) const noexcept
	{
		VALIDATE(Index < mSize);
		return mPtr[Index];
	}

	TArray& operator=(const TArray& Other) noexcept
	{
		if (this != std::addressof(Other))
		{
			Clear();
			InternalConstruct(Other.Begin(), Other.End());
		}

		return *this;
	}

	TArray& operator=(TArray&& Other) noexcept
	{
		if (this != std::addressof(Other))
		{
			Clear();
			InternalMove(::Move(Other));
		}

		return *this;
	}

	TArray& operator=(std::initializer_list<T> IList) noexcept
	{
		Assign(IList);
		return *this;
	}

	T& operator[](SizeType Index) noexcept { return At(Index); }
	const T& operator[](SizeType Index) const noexcept { return At(Index); }

	// STL iterator functions, Only here so that you can use Range for-loops
public:
	Iterator begin() noexcept { return Iterator(mPtr); }
	Iterator end() noexcept { return Iterator(mPtr + mSize); }

	ConstIterator begin() const noexcept { return ConstIterator(mPtr); } 
	ConstIterator end() const noexcept { return ConstIterator(mPtr + mSize); }

	ConstIterator cbegin() const noexcept { return ConstIterator(mPtr); }
  	ConstIterator cend() const noexcept { return ConstIterator(mPtr + mSize); }

	ReverseIterator rbegin() noexcept { return ReverseIterator(mPtr + mSize); }
	ReverseIterator rend() noexcept { return ReverseIterator(mPtr); }

	ReverseConstIterator rbegin() const noexcept { return ReverseConstIterator(mPtr + mSize); }
	ReverseConstIterator rend() const noexcept { return ReverseConstIterator(mPtr); }
	
	ReverseConstIterator crbegin() const noexcept { return ReverseConstIterator(mPtr + mSize); }
	ReverseConstIterator crend() const noexcept { return ReverseConstIterator(mPtr); }

private:
	// Check is the iterator belongs to this TArray
	Bool InternalIsRangeOwner(ConstIterator InBegin, ConstIterator InEnd) const noexcept
	{
		return (InBegin < InEnd) && (InBegin >= begin()) && (InEnd <= end());
	}

	Bool InternalIsIteratorOwner(ConstIterator It) const noexcept
	{
		return (It >= begin()) && (It <= end());
	}

	// Helpers
	template<typename TInput>
	FORCEINLINE const T* InternalUnwrapConst(TInput It) noexcept
	{
		if constexpr (std::is_pointer<TInput>())
		{
			return It;
		}
		else
		{
			return It.Ptr;
		}
	}

	template<typename TInput>
	FORCEINLINE SizeType InternalDistance(TInput InBegin, TInput InEnd) noexcept
	{
		constexpr bool IsPointer = std::is_pointer<TInput>();
		constexpr bool IsCustomIterator = std::is_same<TInput, Iterator>() || std::is_same<TInput, ConstIterator>();

		// Handle outside pointers
		if constexpr (IsPointer || IsCustomIterator)
		{
			return static_cast<SizeType>(InternalUnwrapConst(InEnd) - InternalUnwrapConst(InBegin));
		}
		else
		{
			return static_cast<SizeType>(std::distance(InBegin, InEnd));
		}
	}

	template<typename TInput>
	FORCEINLINE SizeType InternalIndex(TInput Pos) noexcept
	{
		return static_cast<SizeType>(InternalUnwrapConst(Pos) - InternalUnwrapConst(begin()));
	}

	FORCEINLINE SizeType InternalGetResizeFactor() const noexcept
	{
		return InternalGetResizeFactor(mSize);
	}

	FORCEINLINE SizeType InternalGetResizeFactor(SizeType BaseSize) const noexcept
	{
		return BaseSize + (mCapacity / 2) + 1;
	}

	FORCEINLINE T* InternalAllocateElements(SizeType InCapacity) noexcept
	{
		constexpr SizeType ElementByteSize = sizeof(T);
		const SizeType SizeInBytes = ElementByteSize * InCapacity;
		return reinterpret_cast<T*>(malloc(static_cast<size_t>(SizeInBytes)));
	}

	FORCEINLINE void InternalReleaseData() noexcept
	{
		if (mPtr)
		{
			free(mPtr);
			mPtr = nullptr;
		}
	}

	FORCEINLINE void InternalAllocData(SizeType InCapacity) noexcept
	{
		if (InCapacity > mCapacity)
		{
			InternalReleaseData();
			mPtr = InternalAllocateElements(InCapacity);
			mCapacity = InCapacity;
		}
	}

	FORCEINLINE void InternalRealloc(SizeType InCapacity) noexcept
	{
		T* TempData = InternalAllocateElements(InCapacity);
		InternalMoveEmplace(mPtr, mPtr + mSize, TempData);
		InternalDestructRange(mPtr, mPtr + mSize);

		InternalReleaseData();
		mPtr = TempData;
		mCapacity = InCapacity;
	}

	FORCEINLINE void InternalEmplaceRealloc(SizeType InCapacity, T* EmplacePos, SizeType Count) noexcept
	{
		VALIDATE(InCapacity >= mSize + Count);

		const SizeType Index = InternalIndex(EmplacePos);
		T* TempData = InternalAllocateElements(InCapacity);
		InternalMoveEmplace(mPtr, EmplacePos, TempData);
		if (EmplacePos != mPtr + mSize)
		{
			InternalMoveEmplace(EmplacePos, mPtr + mSize, TempData + Index + Count);
		}

		InternalDestructRange(mPtr, mPtr + mSize);

		InternalReleaseData();
		mPtr = TempData;

		mCapacity = InCapacity;
	}

	// Construct
	FORCEINLINE void InternalConstruct(SizeType InSize) noexcept
	{
		if (InSize > 0)
		{
			InternalAllocData(InSize);
			mSize = InSize;
			InternalDefaultConstructRange(mPtr, mPtr + InSize);
		}
	}

	FORCEINLINE void InternalConstruct(SizeType InSize, const T& Value) noexcept
	{
		if (InSize > 0)
		{
			InternalAllocData(InSize);
			InternalCopyEmplace(InSize, Value, mPtr);
			mSize = InSize;
		}
	}

	template<typename TInput>
	FORCEINLINE void InternalConstruct(TInput InBegin, TInput InEnd) noexcept
	{
		const SizeType Distance = InternalDistance(InBegin, InEnd);
		if (Distance > 0)
		{
			InternalAllocData(Distance);
			InternalCopyEmplace(InBegin, InEnd, mPtr);
			mSize = Distance;
		}
	}

	FORCEINLINE void InternalMove(TArray&& Other) noexcept
	{
		InternalReleaseData();

		mPtr 	  = Other.mPtr;
		mSize 	  = Other.mSize;
		mCapacity = Other.mCapacity;

		Other.mPtr 	    = nullptr;
		Other.mSize 	= 0;
		Other.mCapacity = 0;
	}

	// Emplace
	template<typename TInput>
	FORCEINLINE void InternalCopyEmplace(TInput InBegin, TInput InEnd, T* Dest) noexcept
	{
		// This function assumes that there is no overlap
		constexpr bool IsTrivial = std::is_trivially_copy_constructible<T>();
		constexpr bool IsPointer = std::is_pointer<TInput>();
		constexpr bool IsCustomIterator = std::is_same<TInput, Iterator>() || std::is_same<TInput, ConstIterator>();

		if constexpr (IsTrivial && (IsPointer || IsCustomIterator))
		{
			const SizeType Count = InternalDistance(InBegin, InEnd);
			const SizeType CpySize = Count * sizeof(T);
			memcpy(Dest, InternalUnwrapConst(InBegin), CpySize);
		}
		else
		{
			while (InBegin != InEnd)
			{
				new(reinterpret_cast<Void*>(Dest)) T(*InBegin);
				InBegin++;
				Dest++;
			}
		}
	}

	FORCEINLINE void InternalCopyEmplace(SizeType Size, const T& Value, T* Dest) noexcept
	{
		T* ItEnd = Dest + Size;
		while (Dest != ItEnd)
		{
			new(reinterpret_cast<Void*>(Dest)) T(Value);
			Dest++;
		}
	}

	FORCEINLINE void InternalMoveEmplace(T* InBegin, T* InEnd, T* Dest) noexcept
	{
		// This function assumes that there is no overlap
		if constexpr (std::is_trivially_move_constructible<T>())
		{
			const SizeType Count = InternalDistance(InBegin, InEnd);
			const SizeType CpySize = Count * sizeof(T);
			memcpy(Dest, InBegin, CpySize);
		}
		else
		{
			while (InBegin != InEnd)
			{
				new(reinterpret_cast<Void*>(Dest)) T(::Move(*InBegin));
				InBegin++;
				Dest++;
			}
		}
	}

	FORCEINLINE void InternalMemmoveBackwards(T* InBegin, T* InEnd, T* Dest) noexcept
	{
		VALIDATE(InBegin <= InEnd);
		if (InBegin == InEnd)
		{
			return;
		}

		VALIDATE(InEnd <= mPtr + mCapacity);

		// ::Move each object in the range to the destination
		const SizeType Count = InternalDistance(InBegin, InEnd);
		if constexpr (std::is_trivially_move_assignable<T>())
		{
			const SizeType CpySize = Count * sizeof(T);
			memmove(Dest, InBegin, CpySize); // Assumes that data can overlap
		}
		else
		{
			while (InBegin != InEnd)
			{
				if constexpr (std::is_move_assignable<T>())
				{
					(*Dest) = ::Move(*InBegin);
				}
				else if constexpr (std::is_copy_assignable<T>())
				{
					(*Dest) = (*InBegin);
				}

				Dest++;
				InBegin++;
			}
		}
	}

	FORCEINLINE void InternalMemmoveForward(T* InBegin, T* InEnd, T* Dest) noexcept
	{
		// ::Move each object in the range to the destination, starts in the "End" and moves forward
		const SizeType Count = InternalDistance(InBegin, InEnd);
		if constexpr (std::is_trivially_move_assignable<T>())
		{
			if (Count > 0)
			{
				const SizeType CpySize = Count * sizeof(T);
				const SizeType OffsetSize = (Count - 1) * sizeof(T);
				memmove(reinterpret_cast<Char*>(Dest) - OffsetSize, InBegin, CpySize);
			}
		}
		else
		{
			while (InEnd != InBegin)
			{
				InEnd--;
				if constexpr (std::is_move_assignable<T>())
				{
					(*Dest) = ::Move(*InEnd);
				}
				else if constexpr (std::is_copy_assignable<T>())
				{
					(*Dest) = (*InEnd);
				}
				Dest--;
			}
		}
	}

	FORCEINLINE void InternalDestruct(const T* Pos) noexcept
	{
		// Calls the destructor (If It needs to be called)
		if constexpr (std::is_trivially_destructible<T>() == false)
		{
			(*Pos).~T();
		}
	}

	FORCEINLINE void InternalDestructRange(const T* InBegin, const T* InEnd) noexcept
	{
		VALIDATE(InBegin <= InEnd);
		VALIDATE(InEnd - InBegin <= mCapacity);

		// Calls the destructor for every object in the range (If It needs to be called)
		if constexpr (std::is_trivially_destructible<T>() == false)
		{
			while (InBegin != InEnd)
			{
				InternalDestruct(InBegin);
				InBegin++;
			}
		}
	}

	FORCEINLINE void InternalDefaultConstructRange(T* InBegin, T* InEnd) noexcept
	{
		VALIDATE(InBegin <= InEnd);

		// Calls the default constructor for every object in the range (If It can be called)
		if constexpr (std::is_default_constructible<T>())
		{
			while (InBegin != InEnd)
			{
				new(reinterpret_cast<Void*>(InBegin)) T();
				InBegin++;
			}
		}
	}

private:
	T* 		 mPtr;
	SizeType mSize;
	SizeType mCapacity;
};
