#pragma once
#include "TUtilities.h"

#include <initializer_list>
#include <iterator>

/*
* TArray - Dynamic Array similar to std::vector
*/

template<typename T>
class TArray
{
public:
    typedef UInt32 SizeType;

    /*
    * TIterator
    */

    template<typename TIteratorType>
    class TIterator
    {
        friend class TArray;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using difference_type   = SizeType;
        using value_type        = TIteratorType;
        using pointer           = TIteratorType*;
        using reference         = TIteratorType&;

        ~TIterator() = default;

        FORCEINLINE TIterator(TIteratorType* InPtr = nullptr) noexcept
            : Ptr(InPtr)
        {
        }

        FORCEINLINE TIteratorType* operator->() const noexcept
        {
            return Ptr;
        }

        FORCEINLINE TIteratorType& operator*() const noexcept
        {
            return *Ptr;
        }

        FORCEINLINE TIterator operator++() noexcept
        {
            Ptr++;
            return *this;
        }

        FORCEINLINE TIterator operator++(Int32) noexcept
        {
            TIterator Temp = *this;
            Ptr++;
            return Temp;
        }

        FORCEINLINE TIterator operator--() noexcept
        {
            Ptr--;
            return *this;
        }

        FORCEINLINE TIterator operator--(Int32) noexcept
        {
            TIterator Temp = *this;
            Ptr--;
            return Temp;
        }

        FORCEINLINE TIterator operator+(Int32 Offset) const noexcept
        {
            TIterator Temp = *this;
            return Temp += Offset;
        }

        FORCEINLINE TIterator operator-(Int32 Offset) const noexcept
        {
            TIterator Temp = *this;
            return Temp -= Offset;
        }

        FORCEINLINE difference_type operator-(TIterator Other) const noexcept
        {
            return static_cast<difference_type>(Ptr - Other.Ptr);
        }

        FORCEINLINE TIterator& operator+=(Int32 Offset) noexcept
        {
            Ptr += Offset;
            return *this;
        }

        FORCEINLINE TIterator& operator-=(Int32 Offset) noexcept
        {
            Ptr -= Offset;
            return *this;
        }

        FORCEINLINE bool operator==(const TIterator& Other) const noexcept
        {
            return (Ptr == Other.Ptr);
        }

        FORCEINLINE bool operator!=(const TIterator& Other) const noexcept
        {
            return (Ptr != Other.Ptr);
        }

        FORCEINLINE bool operator<(const TIterator& Other) const noexcept
        {
            return Ptr < Other.Ptr;
        }

        FORCEINLINE bool operator<=(const TIterator& Other) const noexcept
        {
            return Ptr <= Other.Ptr;
        }

        FORCEINLINE bool operator>(const TIterator& Other) const noexcept
        {
            return Ptr > Other.Ptr;
        }

        FORCEINLINE bool operator>=(const TIterator& Other) const noexcept
        {
            return Ptr >= Other.Ptr;
        }

        FORCEINLINE operator TIterator<const TIteratorType>() const noexcept
        {
            return TIterator<const TIteratorType>(Ptr);
        }

    protected:
        TIteratorType* Ptr;
    };

    typedef TIterator<T>       Iterator;
    typedef TIterator<const T> ConstIterator;

    /*
    * TReverseIterator - Reverse Iterator: Stores for example End(), but will reference End() - 1
    */

    template<typename TIteratorType>
    class TReverseIterator
    {
        friend class TArray;

    public:
        ~TReverseIterator() = default;

        FORCEINLINE TReverseIterator(TIteratorType* InPtr = nullptr) noexcept
            : Ptr(InPtr)
        {
        }

        FORCEINLINE TIteratorType* operator->() const noexcept
        {
            return (Ptr - 1);
        }

        FORCEINLINE TIteratorType& operator*() const noexcept
        {
            return *(Ptr - 1);
        }

        FORCEINLINE TReverseIterator operator++() noexcept
        {
            Ptr--;
            return *this;
        }

