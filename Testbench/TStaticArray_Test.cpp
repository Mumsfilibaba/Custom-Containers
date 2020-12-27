#include "TStaticArray_Test.h"

#include "../Containers/TStaticArray.h"

#include <iostream>
#include <array>

void TStaticArray_Test()
{
	std::cout << std::endl << "----------TStaticArray----------" << std::endl << std::endl;
	
	constexpr UInt32 Num = 16;
	TStaticArray<Int32, Num> Numbers;
	const TStaticArray<Int32, Num>& ConstNumbers = Numbers;
	
	std::cout << "Testing At()" << std::endl;
	for (UInt32 i = 0; i < Num; i++)
	{
		Numbers.At(i) = i;
	}
	
	for (UInt32 i = 0; i < Num; i++)
	{
		std::cout << ConstNumbers.At(i) << std::endl;
	}
	
	std::cout << "Testing operator[]" << std::endl;
	for (UInt32 i = 0; i < Num; i++)
	{
		Numbers[i] = Num + i;
	}
	
	for (UInt32 i = 0; i < Num; i++)
	{
		std::cout << ConstNumbers[i] << std::endl;
	}
	
	std::cout << "Testing Front" << std::endl;
	std::cout << "[0]:" << Numbers.Front() << std::endl;
	std::cout << "[0]:" << Numbers.Front() << std::endl;
	
	std::cout << "Testing Back" << std::endl;
	std::cout << "[" << Num-1 << "]:" << Numbers.Back() << std::endl;
	std::cout << "[" << Num-1 << "]:" << Numbers.Back() << std::endl;
	
	std::cout << "Testing Size" << std::endl;
	std::cout << "Size:" << Numbers.Size() << std::endl;
	
	std::cout << "Testing Fill" << std::endl;
	Numbers.Fill(5);
	
	for (UInt32 i = 0; i < Num; i++)
	{
		std::cout << Numbers[i] << std::endl;
	}
}
