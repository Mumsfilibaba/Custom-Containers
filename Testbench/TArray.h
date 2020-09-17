#pragma once
#include "TypeUtilities.h"

#include <iterator>

/*
* Dynamic Array similar to std::vector
*/
template<typename T>
class TArray
{
public:
	typedef Uint32 SizeType;

	/*
	* TIterator
	*/
	template<typename TIteratorType>
	class TIterator
	{
		friend class TArray;

	public:
		using iterator_category	= std::random_access_iterator_tag;
		using difference_type	= SizeType;
		using value_type		= TIteratorType;
		using pointer			= TIteratorType*;
		using reference			= TIteratorType&;

		~TIterator() = default;

		FORCEINLINE TIterator(TIteratorType* InPtr = nullptr)
			: Ptr(InPtr)
		{
		}

		FORCEINLINE TIteratorType* operator->() const
		{
			return Ptr;
		}

		FORCEINLINE TIteratorType& operator*() const
		{
			return *Ptr;
		}

		FORCEINLINE TIterator operator++()
		{
			Ptr++;
			return *this;
		}

		FORCEINLINE TIterator operator++(Int32)
		{
			TIterator Temp = *this;
			Ptr++;
			return Temp;
		}

		FORCEINLINE TIterator operator--()
		{
			Ptr--;
			return *this;
		}

		FORCEINLINE TIterator operator--(Int32)
		{
			TIterator Temp = *this;
			Ptr--;
			return Temp;
		}

		FORCEINLINE TIterator operator+(Int32 Offset) const
		{
			TIterator Temp = *this;
			return Temp += Offset;
		}

		FORCEINLINE TIterator operator-(Int32 Offset) const
		{
			TIterator Temp = *this;
			return Temp -= Offset;
		}

		FORCEINLINE difference_type operator-(TIterator Other) const
		{
			return static_cast<difference_type>(Ptr - Other.Ptr);
		}

		FORCEINLINE TIterator& operator+=(Int32 Offset)
		{
			Ptr += Offset;
			return *this;
		}

		FORCEINLINE TIterator& operator-=(Int32 Offset)
		{
			Ptr -= Offset;
			return *this;
		}

		FORCEINLINE bool operator==(const TIterator& Other) const
		{
			return (Ptr == Other.Ptr);
		}

		FORCEINLINE bool operator!=(const TIterator& Other) const
		{
			return (Ptr != Other.Ptr);
		}

		FORCEINLINE bool operator<(const TIterator& Other) const
		{
			return Ptr < Other.Ptr;
		}

		FORCEINLINE bool operator<=(const TIterator& Other) const
		{
			return Ptr <= Other.Ptr;
		}

		FORCEINLINE bool operator>(const TIterator& Other) const
		{
			return Ptr > Other.Ptr;
		}

		FORCEINLINE bool operator>=(const TIterator& Other) const
		{
			return Ptr >= Other.Ptr;
		}

		FORCEINLINE operator TIterator<const TIteratorType>() const
		{
			return TIterator<const TIteratorType>(Ptr);
		}

	protected:
		TIteratorType* Ptr;
	};

	typedef TIterator<T>			Iterator;
	typedef TIterator<const T>	ConstIterator;

	/*
	* Reverse Iterator: Stores for example End(), but will reference End() - 1
	*/
	template<typename TIteratorType>
	class TReverseIterator
	{
		friend class TArray;

	public:
		~TReverseIterator() = default;

		FORCEINLINE TReverseIterator(TIteratorType* InPtr = nullptr)
			: Ptr(InPtr)
		{
		}

		FORCEINLINE TIteratorType* operator->() const
		{
			return (Ptr - 1);
		}

		FORCEINLINE TIteratorType& operator*() const
		{
			return *(Ptr - 1);
		}

		FORCEINLINE TReverseIterator operator++()
		{
			Ptr--;
			return *this;
		}

		FORCEINLINE TReverseIterator operator++(Int32)
		{
			TReverseIterator Temp = *this;
			Ptr--;
			return Temp;
		}

		FORCEINLINE TReverseIterator operator--()
		{
			Ptr++;
			return *this;
		}

		FORCEINLINE TReverseIterator operator--(Int32)
		{
			TReverseIterator Temp = *this;
			Ptr++;
			return Temp;
		}