        FORCEINLINE TReverseIterator operator++(Int32) noexcept
        {
            TReverseIterator Temp = *this;
            Ptr--;
            return Temp;
        }

        FORCEINLINE TReverseIterator operator--() noexcept
        {
            Ptr++;
            return *this;
        }

        FORCEINLINE TReverseIterator operator--(Int32) noexcept
        {
            TReverseIterator Temp = *this;
            Ptr++;
            return Temp;
        }

        FORCEINLINE TReverseIterator operator+(Int32 Offset) const noexcept
        {
            TReverseIterator Temp = *this;
            return Temp += Offset;
        }

        FORCEINLINE TReverseIterator operator-(Int32 Offset) const noexcept
        {
            TReverseIterator Temp = *this;
            return Temp -= Offset;
        }

        FORCEINLINE TReverseIterator& operator+=(Int32 Offset) noexcept
        {
            Ptr -= Offset;
            return *this;
        }

        FORCEINLINE TReverseIterator& operator-=(Int32 Offset) noexcept
        {
            Ptr += Offset;
            return *this;
        }

        FORCEINLINE bool operator==(const TReverseIterator& Other) const noexcept
        {
            return (Ptr == Other.Ptr);
        }

        FORCEINLINE bool operator!=(const TReverseIterator& Other) const noexcept
        {
            return (Ptr != Other.Ptr);
        }

        FORCEINLINE operator TReverseIterator<const TIteratorType>() const noexcept
        {
            return TReverseIterator<const TIteratorType>(Ptr);
        }

    protected:
        TIteratorType* Ptr;
    };

    typedef TReverseIterator<T>			ReverseIterator;
    typedef TReverseIterator<const T>	ReverseConstIterator;

public:
    FORCEINLINE TArray() noexcept
        : ArrayPtr(nullptr)
        , ArraySize(0)
        , ArrayCapacity(0)
    {
    }

    FORCEINLINE explicit TArray(SizeType Size) noexcept
        : ArrayPtr(nullptr)
        , ArraySize(0)
        , ArrayCapacity(0)
    {
        InternalConstruct(Size);
    }

    FORCEINLINE explicit TArray(SizeType Size, const T& Value) noexcept
        : ArrayPtr(nullptr)
        , ArraySize(0)
        , ArrayCapacity(0)
    {
        InternalConstruct(Size, Value);
    }

    template<typename TInputIterator>
    FORCEINLINE explicit TArray(TInputIterator InBegin, TInputIterator InEnd) noexcept
        : ArrayPtr(nullptr)
        , ArraySize(0)
        , ArrayCapacity(0)
    {
        InternalConstruct(InBegin, InEnd);
    }

    FORCEINLINE TArray(std::initializer_list<T> IList) noexcept
        : ArrayPtr(nullptr)
        , ArraySize(0)
        , ArrayCapacity(0)
    {
        InternalConstruct(IList.begin(), IList.end());
    }

    FORCEINLINE TArray(const TArray& Other) noexcept
        : ArrayPtr(nullptr)
        , ArraySize(0)
        , ArrayCapacity(0)
    {
        InternalConstruct(Other.Begin(), Other.End());
    }

    FORCEINLINE TArray(TArray&& Other) noexcept
        : ArrayPtr(nullptr)
        , ArraySize(0)
        , ArrayCapacity(0)
    {
        InternalMove(::Forward<TArray>(Other));
    }

    FORCEINLINE ~TArray() 
    {
        Clear();
        InternalReleaseData();
        ArrayCapacity = 0;
    }

