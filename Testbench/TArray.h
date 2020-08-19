#pragma once
#include "TypeUtilities.h"

/*
* Dynamic Array similar to std::vector
*/
template<typename T>
class TArray
{
public:
	typedef T		TType;
	typedef Uint32	TSize;
	typedef TType*	TPointer;

	/*
	* Constant Iterator
	*/
	class ConstIterator
	{
		friend class TArray;

	public:
		~ConstIterator() = default;

		FORCEINLINE explicit ConstIterator(TPointer InPtr = nullptr)
			: Ptr(InPtr)
		{
		}

		FORCEINLINE const TPointer operator->() const
		{
			return Ptr;
		}

		FORCEINLINE const TType& operator*() const
		{
			return *Ptr;
		}

		FORCEINLINE ConstIterator operator++()
		{
			Ptr++;
			return *this;
		}

		FORCEINLINE ConstIterator operator++(Int32)
		{
			ConstIterator Temp = *this;
			Ptr++;
			return Temp;
		}

		FORCEINLINE ConstIterator operator--()
		{
			Ptr--;
			return *this;
		}

		FORCEINLINE ConstIterator operator--(Int32)
		{
			ConstIterator Temp = *this;
			Ptr--;
			return Temp;
		}

		FORCEINLINE ConstIterator operator+(Int32 Offset) const
		{
			ConstIterator Temp = *this;
			return Temp += Offset;
		}

		FORCEINLINE ConstIterator operator-(Int32 Offset) const
		{
			ConstIterator Temp = *this;
			return Temp -= Offset;
		}

		FORCEINLINE ConstIterator& operator+=(Int32 Offset)
		{
			Ptr += Offset;
			return *this;
		}

		FORCEINLINE ConstIterator& operator-=(Int32 Offset)
		{
			Ptr -= Offset;
			return *this;
		}

		FORCEINLINE bool operator==(const ConstIterator& Other) const
		{
			return (Ptr == Other.Ptr);
		}

		FORCEINLINE bool operator!=(const ConstIterator& Other) const
		{
			return (Ptr != Other.Ptr);
		}

		FORCEINLINE bool operator<(const ConstIterator& Other) const
		{
			return Ptr < Other.Ptr;
		}

		FORCEINLINE bool operator<=(const ConstIterator& Other) const
		{
			return Ptr <= Other.Ptr;
		}

		FORCEINLINE bool operator>(const ConstIterator& Other) const
		{
			return Ptr > Other.Ptr;
		}

		FORCEINLINE bool operator>=(const ConstIterator& Other) const
		{
			return Ptr >= Other.Ptr;
		}

	protected:
		TPointer Ptr;
	};

	/*
	* Standard Iterator
	*/
	class Iterator : public ConstIterator
	{
		friend class TArray;

	public:
		~Iterator() = default;

		FORCEINLINE explicit Iterator(TPointer InPtr = nullptr)
			: ConstIterator(InPtr)
		{
		}

		FORCEINLINE TPointer operator->() const
		{
			return const_cast<TPointer>(ConstIterator::operator->());
		}

		FORCEINLINE TType& operator*() const
		{
			return const_cast<TType&>(ConstIterator::operator*());
		}

		FORCEINLINE Iterator operator++()
		{
			ConstIterator::operator++();
			return *this;
		}

		FORCEINLINE Iterator operator++(Int32)
		{
			Iterator Temp = *this;
			ConstIterator::operator++();
			return Temp;
		}

		FORCEINLINE Iterator operator--()
		{
			ConstIterator::operator--();
			return *this;
		}

		FORCEINLINE Iterator operator--(Int32)
		{
			Iterator Temp = *this;
			ConstIterator::operator--();
			return Temp;
		}

		FORCEINLINE Iterator operator+(Int32 Offset) const
		{
			Iterator Temp = *this;
			return Temp += Offset;
		}

