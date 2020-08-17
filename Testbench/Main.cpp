#include <iostream>
#include <cassert>
#include <utility>
#include <vector>
#include <chrono>
#include <string>
#include <crtdbg.h>

#include "TArray.h"

template<typename T>
void PrintArr(const TArray<T>& Arr, const std::string& Name = "")
{
	std::cout << Name << std::endl;
	std::cout << "--------------------------------" << std::endl;

	for (auto i : Arr)
	{
		std::cout << i << std::endl;
	}

	std::cout << "Size: " << Arr.GetSize() << std::endl;
	std::cout << "Capacity: " << Arr.GetCapacity() << std::endl;

	std::cout << "--------------------------------" << std::endl << std::endl;
}

template<typename T>
void PrintArr(const std::vector<T>& Arr, const std::string& Name = "")
{
	std::cout << Name << std::endl;
	std::cout << "--------------------------------" << std::endl;

	for (auto i : Arr)
	{
		std::cout << i << std::endl;
	}

	std::cout << "Size: " << Arr.size() << std::endl;
	std::cout << "Capacity: " << Arr.capacity() << std::endl;

	std::cout << "--------------------------------" << std::endl << std::endl;
}

#define PrintArr(Arr) PrintArr(Arr, #Arr)

void BenchMark()
{
	// Performance
	std::cout << std::endl << "Benchmark" << std::endl;
	const Uint32 TestCount = 100;

	// Insert
#if 1
	{
		const Uint32 Iterations = 1000;
		std::cout << std::endl << "Insert (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				std::vector<std::string> Strings0;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Strings0.insert(Strings0.begin(), "My name is jeff");
				}
				auto t2 = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
				Total += duration;
			}

			std::cout << "std::vector:" << Total / TestCount << "ns" << std::endl;
		}

		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				TArray<std::string> Strings1;

				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Strings1.Insert(Strings1.begin(), "My name is jeff");
				}
				auto t2 = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
				Total += duration;
			}
			std::cout << "TArray     :" << Total / Iterations << "ns" << std::endl;
		}
	}
#endif

#if 1
	// Emplace
	{
		const Uint32 Iterations = 1000;
		std::cout << std::endl << "Emplace (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				std::vector<std::string> Strings0;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Strings0.emplace(Strings0.begin(), "My name is jeff");
				}
				auto t2 = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
				Total += duration;
			}

			std::cout << "std::vector:" << Total / TestCount << "ns" << std::endl;
		}

		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				TArray<std::string> Strings1;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Strings1.Emplace(Strings1.begin(), "My name is jeff");
				}
				auto t2 = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
				Total += duration;
			}
			std::cout << "TArray     :" << Total / Iterations << "ns" << std::endl;
		}
	}
#endif

#if 1
	// PushBack
	{
		const Uint32 Iterations = 100000;
		std::cout << std::endl << "PushBack (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				std::vector<std::string> Strings0;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Strings0.push_back("My name is jeff");
				}
				auto t2 = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
				Total += duration;
			}

			std::cout << "std::vector:" << Total / TestCount << "ns" << std::endl;
		}

		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				TArray<std::string> Strings1;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Strings1.PushBack("My name is jeff");
				}
				auto t2 = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
				Total += duration;
			}
			std::cout << "TArray     :" << Total / Iterations << "ns" << std::endl;
		}
	}
#endif

#if 1
	// EmplaceBack
	{
		const Uint32 Iterations = 100000;
		std::cout << std::endl << "EmplaceBack (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				std::vector<std::string> Strings0;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Strings0.emplace_back("My name is jeff");
				}
				auto t2 = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
				Total += duration;
			}

			std::cout << "std::vector:" << Total / TestCount << "ns" << std::endl;
		}

		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				TArray<std::string> Strings1;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Strings1.EmplaceBack("My name is jeff");
				}
				auto t2 = std::chrono::high_resolution_clock::now();

				auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
				Total += duration;
			}
			std::cout << "TArray     :" << Total / Iterations << "ns" << std::endl;
		}
	}
#endif
}