		FORCEINLINE TReverseIterator operator+(Int32 Offset) const
		{
			TReverseIterator Temp = *this;
			return Temp += Offset;
		}

		FORCEINLINE TReverseIterator operator-(Int32 Offset) const
		{
			TReverseIterator Temp = *this;
			return Temp -= Offset;
		}

		FORCEINLINE TReverseIterator& operator+=(Int32 Offset)
		{
			Ptr -= Offset;
			return *this;
		}

		FORCEINLINE TReverseIterator& operator-=(Int32 Offset)
		{
			Ptr += Offset;
			return *this;
		}

		FORCEINLINE bool operator==(const TReverseIterator& Other) const
		{
			return (Ptr == Other.Ptr);
		}

		FORCEINLINE bool operator!=(const TReverseIterator& Other) const
		{
			return (Ptr != Other.Ptr);
		}

	protected:
		TIteratorType* Ptr;
	};

	typedef TReverseIterator<T>			ReverseIterator;
	typedef TReverseIterator<const T>	ReverseConstIterator;

	/*
	* TArray API
	*/
public:
	FORCEINLINE TArray() noexcept
		: DataPtr(nullptr)
		, DataSize(0)
		, DataCapacity(0)
	{
	}

	FORCEINLINE explicit TArray(SizeType Size) noexcept
		: DataPtr(nullptr)
		, DataSize(0)
		, DataCapacity(0)
	{
		InternalConstruct(Size);
	}

	FORCEINLINE explicit TArray(SizeType Size, const T& Value) noexcept
		: DataPtr(nullptr)
		, DataSize(0)
		, DataCapacity(0)
	{
		InternalConstruct(Size, Value);
	}

	template<typename TInputIt>
	FORCEINLINE explicit TArray(TInputIt InBegin, TInputIt InEnd) noexcept
		: DataPtr(nullptr)
		, DataSize(0)
		, DataCapacity(0)
	{
		InternalConstruct(InBegin, InEnd);
	}

	FORCEINLINE TArray(std::initializer_list<T> IList) noexcept
		: DataPtr(nullptr)
		, DataSize(0)
		, DataCapacity(0)
	{
		InternalConstruct(IList.begin(), IList.end());
	}

	FORCEINLINE TArray(const TArray& Other) noexcept
		: DataPtr(nullptr)
		, DataSize(0)
		, DataCapacity(0)
	{
		InternalConstruct(Other.begin(), Other.end());
	}

	FORCEINLINE TArray(TArray&& Other) noexcept
		: DataPtr(nullptr)
		, DataSize(0)
		, DataCapacity(0)
	{
		InternalMove(::Move(Other));
	}

	FORCEINLINE ~TArray()
	{
		Clear();
		InternalReleaseData();
		DataCapacity = 0;
	}

	FORCEINLINE void Clear() noexcept
	{
		InternalDestructRange(DataPtr, DataPtr + DataSize);
		DataSize = 0;
	}

	FORCEINLINE void Assign(SizeType Size) noexcept
	{
		Clear();
		InternalConstruct(Size);
	}

	FORCEINLINE void Assign(SizeType Size, const T& Value) noexcept
	{
		Clear();
		InternalConstruct(Size, Value);
	}

	template<typename TInputIt>
	FORCEINLINE void Assign(TInputIt InBegin, TInputIt InEnd) noexcept
	{
		Clear();
		InternalConstruct(InBegin, InEnd);
	}

	FORCEINLINE void Assign(std::initializer_list<T> IList) noexcept
	{
		Clear();
		InternalConstruct(IList.begin(), IList.end());
	}

	FORCEINLINE void Resize(SizeType InSize) noexcept
	{
		if (InSize > DataSize)
		{
			if (InSize >= DataCapacity)
			{
				const SizeType NewCapacity = InternalGetResizeFactor(InSize);
				InternalRealloc(NewCapacity);
			}

			InternalDefaultConstructRange(DataPtr + DataSize, DataPtr + InSize);
		}
		else if (InSize < DataSize)
		{
			InternalDestructRange(DataPtr + InSize, DataPtr + DataSize);
		}

		DataSize = InSize;
	}