		FORCEINLINE Iterator operator-(Int32 Offset) const
		{
			Iterator Temp = *this;
			return Temp -= Offset;
		}

		FORCEINLINE Iterator& operator+=(Int32 Offset)
		{
			ConstIterator::operator+=(Offset);
			return *this;
		}

		FORCEINLINE Iterator& operator-=(Int32 Offset)
		{
			ConstIterator::operator-=(Offset);
			return *this;
		}
	};

	/*
	* Reverse Constant Iterator 
	* Stores for example End(), but will reference End() - 1
	*/
	class ReverseConstIterator
	{
		friend class TArray;

	public:
		~ReverseConstIterator() = default;

		FORCEINLINE explicit ReverseConstIterator(TPointer InPtr = nullptr)
			: Ptr(InPtr)
		{
		}

		FORCEINLINE const TPointer operator->() const
		{
			return (Ptr - 1);
		}

		FORCEINLINE const TType& operator*() const
		{
			return *(Ptr - 1);
		}

		FORCEINLINE ReverseConstIterator operator++()
		{
			Ptr--;
			return *this;
		}

		FORCEINLINE ReverseConstIterator operator++(Int32)
		{
			ConstIterator Temp = *this;
			Ptr--;
			return Temp;
		}

		FORCEINLINE ReverseConstIterator operator--()
		{
			Ptr++;
			return *this;
		}

		FORCEINLINE ReverseConstIterator operator--(Int32)
		{
			ReverseConstIterator Temp = *this;
			Ptr++;
			return Temp;
		}

		FORCEINLINE ReverseConstIterator operator+(Int32 Offset) const
		{
			ReverseConstIterator Temp = *this;
			return Temp += Offset;
		}

		FORCEINLINE ReverseConstIterator operator-(Int32 Offset) const
		{
			ReverseConstIterator Temp = *this;
			return Temp -= Offset;
		}

		FORCEINLINE ReverseConstIterator& operator+=(Int32 Offset)
		{
			Ptr -= Offset;
			return *this;
		}

		FORCEINLINE ReverseConstIterator& operator-=(Int32 Offset)
		{
			Ptr += Offset;
			return *this;
		}

		FORCEINLINE bool operator==(const ReverseConstIterator& Other) const
		{
			return (Ptr == Other.Ptr);
		}

		FORCEINLINE bool operator!=(const ReverseConstIterator& Other) const
		{
			return (Ptr != Other.Ptr);
		}

	protected:
		TPointer Ptr;
	};

	/*
	* Standard Reverse Iterator
	*/
	class ReverseIterator : public ReverseConstIterator
	{
		friend class TArray;

	public:
		~ReverseIterator() = default;

		FORCEINLINE explicit ReverseIterator(TPointer InPtr = nullptr)
			: ReverseConstIterator(InPtr)
		{
		}

		FORCEINLINE TPointer operator->() const
		{
			return const_cast<TPointer>(ReverseConstIterator::operator->());
		}

		FORCEINLINE TType& operator*() const
		{
			return const_cast<TType&>(ReverseConstIterator::operator*());
		}

		FORCEINLINE ReverseIterator operator++()
		{
			ReverseConstIterator::operator++();
			return *this;
		}

		FORCEINLINE ReverseIterator operator++(Int32)
		{
			ReverseIterator Temp = *this;
			ReverseConstIterator::operator++();
			return Temp;
		}

		FORCEINLINE ReverseIterator operator--()
		{
			ReverseConstIterator::operator--();
			return *this;
		}

		FORCEINLINE ReverseIterator operator--(Int32)
		{
			ReverseIterator Temp = *this;
			ReverseConstIterator::operator--();
			return Temp;
		}

		FORCEINLINE ReverseIterator operator+(Int32 Offset) const
		{
			ReverseIterator Temp = *this;
			return Temp += Offset;
		}

		FORCEINLINE ReverseIterator operator-(Int32 Offset) const
		{
			ReverseIterator Temp = *this;
			return Temp -= Offset;
		}