int main(int Argc, const char* Argv[])
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	std::string ArgvStr = Argv[0];

	std::cout << "Testing TArray<std::string>" << std::endl;

	std::cout << std::endl << "Testing Constructors" << std::endl;
	// Test constructors
	TArray<std::string> Strings0;
	TArray<std::string> Strings1(5, "Hello");
	TArray<std::string> Strings2(Strings1.Begin(), Strings1.End());
	TArray<std::string> Strings3 =
	{
		"Hello World",
		"TArray",
		"This is a longer teststring"
	};

	PrintArr(Strings0);
	PrintArr(Strings1);
	PrintArr(Strings2);
	PrintArr(Strings3);

	{
		// Test copy an empty array
		TArray<std::string> Strings4 = Strings0;
		// Test copy an array with data
		TArray<std::string> Strings5 = Strings1;

		PrintArr(Strings4);

		std::cout << "Before move" << std::endl;
		PrintArr(Strings5);

		// Test move an array with data
		TArray<std::string> Strings6 = Move(Strings5);

		std::cout << "After move" << std::endl;
		PrintArr(Strings5);
		PrintArr(Strings6);
	}

	// Assign
	std::cout << std::endl << "Testing Assign" << std::endl;
	Strings0.Assign(7, "This is a teststring");
	PrintArr(Strings0);

	Strings1.Assign({ "Test-String #1", "Test-String #2", "Test-String #3" });
	PrintArr(Strings1);

	Strings2.Assign(Strings3.Begin(), Strings3.End());
	PrintArr(Strings2);

	// Resize
	std::cout << std::endl << "Testing Resize" << std::endl;
	
	// Constructing a empty Array to test resize
	TArray<std::string> Strings4;

	std::cout << "Before Resize" << std::endl;
	PrintArr(Strings4);
	PrintArr(Strings3);
	PrintArr(Strings1);

	Strings4.Resize(10, "New String");
	Strings3.Resize(0);
	Strings1.Resize(6, "Hello World");

	std::cout << "After Resize" << std::endl;
	PrintArr(Strings4);
	PrintArr(Strings3);
	PrintArr(Strings1);

	std::cout << "Testing Shrinking Resize" << std::endl;
	Strings4.Resize(2, "New String");
	PrintArr(Strings4);

	Strings4.Resize(15, "New String");
	PrintArr(Strings4);

	// Reserve
	std::cout << std::endl << "Testing Reserve" << std::endl;
	
	std::cout << "Before Reserve" << std::endl;
	PrintArr(Strings4);

	std::cout << "After Reserve" << std::endl;
	Strings4.Reserve(Strings4.GetCapacity());
	PrintArr(Strings4);
	
	std::cout << "Shrinking" << std::endl;
	Strings4.Reserve(5);
	PrintArr(Strings4);

	std::cout << "Growing" << std::endl;
	Strings4.Reserve(10);
	PrintArr(Strings4);

	std::cout << "Resize" << std::endl;
	Strings4.Resize(Strings4.GetCapacity() - 2, "This spot is reserved");
	PrintArr(Strings4);

	// Shrink To Fit
	std::cout << std::endl << "Testing ShrinkToFit" << std::endl;
	
	std::cout << std::endl << "Before ShrinkToFit" << std::endl;
	PrintArr(Strings4);

	Strings4.ShrinkToFit();

	std::cout << std::endl << "After ShrinkToFit" << std::endl;
	PrintArr(Strings4);

	// Assignment
	std::cout << std::endl << "Testing Assignment" << std::endl;
	
	Strings3.Resize(3, "No I am your father");

	std::cout << "Before Assignment" << std::endl;
	PrintArr(Strings0);
	PrintArr(Strings1);
	PrintArr(Strings2);
	PrintArr(Strings3);
	PrintArr(Strings4);
	
	std::cout << "Strings0 = Strings4" << std::endl;
	Strings0 = Strings4;

	std::cout << "Strings1 = Move(Strings3)" << std::endl;
	Strings1 = Move(Strings3);
	
	std::cout << "Strings2 = InitializerList" << std::endl;
	Strings2 =
	{
		"Another String in a InitializerList",
		"Strings are kinda cool",
		"Letters to fill up space in a string"
	};

	std::cout << "After Assignment" << std::endl;
	PrintArr(Strings0);
	PrintArr(Strings1);
	PrintArr(Strings2);
	PrintArr(Strings3);
	PrintArr(Strings4);

	// PushBack
	std::cout << std::endl << "Testing PushBack" << std::endl;
	for (Uint32 I = 0; I < 6; I++)
	{
		Strings2.PushBack("This is Pushed String #" + std::to_string(I));
	}
	PrintArr(Strings2);

	std::cout << std::endl << "Testing PushBack" << std::endl;
	for (Uint32 I = 0; I < 6; I++)
	{
		Strings2.PushBack(ArgvStr);
	}
	PrintArr(Strings2);

	// EmplaceBack
	std::cout << std::endl << "Testing EmplaceBack" << std::endl;
	for (Uint32 I = 0; I < 6; I++)
	{
		Strings2.EmplaceBack("This is an Emplaced String #" + std::to_string(I));
	}
	PrintArr(Strings2);

	// PopBack
	std::cout << std::endl << "Testing PopBack" << std::endl;
	for (Uint32 I = 0; I < 3; I++)
	{
		Strings2.PopBack();
	}
	PrintArr(Strings2);

	// Insert
	std::cout << std::endl << "Testing Insert" << std::endl;
	std::cout << "At front" << std::endl;
	Strings2.Insert(Strings2.Begin(), ArgvStr);
	Strings2.Insert(Strings2.Begin(), "Inserted String");
	Strings2.Insert(Strings2.Begin(), { "Inserted String #1", "Inserted String #2" });
	PrintArr(Strings2);

	std::cout << "At Arbitrary" << std::endl;
	Strings2.Insert(Strings2.Begin() + 2, ArgvStr);
	Strings2.Insert(Strings2.Begin() + 2, "Inserted String Again");
	Strings2.Insert(Strings2.Begin() + 2, { "Inserted String Again #1", "Inserted String Again #2" });
	PrintArr(Strings2);

	std::cout << "At End" << std::endl;
	Strings2.Insert(Strings2.End(), { "Inserted String At End #1", "Inserted String At End #2" });
	PrintArr(Strings2);

	std::cout << "At front after reallocation" << std::endl;
	// Add a shrink to fit to force reallocation
	Strings2.ShrinkToFit();
	Strings2.Insert(Strings2.Begin(), ArgvStr);
	// Add a shrink to fit to force reallocation
	Strings2.ShrinkToFit();
	Strings2.Insert(Strings2.Begin(), "Inserted String Reallocated");
	// Add a shrink to fit to force reallocation
	Strings2.ShrinkToFit();
	Strings2.Insert(Strings2.Begin(), { "Inserted String Reallocated #1", "Inserted String Reallocated #2" });
	PrintArr(Strings2);

	std::cout << "At Arbitrary after reallocation" << std::endl;
	// Add a shrink to fit to force reallocation
	Strings2.ShrinkToFit();
	Strings2.Insert(Strings2.Begin() + 2, ArgvStr);
	// Add a shrink to fit to force reallocation
	Strings2.ShrinkToFit();
	Strings2.Insert(Strings2.Begin() + 2, "Inserted String Again Reallocated");
	// Add a shrink to fit to force reallocation
	Strings2.ShrinkToFit();
	Strings2.Insert(Strings2.Begin() + 2, { "Inserted String Again Reallocated #1", "Inserted String Again Reallocated #2" });
	PrintArr(Strings2);

	std::cout << "At End after reallocation" << std::endl;
	// Add a shrink to fit to force reallocation
	Strings2.ShrinkToFit();
	Strings2.Insert(Strings2.End(), { "Inserted String At End Reallocated #1", "Inserted String At End Reallocated #2" });
	PrintArr(Strings2);

	// Erase
	std::cout << std::endl << "Testing Erase" << std::endl;
	PrintArr(Strings2);
	
	std::cout << "At front" << std::endl;
	Strings2.Erase(Strings2.Begin());
	PrintArr(Strings2);

	std::cout << "At Arbitrary" << std::endl;
	Strings2.Erase(Strings2.Begin() + 2);
	PrintArr(Strings2);

	std::cout << "Range At front" << std::endl;
	Strings2.Erase(Strings2.Begin(), Strings2.Begin() + 2);
	PrintArr(Strings2);

	std::cout << "Range At Arbitrary" << std::endl;
	Strings2.Erase(Strings2.Begin() + 4, Strings2.Begin() + 7);
	PrintArr(Strings2);

	std::cout << "Range At End" << std::endl;
	Strings2.Erase(Strings2.End() - 3, Strings2.End());
	PrintArr(Strings2);

	// Swap
	std::cout << std::endl << "Testing Swap" << std::endl;
	std::cout << "Before" << std::endl;
	PrintArr(Strings0);
	PrintArr(Strings2);

	Strings0.Swap(Strings2);

	std::cout << "After" << std::endl;
	PrintArr(Strings0);
	PrintArr(Strings2);

	//Performance
	BenchMark();

	return 0;
}