	FORCEINLINE void Resize(SizeType InSize, const T& Value) noexcept
	{
		if (InSize > DataSize)
		{
			if (InSize >= DataCapacity)
			{
				const SizeType NewCapacity = InternalGetResizeFactor(InSize);
				InternalRealloc(NewCapacity);
			}

			InternalCopyEmplace(InSize - DataSize, Value, DataPtr + DataSize);
		}
		else if (InSize < DataSize)
		{
			InternalDestructRange(DataPtr + InSize, DataPtr + DataSize);
		}

		DataSize = InSize;
	}

	FORCEINLINE void Reserve(SizeType InCapacity) noexcept
	{
		if (InCapacity != DataCapacity)
		{
			SizeType OldSize = DataSize;
			if (InCapacity < DataSize)
			{
				DataSize = InCapacity;
			}

			T* TempData = InternalAllocateElements(InCapacity);
			InternalMoveEmplace(DataPtr, DataPtr + DataSize, TempData);
			InternalDestructRange(DataPtr, DataPtr + OldSize);

			InternalReleaseData();
			DataPtr = TempData;
			DataCapacity = InCapacity;
		}
	}

	template<typename... TArgs>
	FORCEINLINE T& EmplaceBack(TArgs&&... Args) noexcept
	{
		if (DataSize >= DataCapacity)
		{
			const SizeType NewCapacity = InternalGetResizeFactor();
			InternalRealloc(NewCapacity);
		}

		T* DataEnd = DataPtr + DataSize;
		new(reinterpret_cast<VoidPtr>(DataEnd)) T(Forward<TArgs>(Args)...);
		DataSize++;
		return (*DataEnd);
	}

	FORCEINLINE T& PushBack(const T& Element) noexcept
	{
		return EmplaceBack(Element);
	}

	FORCEINLINE T& PushBack(T&& Element) noexcept
	{
		return EmplaceBack(::Move(Element));
	}

	template<typename... TArgs>
	FORCEINLINE Iterator Emplace(ConstIterator Pos, TArgs&&... Args) noexcept
	{
		// Emplace back
		if (Pos == cend())
		{
			const SizeType OldSize = DataSize;
			EmplaceBack(Forward<TArgs>(Args)...);
			return Iterator(DataPtr + OldSize);
		}

		// Emplace
		const SizeType Index = InternalIndex(Pos);
		T* DataBegin = DataPtr + Index;
		if (DataSize >= DataCapacity)
		{
			const SizeType NewCapacity = InternalGetResizeFactor();
			InternalEmplaceRealloc(NewCapacity, DataBegin, 1);
			DataBegin = DataPtr + Index;
		}
		else
		{
			// Construct the range so that we can move to It
			T* DataEnd = DataPtr + DataSize;
			InternalDefaultConstructRange(DataEnd, DataEnd + 1);
			InternalMemmoveForward(DataBegin, DataEnd, DataEnd);
			InternalDestruct(DataBegin);
		}

		new (reinterpret_cast<VoidPtr>(DataBegin)) T(Forward<TArgs>(Args)...);
		DataSize++;
		return Iterator(DataBegin);
	}

	FORCEINLINE Iterator Insert(Iterator Pos, const T& Value) noexcept
	{
		return Emplace(ConstIterator(Pos.Ptr), Value);
	}

	FORCEINLINE Iterator Insert(Iterator Pos, T&& Value) noexcept
	{
		return Emplace(ConstIterator(Pos.Ptr), ::Move(Value));
	}

	FORCEINLINE Iterator Insert(ConstIterator Pos, const T& Value) noexcept
	{
		return Emplace(Pos, Value);
	}

	FORCEINLINE Iterator Insert(ConstIterator Pos, T&& Value) noexcept
	{
		return Emplace(Pos, ::Move(Value));
	}

	FORCEINLINE Iterator Insert(Iterator Pos, std::initializer_list<T> IList) noexcept
	{
		return Insert(ConstIterator(Pos.Ptr), IList);
	}