		FORCEINLINE ReverseIterator& operator+=(Int32 Offset)
		{
			ReverseConstIterator::operator+=(Offset);
			return *this;
		}

		FORCEINLINE ReverseIterator& operator-=(Int32 Offset)
		{
			ReverseConstIterator::operator-=(Offset);
			return *this;
		}
	};

	/*
	* TArray API
	*/
public:
	FORCEINLINE TArray() noexcept
		: Data(nullptr)
		, Size(0)
		, Capacity(0)
	{
	}

	FORCEINLINE explicit TArray(TSize InSize, const TType& Value = TType()) noexcept
		: Data(nullptr)
		, Size(0)
		, Capacity(0)
	{
		InternalConstruct(InSize, Value);
	}

	FORCEINLINE explicit TArray(Iterator InBegin, Iterator InEnd) noexcept
		: Data(nullptr)
		, Size(0)
		, Capacity(0)
	{
		InternalConstruct(InBegin, InEnd);
	}

	FORCEINLINE TArray(std::initializer_list<TType> IList) noexcept
		: Data(nullptr)
		, Size(0)
		, Capacity(0)
	{
		InternalConstruct(IList);
	}

	FORCEINLINE TArray(const TArray& Other) noexcept
		: Data(nullptr)
		, Size(0)
		, Capacity(0)
	{
		InternalConstruct(Other.Begin(), Other.End());
	}

	FORCEINLINE TArray(TArray&& Other) noexcept
		: Data(nullptr)
		, Size(0)
		, Capacity(0)
	{
		InternalMove(Move(Other));
	}

	FORCEINLINE ~TArray()
	{
		Clear();

		InternalReleaseData();

		Capacity = 0;
		Data = nullptr;
	}

	FORCEINLINE void Clear() noexcept
	{
		InternalDestructRange(InternalBegin(), InternalRealEnd());
		Size = 0;
	}

	FORCEINLINE void Assign(TSize InSize, const TType& Value = TType()) noexcept
	{
		Clear();
		InternalConstruct(InSize, Value);
	}

	FORCEINLINE void Assign(Iterator InBegin, Iterator InEnd) noexcept
	{
		Clear();
		InternalConstruct(InBegin, InEnd);
		InternalFillConstruct();
	}

	FORCEINLINE void Assign(std::initializer_list<TType> IList) noexcept
	{
		Clear();
		InternalConstruct(IList);
		InternalFillConstruct();
	}

	FORCEINLINE void Resize(TSize InSize, const TType& Value = TType()) noexcept
	{
		if (InSize == Size)
		{
			return;
		}

		if (InSize == 0)
		{
			Clear();
		}
		else if (InSize > Size)
		{
			if (InSize >= Capacity)
			{
				const TSize NewCapacity = InSize + (Capacity / 2) + 1;
				InternalEmplaceRealloc(NewCapacity, InternalEnd(), InSize - Size);
			}

			InternalCopyEmplace(InSize - Size, Value, InternalEnd());
		}
		else if (InSize < Size)
		{
			InternalDestructRange(InternalBegin() + InSize, InternalEnd());
		}

		Size = InSize;
	}

	FORCEINLINE void Reserve(TSize InCapacity) noexcept
	{
		if (InCapacity == Capacity)
		{
			return;
		}

		if (InCapacity < Size)
		{
			Size = InCapacity;
		}

		TPointer TempData = InternalAllocateElements(InCapacity);
		InternalMoveEmplace(InternalBegin(), InternalEnd(), TempData);

		InternalConstructRange(TempData + Size, TempData + InCapacity);
		InternalDestructRange(InternalBegin(), InternalRealEnd());
		InternalReleaseData();

		Data		= TempData;
		Capacity	= InCapacity;
	}

