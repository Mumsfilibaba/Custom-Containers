#include "TArrayView_Test.h"

#include "../Containers/Array.h"
#include "../Containers/StaticArray.h"
#include "../Containers/ArrayView.h"

#include <iostream>

template<typename T>
static void PrintArrayView(const TArrayView<T>& View)
{
    std::cout << "------------------------------" << std::endl;
    for (UInt32 i = 0; i < View.Size(); i++)
    {
        std::cout << View[i] << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
}

template<typename T>
static void PrintArrayViewRangeBased(const TArrayView<T>& View)
{
    std::cout << "------------------------------" << std::endl;
    for (const T& Element : View)
    {
        std::cout << Element << std::endl;
    }
    std::cout << "------------------------------" << std::endl;
}

void TArrayView_Test()
{
    std::cout << std::endl << "----------TArrayView----------" << std::endl << std::endl;
    std::cout << "Testing Constructors" << std::endl;

    TArray<UInt32> Arr0 = { 1, 2, 3, 4 };
    TArrayView<UInt32> ArrView0 = TArrayView<UInt32>(Arr0);

    TStaticArray<UInt32, 4> Arr1 = { 11, 12, 13, 14 };
    TArrayView<UInt32> ArrView1 = TArrayView<UInt32>(Arr1);

    UInt32 Arr2[] = { 21, 22, 23, 24 };
    TArrayView<UInt32> ArrView2 = TArrayView<UInt32>(Arr2);

    UInt32* DynamicPtr = new UInt32[]{ 31, 32, 33, 34, 35 };
    TArrayView<UInt32> ArrView3 = TArrayView<UInt32>(DynamicPtr, DynamicPtr + 5);

    std::cout << "Testing At and operator[]" << std::endl;
    PrintArrayView(ArrView0);
    PrintArrayView(ArrView1);
    PrintArrayView(ArrView2);
    PrintArrayView(ArrView3);

    std::cout << "Testing range-based for-loops" << std::endl;
    PrintArrayViewRangeBased(ArrView0);
    PrintArrayViewRangeBased(ArrView1);
    PrintArrayViewRangeBased(ArrView2);
    PrintArrayViewRangeBased(ArrView3);

    std::cout << "Testing copy/move constructor" << std::endl;
    TArrayView<UInt32> ArrView4 = ArrView1;
    TArrayView<UInt32> ArrView5 = ::Move(ArrView0);

    PrintArrayViewRangeBased(ArrView4);
    PrintArrayViewRangeBased(ArrView5);

    std::cout << "Testing Size/SizeInBytes" << std::endl;
    std::cout << "Size: " << ArrView4.Size() << std::endl;
    std::cout << "SizeInBytes: " << ArrView4.SizeInBytes() << std::endl;

    std::cout << "Testing Swap" << std::endl;
    std::cout << "-----------Before----------" << std::endl;
    PrintArrayViewRangeBased(ArrView4);
    PrintArrayViewRangeBased(ArrView5);

    ArrView4.Swap(ArrView5);

    std::cout << "-----------After-----------" << std::endl;
    PrintArrayViewRangeBased(ArrView4);
    PrintArrayViewRangeBased(ArrView5);

    delete DynamicPtr;
}