	FORCEINLINE Iterator Insert(ConstIterator Pos, std::initializer_list<T> IList) noexcept
	{
		// Insert at InEnd
		if (Pos == cend())
		{
			const SizeType OldSize = DataSize;
			for (const T& Value : IList)
			{
				EmplaceBack(::Move(Value));
			}

			return Iterator(DataPtr + OldSize);
		}

		// Insert
		const SizeType ListSize = static_cast<SizeType>(IList.size());
		const SizeType NewSize = DataSize + ListSize;
		const SizeType Index = InternalIndex(Pos);

		T* RangeBegin = DataPtr + Index;
		if (NewSize >= DataCapacity)
		{
			const SizeType NewCapacity = InternalGetResizeFactor(NewSize);
			InternalEmplaceRealloc(NewCapacity, RangeBegin, ListSize);
			RangeBegin = DataPtr + Index;
		}
		else
		{
			// Construct the range so that we can move to It
			T* DataEnd = DataPtr + DataSize;
			T* NewDataEnd = DataPtr + DataSize + ListSize;
			T* RangeEnd = RangeBegin + ListSize;
			InternalDefaultConstructRange(DataEnd, NewDataEnd);
			InternalMemmoveForward(RangeBegin, DataEnd, NewDataEnd - 1);
			InternalDestructRange(RangeBegin, RangeEnd);
		}

		// TODO: Get rid of const_cast
		InternalMoveEmplace(const_cast<T*>(IList.begin()), const_cast<T*>(IList.end()), RangeBegin);
		DataSize = NewSize;
		return Iterator(RangeBegin);
	}

	template<typename TInputIt>
	FORCEINLINE Iterator Insert(Iterator Pos, TInputIt InBegin, TInputIt InEnd) noexcept
	{
		return Insert(ConstIterator(Pos.Ptr), InBegin, InEnd);
	}

	template<typename TInputIt>
	FORCEINLINE Iterator Insert(ConstIterator Pos, TInputIt InBegin, TInputIt InEnd) noexcept
	{
		// Insert at InEnd
		if (Pos == ConstEnd())
		{
			const SizeType OldSize = DataSize;
			for (TInputIt It = InBegin; It != InEnd; It++)
			{
				EmplaceBack(*It);
			}

			return Iterator(DataPtr + OldSize);
		}

		// Insert
		const SizeType rangeSize = InternalDistance(InBegin, InEnd);
		const SizeType NewSize = DataSize + rangeSize;
		const SizeType Index = InternalIndex(Pos);

		T* RangeBegin = DataPtr + Index;
		if (NewSize >= DataCapacity)
		{
			const SizeType NewCapacity = InternalGetResizeFactor(NewSize);
			InternalEmplaceRealloc(NewCapacity, RangeBegin, rangeSize);
			RangeBegin = DataPtr + Index;
		}
		else
		{
			// Construct the range so that we can move to it
			T* DataEnd = DataPtr + DataSize;
			T* NewDataEnd = DataPtr + DataSize + rangeSize;
			T* RangeEnd = RangeBegin + rangeSize;
			InternalDefaultConstructRange(DataEnd, NewDataEnd);
			InternalMemmoveForward(RangeBegin, DataEnd, NewDataEnd - 1);
			InternalDestructRange(RangeBegin, RangeEnd);
		}

		InternalCopyEmplace(InBegin.Ptr, InEnd.Ptr, RangeBegin);
		DataSize = NewSize;
		return Iterator(RangeBegin);
	}

	FORCEINLINE void PopBack() noexcept
	{
		if (DataSize > 0)
		{
			InternalDestruct(DataPtr + (--DataSize));
		}
	}

	FORCEINLINE Iterator Erase(Iterator Pos) noexcept
	{
		return Erase(ConstIterator(Pos.Ptr));
	}

	FORCEINLINE Iterator Erase(ConstIterator Pos) noexcept
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
		T* dataBegin = DataPtr + Index;
		T* DataEnd = DataPtr + DataSize;
		InternalMemmoveBackwards(dataBegin + 1, DataEnd, dataBegin);
		InternalDestruct(DataEnd - 1);