	template<typename... TArgs>
	FORCEINLINE TType& EmplaceBack(TArgs&&... Args) noexcept
	{
		TPointer ItEnd = InternalEnd();
		if (Size >= Capacity)
		{
			const TSize NewCapacity = InternalGetResizeFactor();
			InternalEmplaceRealloc(NewCapacity, ItEnd, 1);
			ItEnd = InternalEnd();
		}
		else
		{
			InternalDestruct(ItEnd);
		}

		new(ItEnd) TType(Forward<TArgs>(Args)...);
		Size++;
		return (*ItEnd);
	}

	FORCEINLINE TType& PushBack(const TType& Element) noexcept
	{
		return EmplaceBack(Element);
	}

	FORCEINLINE TType& PushBack(TType&& Element) noexcept
	{
		return EmplaceBack(Move(Element));
	}

	template<typename... TArgs>
	FORCEINLINE Iterator Emplace(ConstIterator Pos, TArgs&&... Args) noexcept
	{
		if (Pos == End())
		{
			EmplaceBack(Forward<TArgs>(Args)...);
			return (End() - 1);
		}

		const TSize Index = InternalIndex(Pos);
		TPointer ItBegin = InternalBegin() + Index;
		if (Size >= Capacity)
		{
			const TSize NewCapacity = InternalGetResizeFactor();
			InternalEmplaceRealloc(NewCapacity, ItBegin, 1);
			ItBegin = InternalBegin() + Index;
		}
		else
		{
			InternalMemmoveForward(ItBegin, InternalEnd(), InternalEnd());
			InternalDestruct(ItBegin);
		}

		new (ItBegin) TType(Forward<TArgs>(Args)...);
		Size++;
		return Iterator(ItBegin);
	}

	FORCEINLINE Iterator Insert(ConstIterator Pos, const TType& Value) noexcept
	{
		return Emplace(Pos, Value);
	}

	FORCEINLINE Iterator Insert(ConstIterator Pos, TType&& Value) noexcept
	{
		return Emplace(Pos, Move(Value));
	}

	FORCEINLINE Iterator Insert(ConstIterator Pos, std::initializer_list<TType> IList) noexcept
	{
		const TSize ListSize	= static_cast<TSize>(IList.size());
		const TSize NewSize		= Size + ListSize;
		const TSize Index		= InternalIndex(Pos);

		TPointer ItBegin = InternalBegin() + Index;
		if (NewSize >= Capacity)
		{
			const TSize NewCapacity = NewSize + (Capacity / 2) + 1;
			InternalEmplaceRealloc(NewCapacity, ItBegin, ListSize);
			ItBegin = InternalBegin() + Index;
		}
		else
		{
			InternalMemmoveForward(ItBegin, InternalEnd(), InternalEnd() + (ListSize - 1));
			InternalDestructRange(ItBegin, ItBegin + ListSize);
		}

		InternalMoveEmplace(IList, ItBegin);
		Size = NewSize;
		return Iterator(ItBegin);
	}

	FORCEINLINE void PopBack() noexcept
	{
		if (Size > 0)
		{
			InternalDestruct(InternalBegin() + (--Size));
		}
	}

	FORCEINLINE Iterator Erase(Iterator Pos) noexcept
	{
		return Erase(static_cast<ConstIterator>(Pos));
	}

	FORCEINLINE Iterator Erase(ConstIterator Pos) noexcept
	{
		VALIDATE(InternalIsIteratorOwner(Pos));

		if (Pos == End())
		{
			PopBack();
			return End();
		}
		else
		{
			return Erase(Pos, Pos + 1);
		}
	}

	FORCEINLINE Iterator Erase(Iterator InBegin, Iterator InEnd) noexcept
	{
		return Erase(static_cast<ConstIterator>(InBegin), static_cast<ConstIterator>(InEnd));
	}