    FORCEINLINE void Clear() noexcept
    {
        InternalDestructRange(ArrayPtr, ArrayPtr + ArraySize);
        ArraySize = 0;
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

    template<typename TInputIterator>
    FORCEINLINE void Assign(TInputIterator InBegin, TInputIterator InEnd) noexcept
    {
        Clear();
        InternalConstruct(InBegin, InEnd);
    }

    FORCEINLINE void Assign(std::initializer_list<T> IList) noexcept
    {
        Clear();
        InternalConstruct(IList.begin(), IList.end());
    }

    FORCEINLINE void Fill(const T& Value) noexcept
    {
        T* ArrayBegin = ArrayPtr;
        T* ArrayEnd   = ArrayBegin + ArraySize;

        while (ArrayBegin != ArrayEnd)
        {
            *ArrayBegin = Value;
            ArrayBegin++;
        }
    }

    FORCEINLINE void Fill(T&& Value) noexcept
    {
        T* ArrayBegin = ArrayPtr;
        T* ArrayEnd   = ArrayBegin + ArraySize;

        while (ArrayBegin != ArrayEnd)
        {
            *ArrayBegin = ::Move(Value);
            ArrayBegin++;
        }
    }

    FORCEINLINE void Resize(SizeType InSize) noexcept
    {
        if (InSize > ArraySize)
        {
            if (InSize > ArrayCapacity)
            {
                InternalRealloc(InSize);
            }

            InternalDefaultConstructRange(ArrayPtr + ArraySize, ArrayPtr + InSize);
        }
        else if (InSize < ArraySize)
        {
            InternalDestructRange(ArrayPtr + InSize, ArrayPtr + ArraySize);
        }

        ArraySize = InSize;
    }

    FORCEINLINE void Resize(SizeType InSize, const T& Value) noexcept
    {
        if (InSize > ArraySize)
        {
            if (InSize > ArrayCapacity)
            {
                InternalRealloc(InSize);
            }

            InternalCopyEmplace(InSize - ArraySize, Value, ArrayPtr + ArraySize);
        }
        else if (InSize < ArraySize)
        {
            InternalDestructRange(ArrayPtr + InSize, ArrayPtr + ArraySize);
        }

        ArraySize = InSize;
    }

    FORCEINLINE void Reserve(SizeType InCapacity) noexcept
    {
        if (InCapacity != ArrayCapacity)
        {
            SizeType OldSize = ArraySize;
            if (InCapacity < ArraySize)
            {
                ArraySize = InCapacity;
            }

            T* TempData = InternalAllocateElements(InCapacity);
            InternalMoveEmplace(ArrayPtr, ArrayPtr + ArraySize, TempData);
            InternalDestructRange(ArrayPtr, ArrayPtr + OldSize);

            InternalReleaseData();
            ArrayPtr      = TempData;
            ArrayCapacity = InCapacity;
        }
    }

    template<typename... TArgs>
    FORCEINLINE T& EmplaceBack(TArgs&&... Args) noexcept
    {
        if (ArraySize >= ArrayCapacity)
        {
            const SizeType NewCapacity = InternalGetResizeFactor();
            InternalRealloc(NewCapacity);
        }

        T* DataEnd = ArrayPtr + ArraySize;
        new(reinterpret_cast<Void*>(DataEnd)) T(::Forward<TArgs>(Args)...);
        ArraySize++;
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
        VALIDATE(InternalIsIteratorOwner(Pos));

        if (Pos == End())
        {
            const SizeType OldSize = ArraySize;
            EmplaceBack(::Forward<TArgs>(Args)...);
            return End() - 1;
        }

        const SizeType Index = InternalIndex(Pos);
        T* DataBegin = ArrayPtr + Index;
        if (ArraySize >= ArrayCapacity)
        {
            const SizeType NewCapacity = InternalGetResizeFactor();
            InternalEmplaceRealloc(NewCapacity, DataBegin, 1);
            DataBegin = ArrayPtr + Index;
        }
        else
        {
            // Construct the range so that we can move to It
            T* DataEnd = ArrayPtr + ArraySize;
            InternalDefaultConstructRange(DataEnd, DataEnd + 1);
            InternalMemmoveForward(DataBegin, DataEnd, DataEnd);
            InternalDestruct(DataBegin);
        }

        new (reinterpret_cast<Void*>(DataBegin)) T(::Forward<TArgs>(Args)...);
        ArraySize++;
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
        VALIDATE(InternalIsIteratorOwner(Pos));

        if (Pos == End())
        {
            for (const T& Value : IList)
            {
                EmplaceBack(::Move(Value));
            }

            return End() - 1;
        }

        const SizeType ListSize = static_cast<SizeType>(IList.size());
        const SizeType NewSize  = ArraySize + ListSize;
        const SizeType Index    = InternalIndex(Pos);

        T* RangeBegin = ArrayPtr + Index;
        if (NewSize >= ArrayCapacity)
        {
            const SizeType NewCapacity = InternalGetResizeFactor(NewSize);
            InternalEmplaceRealloc(NewCapacity, RangeBegin, ListSize);
            RangeBegin = ArrayPtr + Index;
        }
        else
        {
            // Construct the range so that we can move to It
            T* DataEnd    = ArrayPtr + ArraySize;
            T* NewDataEnd = ArrayPtr + ArraySize + ListSize;
            T* RangeEnd   = RangeBegin + ListSize;
            InternalDefaultConstructRange(DataEnd, NewDataEnd);
            InternalMemmoveForward(RangeBegin, DataEnd, NewDataEnd - 1);
            InternalDestructRange(RangeBegin, RangeEnd);
        }

        // TODO: Get rid of const_cast
        InternalMoveEmplace(const_cast<T*>(IList.begin()), const_cast<T*>(IList.end()), RangeBegin);
        ArraySize = NewSize;
        return Iterator(RangeBegin);
    }

    template<typename TInputIterator>
    FORCEINLINE Iterator Insert(Iterator Pos, TInputIterator InBegin, TInputIterator InEnd) noexcept
    {
        return Insert(ConstIterator(Pos.Ptr), InBegin, InEnd);
    }

    template<typename TInputIterator>
    FORCEINLINE Iterator Insert(ConstIterator Pos, TInputIterator InBegin, TInputIterator InEnd) noexcept
    {
        VALIDATE(InternalIsIteratorOwner(Pos));

        if (Pos == End())
        {
            for (TInputIterator It = InBegin; It != InEnd; It++)
            {
                EmplaceBack(*It);
            }

            return End() - 1;
        }

        const SizeType RangeSize = InternalDistance(InBegin, InEnd);
        const SizeType NewSize   = ArraySize + RangeSize;
        const SizeType Index     = InternalIndex(Pos);

        T* RangeBegin = ArrayPtr + Index;
        if (NewSize >= ArrayCapacity)
        {
            const SizeType NewCapacity = InternalGetResizeFactor(NewSize);
            InternalEmplaceRealloc(NewCapacity, RangeBegin, RangeSize);
            RangeBegin = ArrayPtr + Index;
        }
        else
        {
            // Construct the range so that we can move to it
            T* DataEnd    = ArrayPtr + ArraySize;
            T* NewDataEnd = ArrayPtr + ArraySize + RangeSize;
            T* RangeEnd   = RangeBegin + RangeSize;
            InternalDefaultConstructRange(DataEnd, NewDataEnd);
            InternalMemmoveForward(RangeBegin, DataEnd, NewDataEnd - 1);
            InternalDestructRange(RangeBegin, RangeEnd);
        }

        InternalCopyEmplace(InBegin.Ptr, InEnd.Ptr, RangeBegin);
        ArraySize = NewSize;
        return Iterator(RangeBegin);
    }

    FORCEINLINE void PopBack() noexcept
    {
        if (!IsEmpty())
        {
            InternalDestruct(ArrayPtr + (--ArraySize));
        }
    }

    FORCEINLINE Iterator Erase(Iterator Pos) noexcept
    {
        return Erase(ConstIterator(Pos.Ptr));
    }

    FORCEINLINE Iterator Erase(ConstIterator Pos) noexcept
    {
        VALIDATE(InternalIsIteratorOwner(Pos));
        VALIDATE(Pos < End());

        if (Pos == End() - 1)
        {
            PopBack();
            return End();
        }

        const SizeType Index = InternalIndex(Pos);
        T* DataBegin = ArrayPtr + Index;
        T* DataEnd   = ArrayPtr + ArraySize;
        InternalMemmoveBackwards(DataBegin + 1, DataEnd, DataBegin);
        InternalDestruct(DataEnd - 1);

        ArraySize--;
        return Iterator(DataBegin);
    }

    FORCEINLINE Iterator Erase(Iterator InBegin, Iterator InEnd) noexcept
    {
        return Erase(ConstIterator(InBegin.Ptr), ConstIterator(InEnd.Ptr));
    }

    FORCEINLINE Iterator Erase(ConstIterator InBegin, ConstIterator InEnd) noexcept
    {
        VALIDATE(InBegin < InEnd);
        VALIDATE(InternalIsRangeOwner(InBegin, InEnd));

        T* DataBegin = ArrayPtr + InternalIndex(InBegin);
        T* DataEnd   = ArrayPtr + InternalIndex(InEnd);

        const SizeType elementCount = InternalDistance(DataBegin, DataEnd);
        if (InEnd == End())
        {
            InternalDestructRange(DataBegin, DataEnd);
        }
        else
        {
            T* RealEnd = ArrayPtr + ArraySize;
            InternalMemmoveBackwards(DataEnd, RealEnd, DataBegin);
            InternalDestructRange(RealEnd - elementCount, RealEnd);
        }

        ArraySize -= elementCount;
        return Iterator(DataBegin);
    }

    FORCEINLINE void Swap(TArray& Other) noexcept
    {
        TArray TempArray(::Move(*this));
        *this = ::Move(Other);
        Other = ::Move(TempArray);
    }

    FORCEINLINE void ShrinkToFit() noexcept
    {
        if (ArrayCapacity > ArraySize)
        {
            InternalRealloc(ArraySize);
        }
    }

    FORCEINLINE bool IsEmpty() const noexcept
    {
        return (ArraySize == 0);
    }

    FORCEINLINE T& Front() noexcept
    {
        VALIDATE(ArraySize > 0);
        return ArrayPtr[0];
    }

    FORCEINLINE Iterator Begin() noexcept
    {
        return Iterator(ArrayPtr);
    }

    FORCEINLINE Iterator End() noexcept
    {
        return Iterator(ArrayPtr + ArraySize);
    }

    FORCEINLINE ConstIterator Begin() const noexcept
    {
        return Iterator(ArrayPtr);
    }

    FORCEINLINE ConstIterator End() const noexcept
    {
        return Iterator(ArrayPtr + ArraySize);
    }

    FORCEINLINE const T& Front() const noexcept
    {
        VALIDATE(ArraySize > 0);
        return ArrayPtr[0];
    }

    FORCEINLINE T& Back() noexcept
    {
        VALIDATE(ArraySize > 0);
        return ArrayPtr[ArraySize - 1];
    }

    FORCEINLINE const T& Back() const noexcept
    {
        VALIDATE(ArraySize > 0);
        return ArrayPtr[ArraySize - 1];
    }

    FORCEINLINE T* Data() noexcept
    {
        return ArrayPtr;
    }

    FORCEINLINE const T* Data() const noexcept
    {
        return ArrayPtr;
    }

    FORCEINLINE SizeType Size() const noexcept
    {
        return ArraySize;
    }

    FORCEINLINE SizeType SizeInBytes() const noexcept
    {
        return ArraySize * sizeof(T);
    }

    FORCEINLINE SizeType Capacity() const noexcept
    {
        return ArrayCapacity;
    }

    FORCEINLINE SizeType CapacityInBytes() const noexcept
    {
        return ArrayCapacity * sizeof(T);
    }

    FORCEINLINE T& At(SizeType Index) noexcept
    {
        VALIDATE(Index < ArraySize);
        return ArrayPtr[Index];
    }

    FORCEINLINE const T& At(SizeType Index) const noexcept
    {
        VALIDATE(Index < ArraySize);
        return ArrayPtr[Index];
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
            InternalMove(::Forward<TArray>(Other));
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
        return Iterator(ArrayPtr);
    }

    FORCEINLINE Iterator end() noexcept
    {
        return Iterator(ArrayPtr + ArraySize);
    }

    FORCEINLINE ConstIterator begin() const noexcept
    {
        return ConstIterator(ArrayPtr);
    }

    FORCEINLINE ConstIterator end() const noexcept
    {
        return ConstIterator(ArrayPtr + ArraySize);
    }

    FORCEINLINE ConstIterator cbegin() const noexcept
    {
        return ConstIterator(ArrayPtr);
    }

    FORCEINLINE ConstIterator cend() const noexcept
    {
        return ConstIterator(ArrayPtr + ArraySize);
    }

    FORCEINLINE ReverseIterator rbegin() noexcept
    {
        return ReverseIterator(ArrayPtr + ArraySize);
    }

    FORCEINLINE ReverseIterator rend() noexcept
    {
        return ReverseIterator(ArrayPtr);
    }

    FORCEINLINE ReverseConstIterator rbegin() const noexcept
    {
        return ReverseConstIterator(ArrayPtr + ArraySize);
    }

    FORCEINLINE ReverseConstIterator rend() const noexcept
    {
        return ReverseConstIterator(ArrayPtr);
    }

    FORCEINLINE ReverseConstIterator crbegin() const noexcept
    {
        return ReverseConstIterator(ArrayPtr + ArraySize);
    }

    FORCEINLINE ReverseConstIterator crend() const noexcept
    {
        return ReverseConstIterator(ArrayPtr);
    }

private:
    // Check is the iterator belongs to this TArray
    FORCEINLINE bool InternalIsRangeOwner(ConstIterator InBegin, ConstIterator InEnd) const noexcept
    {
        return (InBegin < InEnd) && (InBegin >= begin()) && (InEnd <= end());
    }

    FORCEINLINE bool InternalIsIteratorOwner(ConstIterator It) const noexcept
    {
        return (It >= begin()) && (It <= end());
    }

    // Helpers
    template<typename TInputIterator>
    FORCEINLINE const T* InternalUnwrapConst(TInputIterator It) noexcept
    {
        if constexpr (std::is_pointer<TInputIterator>())
        {
            return It;
        }
        else
        {
            return It.Ptr;
        }
    }

    template<typename TInputIterator>
    FORCEINLINE SizeType InternalDistance(TInputIterator InBegin, TInputIterator InEnd) noexcept
    {
        constexpr bool IsPointer        = std::is_pointer<TInputIterator>();
        constexpr bool IsCustomIterator = std::is_same<TInputIterator, Iterator>() || std::is_same<TInputIterator, ConstIterator>();

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

    template<typename TInputIterator>
    FORCEINLINE SizeType InternalIndex(TInputIterator Pos) noexcept
    {
        return static_cast<SizeType>(InternalUnwrapConst(Pos) - InternalUnwrapConst(begin()));
    }

    FORCEINLINE SizeType InternalGetResizeFactor() const noexcept
    {
        return InternalGetResizeFactor(ArraySize);
    }

    FORCEINLINE SizeType InternalGetResizeFactor(SizeType BaseSize) const noexcept
    {
        return BaseSize + (ArrayCapacity / 2) + 1;
    }

    FORCEINLINE T* InternalAllocateElements(SizeType InCapacity) noexcept
    {
        constexpr SizeType ElementByteSize = sizeof(T);
        const SizeType SizeInBytes = ElementByteSize * InCapacity;
        return reinterpret_cast<T*>(malloc(static_cast<size_t>(SizeInBytes)));
    }

    FORCEINLINE void InternalReleaseData() noexcept
    {
        if (ArrayPtr)
        {
            free(ArrayPtr);
            ArrayPtr = nullptr;
        }
    }

    FORCEINLINE void InternalAllocData(SizeType InCapacity) noexcept
    {
        if (InCapacity > ArrayCapacity)
        {
            InternalReleaseData();
            ArrayPtr = InternalAllocateElements(InCapacity);
            ArrayCapacity = InCapacity;
        }
    }

    FORCEINLINE void InternalRealloc(SizeType InCapacity) noexcept
    {
        T* TempData = InternalAllocateElements(InCapacity);
        InternalMoveEmplace(ArrayPtr, ArrayPtr + ArraySize, TempData);
        InternalDestructRange(ArrayPtr, ArrayPtr + ArraySize);

        InternalReleaseData();
        ArrayPtr = TempData;
        ArrayCapacity = InCapacity;
    }

    FORCEINLINE void InternalEmplaceRealloc(SizeType InCapacity, T* EmplacePos, SizeType Count) noexcept
    {
        VALIDATE(InCapacity >= ArraySize + Count);

        const SizeType Index = InternalIndex(EmplacePos);
        T* TempData = InternalAllocateElements(InCapacity);
        InternalMoveEmplace(ArrayPtr, EmplacePos, TempData);
        if (EmplacePos != ArrayPtr + ArraySize)
        {
            InternalMoveEmplace(EmplacePos, ArrayPtr + ArraySize, TempData + Index + Count);
        }

        InternalDestructRange(ArrayPtr, ArrayPtr + ArraySize);

        InternalReleaseData();
        ArrayPtr = TempData;

        ArrayCapacity = InCapacity;
    }

    // Construct
    FORCEINLINE void InternalConstruct(SizeType InSize) noexcept
    {
        if (InSize > 0)
        {
            InternalAllocData(InSize);
            ArraySize = InSize;
            InternalDefaultConstructRange(ArrayPtr, ArrayPtr + InSize);
        }
    }

    FORCEINLINE void InternalConstruct(SizeType InSize, const T& Value) noexcept
    {
        if (InSize > 0)
        {
            InternalAllocData(InSize);
            InternalCopyEmplace(InSize, Value, ArrayPtr);
            ArraySize = InSize;
        }
    }

    template<typename TInputIterator>
    FORCEINLINE void InternalConstruct(TInputIterator InBegin, TInputIterator InEnd) noexcept
    {
        const SizeType Distance = InternalDistance(InBegin, InEnd);
        if (Distance > 0)
        {
            InternalAllocData(Distance);
            InternalCopyEmplace(InBegin, InEnd, ArrayPtr);
            ArraySize = Distance;
        }
    }

    FORCEINLINE void InternalMove(TArray&& Other) noexcept
    {
        InternalReleaseData();

        ArrayPtr      = Other.ArrayPtr;
        ArraySize     = Other.ArraySize;
        ArrayCapacity = Other.ArrayCapacity;

        Other.ArrayPtr      = nullptr;
        Other.ArraySize     = 0;
        Other.ArrayCapacity = 0;
    }

    // Emplace
    template<typename TInputIterator>
    FORCEINLINE void InternalCopyEmplace(TInputIterator InBegin, TInputIterator InEnd, T* Dest) noexcept
    {
        // This function assumes that there is no overlap
        constexpr bool IsTrivial = std::is_trivially_copy_constructible<T>();
        constexpr bool IsPointer = std::is_pointer<TInputIterator>();
        constexpr bool IsCustomIterator = std::is_same<TInputIterator, Iterator>() || std::is_same<TInputIterator, ConstIterator>();

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

        VALIDATE(InEnd <= ArrayPtr + ArrayCapacity);

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
                const SizeType CpySize    = Count * sizeof(T);
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
        VALIDATE(InEnd - InBegin <= ArrayCapacity);

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
    T*       ArrayPtr;
    SizeType ArraySize;
    SizeType ArrayCapacity;
};