		DataSize--;
		return Iterator(dataBegin);
	}

	FORCEINLINE Iterator Erase(Iterator InBegin, Iterator InEnd) noexcept
	{
		return Erase(ConstIterator(InBegin.Ptr), ConstIterator(InEnd.Ptr));
	}

	FORCEINLINE Iterator Erase(ConstIterator InBegin, ConstIterator InEnd) noexcept
	{
		VALIDATE(InBegin < InEnd);
		VALIDATE(InternalIsRangeOwner(InBegin, InEnd));

		T* dataBegin = DataPtr + InternalIndex(InBegin);
		T* DataEnd = DataPtr + InternalIndex(InEnd);

		const SizeType elementCount = InternalDistance(dataBegin, DataEnd);
		if (InEnd >= cend())
		{
			InternalDestructRange(dataBegin, DataEnd);
		}
		else
		{
			T* realEnd = DataPtr + DataSize;
			InternalMemmoveBackwards(DataEnd, realEnd, dataBegin);
			InternalDestructRange(realEnd - elementCount, realEnd);
		}

		DataSize -= elementCount;
		return Iterator(dataBegin);
	}

	FORCEINLINE void Swap(TArray& Other) noexcept
	{
		if (this != std::addressof(Other))
		{
			T* tempPtr = DataPtr;
			SizeType tempSize = DataSize;
			SizeType tempCapacity = DataCapacity;

			DataPtr = Other.DataPtr;
			DataSize = Other.DataSize;
			DataCapacity = Other.DataCapacity;

			Other.DataPtr = tempPtr;
			Other.DataSize = tempSize;
			Other.DataCapacity = tempCapacity;
		}
	}

	FORCEINLINE void ShrinkToFit() noexcept
	{
		if (DataCapacity > DataSize)
		{
			InternalRealloc(DataSize);
		}
	}

	FORCEINLINE bool IsEmpty() const noexcept
	{
		return (DataSize == 0);
	}

	FORCEINLINE T& Front() noexcept
	{
		VALIDATE(DataSize > 0);
		return DataPtr[0];
	}

	FORCEINLINE Iterator Begin() noexcept
	{
		return Iterator(DataPtr);
	}

	FORCEINLINE Iterator End() noexcept
	{
		return Iterator(DataPtr + DataSize);
	}

	FORCEINLINE ConstIterator Begin() const noexcept
	{
		return Iterator(DataPtr);
	}

	FORCEINLINE ConstIterator End() const noexcept
	{
		return Iterator(DataPtr + DataSize);
	}

	FORCEINLINE const T& Front() const noexcept
	{
		VALIDATE(DataSize > 0);
		return DataPtr[0];
	}

	FORCEINLINE T& Back() noexcept
	{
		VALIDATE(DataSize > 0);
		return DataPtr[DataSize - 1];
	}

	FORCEINLINE const T& Back() const noexcept
	{
		VALIDATE(DataSize > 0);
		return DataPtr[DataSize - 1];
	}

	FORCEINLINE T* Data() noexcept
	{
		return DataPtr;
	}

	FORCEINLINE const T* Data() const noexcept
	{
		return DataPtr;
	}

	FORCEINLINE SizeType Size() const noexcept
	{
		return DataSize;
	}

	FORCEINLINE SizeType Capacity() const noexcept
	{
		return DataCapacity;
	}

	FORCEINLINE T& At(SizeType Index) noexcept
	{
		VALIDATE(Index < DataSize);
		return DataPtr[Index];
	}

	FORCEINLINE const T& At(SizeType Index) const noexcept
	{
		VALIDATE(Index < DataSize);
		return DataPtr[Index];
	}

	FORCEINLINE TArray& operator=(const TArray& Other) noexcept
	{
		if (this != std::addressof(Other))
		{
			Clear();
			InternalConstruct(Other.Begin(), Other.End());
		}

		return *this;
	}

	FORCEINLINE TArray& operator=(TArray&& Other) noexcept
	{
		if (this != std::addressof(Other))
		{
			Clear();
			InternalMove(::Move(Other));
		}

		return *this;
	}

	FORCEINLINE TArray& operator=(std::initializer_list<T> IList) noexcept
	{
		Assign(IList);
		return *this;
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
	* STL iterator functions, Only here so that you can use Range for-loops
	*/
public:
	FORCEINLINE Iterator begin() noexcept
	{
		return Iterator(DataPtr);
	}

	FORCEINLINE Iterator end() noexcept
	{
		return Iterator(DataPtr + DataSize);
	}

	FORCEINLINE ConstIterator begin() const noexcept
	{
		return ConstIterator(DataPtr);
	}

	FORCEINLINE ConstIterator end() const noexcept
	{
		return ConstIterator(DataPtr + DataSize);
	}

	FORCEINLINE ConstIterator cbegin() const noexcept
	{
		return ConstIterator(DataPtr);
	}

	FORCEINLINE ConstIterator cend() const noexcept
	{
		return ConstIterator(DataPtr + DataSize);
	}

	FORCEINLINE ReverseIterator rbegin() noexcept
	{
		return ReverseIterator(DataPtr);
	}

	FORCEINLINE ReverseIterator rend() noexcept
	{
		return ReverseIterator(DataPtr + DataSize);
	}

	FORCEINLINE ReverseConstIterator rbegin() const noexcept
	{
		return ReverseConstIterator(DataPtr);
	}

	FORCEINLINE ReverseConstIterator rend() const noexcept
	{
		return ReverseConstIterator(DataPtr + DataSize);
	}

	FORCEINLINE ReverseConstIterator crbegin() const noexcept
	{
		return ReverseConstIterator(DataPtr);
	}

	FORCEINLINE ReverseConstIterator crend() const noexcept
	{
		return ReverseConstIterator(DataPtr + DataSize);
	}

private:
	// Check is the iterator belongs to this TArray
	FORCEINLINE bool InternalIsRangeOwner(ConstIterator InBegin, ConstIterator InEnd) const
	{
		return (InBegin < InEnd) && (InBegin >= begin()) && (InEnd <= end());
	}

	FORCEINLINE bool InternalIsIteratorOwner(ConstIterator It) const
	{
		return (It >= begin()) && (It <= end());
	}

	// Helpers
	template<typename TInputIt>
	FORCEINLINE const T* InternalUnwrapConst(TInputIt It)
	{
		if constexpr (std::is_pointer<TInputIt>())
		{
			return It;
		}
		else
		{
			return It.Ptr;
		}
	}

	template<typename TInputIt>
	FORCEINLINE SizeType InternalDistance(TInputIt InBegin, TInputIt InEnd)
	{
		constexpr bool IsPointer = std::is_pointer<TInputIt>();
		constexpr bool IsCustomIterator = std::is_same<TInputIt, Iterator>() || std::is_same<TInputIt, ConstIterator>();

		// Handle outside pointers
		if constexpr (IsPointer || IsCustomIterator)
		{
			return static_cast<SizeType>(InternalUnwrapConst(InEnd) - InternalUnwrapConst(InBegin));
		}
		else
		{
			return static_cast<SizeType>(std::Distance(InBegin, InEnd));
		}
	}

	template<typename TInputIt>
	FORCEINLINE SizeType InternalIndex(TInputIt Pos)
	{
		return static_cast<SizeType>(InternalUnwrapConst(Pos) - InternalUnwrapConst(begin()));
	}

	FORCEINLINE SizeType InternalGetResizeFactor() const
	{
		return InternalGetResizeFactor(DataSize);
	}

	FORCEINLINE SizeType InternalGetResizeFactor(SizeType BaseSize) const
	{
		return BaseSize + (DataCapacity / 2) + 1;
	}

	FORCEINLINE T* InternalAllocateElements(SizeType InCapacity)
	{
		constexpr SizeType ElementByteSize = sizeof(T);
		const SizeType SizeInBytes = ElementByteSize * InCapacity;
		return reinterpret_cast<T*>(malloc(static_cast<size_t>(SizeInBytes)));
	}

	FORCEINLINE void InternalReleaseData()
	{
		if (DataPtr)
		{
			free(DataPtr);
			DataPtr = nullptr;
		}
	}

	FORCEINLINE void InternalAllocData(SizeType InCapacity)
	{
		if (InCapacity > DataCapacity)
		{
			InternalReleaseData();
			DataPtr = InternalAllocateElements(InCapacity);
			DataCapacity = InCapacity;
		}
	}

	FORCEINLINE void InternalRealloc(SizeType InCapacity)
	{
		T* TempData = InternalAllocateElements(InCapacity);
		InternalMoveEmplace(DataPtr, DataPtr + DataSize, TempData);
		InternalDestructRange(DataPtr, DataPtr + DataSize);

		InternalReleaseData();
		DataPtr = TempData;

		DataCapacity = InCapacity;
	}

	FORCEINLINE void InternalEmplaceRealloc(SizeType InCapacity, T* EmplacePos, SizeType Count)
	{
		VALIDATE(InCapacity >= DataSize + Count);

		const SizeType Index = InternalIndex(EmplacePos);
		T* TempData = InternalAllocateElements(InCapacity);
		InternalMoveEmplace(DataPtr, EmplacePos, TempData);
		if (EmplacePos != DataPtr + DataSize)
		{
			InternalMoveEmplace(EmplacePos, DataPtr + DataSize, TempData + Index + Count);
		}

		InternalDestructRange(DataPtr, DataPtr + DataSize);

		InternalReleaseData();
		DataPtr = TempData;

		DataCapacity = InCapacity;
	}

	// Construct
	FORCEINLINE void InternalConstruct(SizeType InSize)
	{
		if (InSize > 0)
		{
			InternalAllocData(InSize);
			DataSize = InSize;
			InternalDefaultConstructRange(DataPtr, DataPtr + InSize);
		}
	}

	FORCEINLINE void InternalConstruct(SizeType InSize, const T& Value)
	{
		if (InSize > 0)
		{
			InternalAllocData(InSize);
			InternalCopyEmplace(InSize, Value, DataPtr);
			DataSize = InSize;
		}
	}

	template<typename TInputIt>
	FORCEINLINE void InternalConstruct(TInputIt InBegin, TInputIt InEnd)
	{
		const SizeType Distance = InternalDistance(InBegin, InEnd);
		if (Distance > 0)
		{
			InternalAllocData(Distance);
			InternalCopyEmplace(InBegin, InEnd, DataPtr);
			DataSize = Distance;
		}
	}

	FORCEINLINE void InternalMove(TArray&& Other)
	{
		InternalReleaseData();

		DataPtr = Other.DataPtr;
		DataSize = Other.DataSize;
		DataCapacity = Other.DataCapacity;

		Other.DataPtr = nullptr;
		Other.DataSize = 0;
		Other.DataCapacity = 0;
	}

	// Emplace
	template<typename TInputIt>
	FORCEINLINE void InternalCopyEmplace(TInputIt InBegin, TInputIt InEnd, T* Dest)
	{
		// This function assumes that there is no overlap
		constexpr bool IsTrivial = std::is_trivially_copy_constructible<T>();
		constexpr bool IsPointer = std::is_pointer<TInputIt>();
		constexpr bool IsCustomIterator = std::is_same<TInputIt, Iterator>() || std::is_same<TInputIt, ConstIterator>();

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
				new(reinterpret_cast<VoidPtr>(Dest)) T(*InBegin);
				InBegin++;
				Dest++;
			}
		}
	}

	FORCEINLINE void InternalCopyEmplace(SizeType Size, const T& Value, T* Dest)
	{
		T* ItEnd = Dest + Size;
		while (Dest != ItEnd)
		{
			new(reinterpret_cast<VoidPtr>(Dest)) T(Value);
			Dest++;
		}
	}

	FORCEINLINE void InternalMoveEmplace(T* InBegin, T* InEnd, T* Dest)
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
				new(reinterpret_cast<VoidPtr>(Dest)) T(::Move(*InBegin));
				InBegin++;
				Dest++;
			}
		}
	}

	FORCEINLINE void InternalMemmoveBackwards(T* InBegin, T* InEnd, T* Dest)
	{
		VALIDATE(InBegin <= InEnd);
		if (InBegin == InEnd)
		{
			return;
		}

		VALIDATE(InEnd <= DataPtr + DataCapacity);

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

	FORCEINLINE void InternalMemmoveForward(T* InBegin, T* InEnd, T* Dest)
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

	FORCEINLINE void InternalDestruct(const T* Pos)
	{
		// Calls the destructor (If It needs to be called)
		if constexpr (std::is_trivially_destructible<T>() == false)
		{
			(*Pos).~T();
		}
	}

	FORCEINLINE void InternalDestructRange(const T* InBegin, const T* InEnd)
	{
		VALIDATE(InBegin <= InEnd);

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

	FORCEINLINE void InternalDefaultConstructRange(T* InBegin, T* InEnd)
	{
		VALIDATE(InBegin <= InEnd);

		// Calls the default constructor for every object in the range (If It can be called)
		if constexpr (std::is_default_constructible<T>())
		{
			while (InBegin != InEnd)
			{
				new(reinterpret_cast<VoidPtr>(InBegin)) T();
				InBegin++;
			}
		}
	}

private:
	T* DataPtr;
	SizeType DataSize;
	SizeType DataCapacity;
};