	FORCEINLINE Iterator Erase(ConstIterator InBegin, ConstIterator InEnd) noexcept
	{
		VALIDATE(InBegin < InEnd);
		VALIDATE(InternalIsRangeOwner(InBegin, InEnd));

		InternalDestructRange(InBegin.Ptr, InEnd.Ptr);

		const TSize Index	= static_cast<TSize>(InBegin.Ptr - InternalBegin());
		const TSize Offset	= static_cast<TSize>(InEnd.Ptr - InBegin.Ptr);

		TPointer ItBegin	= InternalBegin() + Index;
		TPointer ItOffset	= ItBegin + Offset;
		if (ItOffset < InternalEnd())
		{
			InternalMemmoveBackwards(ItOffset, InternalEnd(), ItBegin);
		}

		Size -= Offset;
		return Iterator(ItBegin);
	}

	FORCEINLINE void Swap(TArray& Other) noexcept
	{
		if (this != std::addressof(Other))
		{
			TPointer	TempPtr = Data;
			TSize		TempSize = Size;
			TSize		TempCapacity = Capacity;

			Data = Other.Data;
			Size = Other.Size;
			Capacity = Other.Capacity;

			Other.Data = TempPtr;
			Other.Size = TempSize;
			Other.Capacity = TempCapacity;
		}
	}

	FORCEINLINE void ShrinkToFit() noexcept
	{
		if (Capacity > Size)
		{
			InternalEmplaceRealloc(Size, InternalEnd(), 0);
		}
	}

	FORCEINLINE bool IsEmpty() const noexcept
	{
		return (Size == 0);
	}

	FORCEINLINE Iterator Begin() noexcept
	{
		return Iterator(Data);
	}

	FORCEINLINE Iterator End() noexcept
	{
		return Iterator(Data + Size);
	}

	FORCEINLINE ConstIterator Begin() const noexcept
	{
		return ConstIterator(Data);
	}

	FORCEINLINE ConstIterator End() const noexcept
	{
		return ConstIterator(Data + Size);
	}

	FORCEINLINE ConstIterator ConstBegin() const noexcept
	{
		return ConstIterator(Data);
	}

	FORCEINLINE ConstIterator ConstEnd() const noexcept
	{
		return ConstIterator(Data + Size);
	}

	FORCEINLINE ReverseIterator ReverseBegin() noexcept
	{
		return ReverseIterator(Data + Size);
	}

	FORCEINLINE ReverseIterator ReverseEnd() noexcept
	{
		return ReverseIterator(Data);
	}

	FORCEINLINE ReverseConstIterator ReverseBegin() const noexcept
	{
		return ReverseConstIterator(Data + Size);
	}

	FORCEINLINE ReverseConstIterator ReverseEnd() const noexcept
	{
		return ReverseConstIterator(Data);
	}

	FORCEINLINE ReverseConstIterator ConstReverseBegin() const noexcept
	{
		return ReverseConstIterator(Data + Size);
	}

	FORCEINLINE ReverseConstIterator ConstReverseEnd() const noexcept
	{
		return ReverseConstIterator(Data);
	}

	FORCEINLINE TType& GetFront() noexcept
	{
		VALIDATE(Size > 0);
		return Data[0];
	}

	FORCEINLINE const TType& GetFront() const noexcept
	{
		VALIDATE(Size > 0);
		return Data[0];
	}

	FORCEINLINE TType& GetBack() noexcept
	{
		VALIDATE(Size > 0);
		return Data[Size - 1];
	}

	FORCEINLINE const TType& GetBack() const noexcept
	{
		VALIDATE(Size > 0);
		return Data[Size - 1];
	}

	FORCEINLINE TPointer GetData() noexcept
	{
		return Data;
	}

	FORCEINLINE const TPointer GetData() const noexcept
	{
		return Data;
	}

	FORCEINLINE TSize GetSize() const noexcept
	{
		return Size;
	}

	FORCEINLINE TSize GetCapacity() const noexcept
	{
		return Capacity;
	}

	FORCEINLINE TType& GetElementAt(TSize Index) noexcept
	{
		VALIDATE(Index < Size);
		return Data[Index];
	}

	FORCEINLINE const TType& GetElementAt(TSize Index) const noexcept
	{
		VALIDATE(Index < Size);
		return Data[Index];
	}

	FORCEINLINE TArray& operator=(const TArray& Other) noexcept
	{
		if (this != std::addressof(Other))
		{
			Clear();
			InternalConstruct(Other.Begin(), Other.End());
			InternalFillConstruct();
		}

		return *this;
	}

	FORCEINLINE TArray& operator=(TArray&& Other) noexcept
	{
		if (this != std::addressof(Other))
		{
			Clear();
			InternalReleaseData();
			InternalMove(Move(Other));
		}

		return *this;
	}

	FORCEINLINE TArray& operator=(std::initializer_list<TType> IList) noexcept
	{
		Assign(IList);
		return *this;
	}

	FORCEINLINE TType& operator[](TSize Index) noexcept
	{
		return GetElementAt(Index);
	}

	FORCEINLINE const TType& operator[](TSize Index) const noexcept
	{
		return GetElementAt(Index);
	}

	/*
	* STL iterator functions, Only here so that you can use Range for-loops
	*/
public:
	FORCEINLINE Iterator begin() noexcept
	{
		return Iterator(Data);
	}

	FORCEINLINE Iterator end() noexcept
	{
		return Iterator(Data + Size);
	}

	FORCEINLINE ConstIterator begin() const noexcept
	{
		return ConstIterator(Data);
	}

	FORCEINLINE ConstIterator end() const noexcept
	{
		return ConstIterator(Data + Size);
	}

	FORCEINLINE ConstIterator cbegin() const noexcept
	{
		return ConstIterator(Data);
	}

	FORCEINLINE ConstIterator cend() const noexcept
	{
		return ConstIterator(Data + Size);
	}

	FORCEINLINE ReverseIterator rbegin() noexcept
	{
		return ReverseIterator(Data);
	}

	FORCEINLINE ReverseIterator rend() noexcept
	{
		return ReverseIterator(Data + Size);
	}

	FORCEINLINE ReverseConstIterator rbegin() const noexcept
	{
		return ReverseConstIterator(Data);
	}

	FORCEINLINE ReverseConstIterator rend() const noexcept
	{
		return ReverseConstIterator(Data + Size);
	}

	FORCEINLINE ReverseConstIterator crbegin() const noexcept
	{
		return ReverseConstIterator(Data);
	}

	FORCEINLINE ReverseConstIterator crend() const noexcept
	{
		return ReverseConstIterator(Data + Size);
	}

private:
	// Internally use pointers for iterating
	FORCEINLINE TPointer InternalBegin() const noexcept
	{
		return Data;
	}

	FORCEINLINE TPointer InternalEnd() const noexcept
	{
		return Data + Size;
	}
	
	FORCEINLINE TPointer InternalRealEnd() const noexcept
	{
		return Data + Capacity;
	}

	// Check is the iterator belongs to this TArray
	FORCEINLINE bool InternalIsRangeOwner(ConstIterator InBegin, ConstIterator InEnd)
	{
		return (InBegin < InEnd) && (InBegin >= Begin()) && (InEnd <= End());
	}

	FORCEINLINE bool InternalIsIteratorOwner(ConstIterator It)
	{
		return (It >= Begin()) && (It <= End());
	}

	// Helpers
	FORCEINLINE TSize InternalDistance(ConstIterator InBegin, ConstIterator InEnd)
	{
		return static_cast<TSize>(InEnd.Ptr - InBegin.Ptr);
	}

	FORCEINLINE TSize InternalIndex(ConstIterator Pos)
	{
		return static_cast<TSize>(Pos.Ptr - Data);
	}

	FORCEINLINE TSize InternalIndex(TPointer Pos)
	{
		return static_cast<TSize>(Pos - Data);
	}

	FORCEINLINE TSize InternalGetResizeFactor() const
	{
		return Size + (Capacity) + 1;
	}

	FORCEINLINE TPointer InternalAllocateElements(TSize InCapacity)
	{
		constexpr TSize ElementByteSize = sizeof(TType);
		return reinterpret_cast<TPointer>(malloc(static_cast<size_t>(ElementByteSize) * InCapacity));
	}

	FORCEINLINE void InternalReleaseData()
	{
		if (Data)
		{
			free(Data);
		}
	}

	FORCEINLINE void InternalAllocData(TSize InCapacity)
	{
		if (InCapacity > Capacity)
		{
			InternalReleaseData();

			Data = InternalAllocateElements(InCapacity);
			Capacity = InCapacity;
		}
	}

	FORCEINLINE void InternalEmplaceRealloc(TSize InCapacity, TPointer EmplacePos, TSize Count)
	{
		VALIDATE(InCapacity >= Size + Count);

		const TSize Index = InternalIndex(EmplacePos);
		TPointer TempData = InternalAllocateElements(InCapacity);
		InternalMoveEmplace(Data, EmplacePos, TempData);
		
		if (EmplacePos != InternalEnd())
		{
			InternalMoveEmplace(EmplacePos, InternalRealEnd(), TempData + Index + Count);
		}

		if (InCapacity > Size + Count)
		{
			InternalConstructRange(TempData + Size + Count, TempData + InCapacity);
		}

		InternalDestructRange(InternalBegin(), InternalRealEnd());
		InternalReleaseData();

		Data		= TempData;
		Capacity	= InCapacity;
	}

	// Construct
	FORCEINLINE void InternalConstruct(TSize InSize, const TType& Value)
	{
		if (InSize > 0)
		{
			InternalAllocData(InSize);
			InternalCopyEmplace(InSize, Value, Data);
			Size = InSize;
		}
	}

	FORCEINLINE void InternalConstruct(ConstIterator InBegin, ConstIterator InEnd)
	{
		VALIDATE(InternalIsRangeOwner(InBegin, InEnd) == false);

		const TSize Distance = InternalDistance(InBegin, InEnd);
		if (Distance > 0)
		{
			InternalAllocData(Distance);
			InternalCopyEmplace(InBegin.Ptr, InEnd.Ptr, Data);
			Size = Distance;
		}
	}

	FORCEINLINE void InternalConstruct(std::initializer_list<TType> IList)
	{
		const TSize ListSize = static_cast<TSize>(IList.size());
		if (ListSize > 0)
		{
			InternalAllocData(ListSize);
			InternalMoveEmplace(IList, Data);
			Size = ListSize;
		}
	}

	FORCEINLINE void InternalFillConstruct()
	{
		if (Size < Capacity)
		{
			InternalConstructRange(InternalEnd(), InternalRealEnd());
		}
	}

	FORCEINLINE void InternalMove(TArray&& Other)
	{
		Data = Other.Data;
		Size = Other.Size;
		Capacity = Other.Capacity;

		Other.Data = nullptr;
		Other.Size = 0;
		Other.Capacity = 0;
	}

	// Emplace
	FORCEINLINE void InternalCopyEmplace(TPointer InBegin, TPointer InEnd, TPointer Dest)
	{
		// This function assumes that there is no overlap
		if constexpr (std::is_trivially_move_constructible<TType>())
		{
			const TSize Count = static_cast<TSize>(InEnd - InBegin);
			const TSize CpySize = Count * sizeof(TType);
			memcpy(Dest, InBegin, CpySize);
		}
		else
		{
			while (InBegin != InEnd)
			{
				new(Dest) TType(*InBegin);
				InBegin++;
				Dest++;
			}
		}
	}

	FORCEINLINE void InternalCopyEmplace(TSize InSize, const TType& Value, TPointer Dest)
	{
		TPointer ItEnd = Dest + InSize;
		while (Dest != ItEnd)
		{
			new(Dest) TType(Value);
			Dest++;
		}
	}

	FORCEINLINE void InternalMoveEmplace(TPointer InBegin, TPointer InEnd, TPointer Dest)
	{
		// This function assumes that there is no overlap
		if constexpr (std::is_trivially_move_constructible<TType>())
		{
			const TSize Count = static_cast<TSize>(InEnd - InBegin);
			const TSize CpySize = Count * sizeof(TType);
			memcpy(Dest, InBegin, CpySize);
		}
		else
		{
			while (InBegin != InEnd)
			{
				new(Dest) TType(Move(*InBegin));
				InBegin++;
				Dest++;
			}
		}
	}

	FORCEINLINE void InternalMoveEmplace(std::initializer_list<TType> IList, TPointer Dest)
	{
		// This function assumes that there is no overlap
		for (const TType& Value : IList)
		{
			new(Dest) TType(Move(Value));
			Dest++;
		}
	}

	FORCEINLINE void InternalMemmoveBackwards(TPointer InBegin, TPointer InEnd, TPointer Dest)
	{
		VALIDATE(InBegin <= InEnd);
		if (InBegin == InEnd)
		{
			return;
		}

		VALIDATE(InEnd <= InternalRealEnd());
		VALIDATE(InternalIsIteratorOwner(ConstIterator(Dest)));

		// Move each object in the range to the destination
		// This functions assumes that the range is from this TArray (I.e It can overlap)

		const TSize Count = static_cast<TSize>(InEnd - InBegin);
		if constexpr (std::is_trivially_move_assignable<TType>())
		{
			const TSize CpySize = Count * sizeof(TType);
			memmove(Dest, InBegin, CpySize);
		}
		else
		{
			while (InBegin != InEnd)
			{
				(*Dest) = Move(*InBegin);
				Dest++;
				InBegin++;
			}
		}
	}

	FORCEINLINE void InternalMemmoveForward(TPointer InBegin, TPointer InEnd, TPointer Dest)
	{
		// Move each object in the range to the destination, starts in the "End" and moves forward
		const TSize Count = static_cast<TSize>(InEnd - InBegin);
		if constexpr (std::is_trivially_move_assignable<TType>())
		{
			if (Count > 0)
			{
				const TSize CpySize		= Count * sizeof(TType);
				const TSize OffsetSize	= (Count - 1) * sizeof(TType);
				memmove(reinterpret_cast<Char*>(Dest) - OffsetSize, InBegin, CpySize);
			}
		}
		else
		{
			while (InEnd != InBegin)
			{
				InEnd--;
				(*Dest) = Move(*InEnd);
				Dest--;
			}
		}
	}

	FORCEINLINE void InternalDestruct(TPointer Pos)
	{
		// Calls the destructor (If it needs to be called)
		if constexpr (std::is_trivially_destructible<TType>() == false)
		{
			(*Pos).~TType();
		}
	}

	FORCEINLINE void InternalDestructRange(TPointer InBegin, TPointer InEnd)
	{
		VALIDATE(InBegin <= InEnd);

		// Calls the destructor for every object in the range (If it needs to be called)
		if constexpr (std::is_trivially_destructible<TType>() == false)
		{
			while (InBegin != InEnd)
			{
				InternalDestruct(InBegin);
				InBegin++;
			}
		}
	}

	FORCEINLINE void InternalConstructRange(TPointer InBegin, TPointer InEnd)
	{
		VALIDATE(InBegin <= InEnd);

		// Calls the default constructor for every object in the range (If it can be called)
		if constexpr (std::is_default_constructible<TType>())
		{
			while (InBegin != InEnd)
			{
				new(InBegin) TType();
				InBegin++;
			}
		}
	}

	TPointer Data;
	TSize Size;
	TSize Capacity;
};