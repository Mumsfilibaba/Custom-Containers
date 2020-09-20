#include <iostream>
#include <cassert>
#include <utility>
#include <vector>
#include <chrono>
#include <string>
#include <memory>

#ifdef _WIN32
	#include <crtdbg.h>
#endif

#include <type_traits>

#include "TArray.h"
#include "TSharedPtr.h"
#include "TUniquePtr.h"
#include "TFunction.h"
#include "String.h"

struct Vec3
{
	Vec3()
		: X(0.0)
		, Y(0.0)
		, Z(0.0)
	{
	}

	Vec3(double InX, double InY, double InZ)
		: X(InX)
		, Y(InY)
		, Z(InZ)
	{
	}

	double X;
	double Y;
	double Z;

	operator std::string()
	{
		return std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z);
	}
};

template<typename T>
void PrintArr(const TArray<T>& Arr, const std::string& Name = "")
{
	std::cout << Name << std::endl;
	std::cout << "--------------------------------" << std::endl;

	for (auto i : Arr)
	{
		std::cout << (std::string)i << std::endl;
	}

	std::cout << "Size: " << Arr.Size() << std::endl;
	std::cout << "Capacity: " << Arr.Capacity() << std::endl;

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

struct A
{
	bool Func(Int32 In)
	{
		std::cout << "MemberCall " << In << std::endl;
		return true;
	}
	
	bool Func2(Int32 In)
	{
		std::cout << "MemberCall2 " << In << std::endl;
		return true;
	}
};

static bool Func(Int32 In)
{
	std::cout << "FunctionCall " << In << std::endl;
	return true;
}

static bool Func2(Int32 In)
{
	std::cout << "FunctionCall2 " << In << std::endl;
	return true;
}

#define PrintArr(Arr) PrintArr(Arr, #Arr)

void BenchMark()
{
	// Performance
	std::cout << std::endl << "Benchmark (std::string)" << std::endl;
	const Uint32 TestCount = 100;

	// Insert
#if 1
	{
		const Uint32 Iterations = 10000;
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
					Strings1.Insert(Strings1.Begin(), "My name is jeff");
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
		const Uint32 Iterations = 10000;
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
		const Uint32 Iterations = 10000;
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
		const Uint32 Iterations = 10000;
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

	std::cout << std::endl << "Benchmark (Vec3)" << std::endl;

	// Insert
#if 1
	{
		const Uint32 Iterations = 10000;
		std::cout << std::endl << "Insert (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				std::vector<Vec3> Vectors0;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Vectors0.insert(Vectors0.begin(), Vec3(3.0, 5.0, -6.0));
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
				TArray<Vec3> Vectors1;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Vectors1.Insert(Vectors1.Begin(), Vec3(3.0, 5.0, -6.0));
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
		const Uint32 Iterations = 10000;
		std::cout << std::endl << "Emplace (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				std::vector<Vec3> Vectors0;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Vectors0.emplace(Vectors0.begin(), 3.0, 5.0, -6.0);
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
				TArray<Vec3> Vectors1;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Vectors1.Emplace(Vectors1.Begin(), double(J + 1), 5.0, -6.0);
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
		const Uint32 Iterations = 10000;
		std::cout << std::endl << "PushBack (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				std::vector<Vec3> Vectors0;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Vectors0.push_back(Vec3(3.0, 5.0, -6.0));
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
				TArray<Vec3> Vectors1;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Vectors1.PushBack(Vec3(3.0, 5.0, -6.0));
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
		const Uint32 Iterations = 10000;
		std::cout << std::endl << "EmplaceBack (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Uint64 Total = 0;
			for (Uint32 I = 0; I < TestCount; I++)
			{
				std::vector<Vec3> Vectors0;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Vectors0.emplace_back(3.0, 5.0, -6.0);
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
				TArray<Vec3> Vectors1;
				auto t1 = std::chrono::high_resolution_clock::now();
				for (Uint32 J = 0; J < Iterations; J++)
				{
					Vectors1.EmplaceBack(3.0, 5.0, -6.0);
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
	UNREFERENCED_VARIABLE(Argc);
	UNREFERENCED_VARIABLE(Argv);

#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
	// TArray
#if 0
	std::cout << std::endl << "----------TArray----------" << std::endl << std::endl;
	{
		std::string ArgvStr = Argv[0];

		std::cout << "Testing TArray<std::string>" << std::endl;

		std::cout << std::endl << "Testing Constructors" << std::endl << std::endl;
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

			std::cout << "Before move" << std::endl << std::endl;
			PrintArr(Strings5);

			// Test move an array with data
			TArray<std::string> Strings6 = Move(Strings5);

			std::cout << "After move" << std::endl << std::endl;
			PrintArr(Strings5);
			PrintArr(Strings6);
		}

		// Assign
		std::cout << std::endl << "Testing Assign" << std::endl << std::endl;
		Strings0.Assign(7, "This is a teststring");
		PrintArr(Strings0);

		Strings1.Assign({ "Test-String #1", "Test-String #2", "Test-String #3" });
		PrintArr(Strings1);

		Strings2.Assign(Strings3.Begin(), Strings3.End());
		PrintArr(Strings2);

		// Resize
		std::cout << std::endl << "Testing Resize" << std::endl << std::endl;

		// Constructing a empty Array to test resize
		TArray<std::string> Strings4;

		std::cout << "Before Resize" << std::endl << std::endl;
		PrintArr(Strings4);
		PrintArr(Strings3);
		PrintArr(Strings1);

		Strings4.Resize(10, "New String");
		Strings3.Resize(0, "Hi, hi");
		Strings1.Resize(6, "Hello World");

		std::cout << "After Resize" << std::endl << std::endl;
		PrintArr(Strings4);
		PrintArr(Strings3);
		PrintArr(Strings1);

		std::cout << "Testing Shrinking Resize" << std::endl << std::endl;
		Strings4.Resize(2, "New String");
		PrintArr(Strings4);

		Strings4.Resize(15, "New String");
		PrintArr(Strings4);

		// Reserve
		std::cout << std::endl << "Testing Reserve" << std::endl << std::endl;

		std::cout << "Before Reserve" << std::endl << std::endl;
		PrintArr(Strings4);

		std::cout << "After Reserve" << std::endl << std::endl;
		Strings4.Reserve(Strings4.Capacity());
		PrintArr(Strings4);

		std::cout << "Shrinking" << std::endl << std::endl;
		Strings4.Reserve(5);
		PrintArr(Strings4);
		
		std::cout << "Growing" << std::endl << std::endl;
		Strings4.Reserve(10);
		PrintArr(Strings4);

		std::cout << "Resize" << std::endl << std::endl;
		Strings4.Resize(Strings4.Capacity() - 2, "This spot is reserved");
		PrintArr(Strings4);

		// Shrink To Fit
		std::cout << std::endl << "Testing ShrinkToFit" << std::endl << std::endl;

		std::cout << std::endl << "Before ShrinkToFit" << std::endl << std::endl;
		PrintArr(Strings4);

		Strings4.ShrinkToFit();

		std::cout << std::endl << "After ShrinkToFit" << std::endl << std::endl;
		PrintArr(Strings4);

		// Assignment
		std::cout << std::endl << "Testing Assignment" << std::endl << std::endl;

		Strings3.Resize(3, "No I am your father");

		std::cout << "Before Assignment" << std::endl << std::endl;
		PrintArr(Strings0);
		PrintArr(Strings1);
		PrintArr(Strings2);
		PrintArr(Strings3);
		PrintArr(Strings4);

		std::cout << "Strings0 = Strings4" << std::endl;
		Strings0 = Strings4;

		std::cout << "Strings1 = Move(Strings3)" << std::endl;
		Strings1 = Move(Strings3);

		std::cout << "Strings2 = InitializerList" << std::endl << std::endl;
		Strings2 =
		{
			"Another String in a InitializerList",
			"Strings are kinda cool",
			"Letters to fill up space in a string"
		};

		std::cout << "After Assignment" << std::endl << std::endl;
		PrintArr(Strings0);
		PrintArr(Strings1);
		PrintArr(Strings2);
		PrintArr(Strings3);
		PrintArr(Strings4);

		// PushBack
		std::cout << std::endl << "Testing PushBack" << std::endl << std::endl;
		for (Uint32 I = 0; I < 6; I++)
		{
			Strings2.PushBack("This is Pushed String #" + std::to_string(I));
		}
		PrintArr(Strings2);

		std::cout << std::endl << "Testing PushBack" << std::endl << std::endl;
		for (Uint32 I = 0; I < 6; I++)
		{
			Strings2.PushBack(ArgvStr);
		}
		PrintArr(Strings2);

		// EmplaceBack
		std::cout << std::endl << "Testing EmplaceBack" << std::endl << std::endl;
		for (Uint32 I = 0; I < 6; I++)
		{
			Strings2.EmplaceBack("This is an Emplaced String #" + std::to_string(I));
		}
		PrintArr(Strings2);

		// PopBack
		std::cout << std::endl << "Testing PopBack" << std::endl << std::endl;
		for (Uint32 I = 0; I < 3; I++)
		{
			Strings2.PopBack();
		}
		PrintArr(Strings2);

		// Insert
		std::cout << std::endl << "Testing Insert" << std::endl << std::endl;
		PrintArr(Strings2);
		
		std::cout << "At front" << std::endl << std::endl;
		Strings2.Insert(Strings2.Begin(), ArgvStr);
		PrintArr(Strings2);
		Strings2.Insert(Strings2.Begin(), "Inserted String");
		PrintArr(Strings2);
		Strings2.Insert(Strings2.Begin(), { "Inserted String #1", "Inserted String #2" });
		PrintArr(Strings2);

		std::cout << "At Arbitrary" << std::endl << std::endl;
		Strings2.Insert(Strings2.Begin() + 2, ArgvStr);
		PrintArr(Strings2);
		Strings2.Insert(Strings2.Begin() + 2, "Inserted String Again");
		PrintArr(Strings2);
		Strings2.Insert(Strings2.Begin() + 2, { "Inserted String Again #1", "Inserted String Again #2" });
		PrintArr(Strings2);

		std::cout << "At End" << std::endl << std::endl;
		Strings2.Insert(Strings2.End(), { "Inserted String At End #1", "Inserted String At End #2" });
		PrintArr(Strings2);

		std::cout << "At front after reallocation" << std::endl << std::endl;
		// Add a shrink to fit to force reallocation
		Strings2.ShrinkToFit();
		Strings2.Insert(Strings2.Begin(), ArgvStr);
		PrintArr(Strings2);
		// Add a shrink to fit to force reallocation
		Strings2.ShrinkToFit();
		Strings2.Insert(Strings2.Begin(), "Inserted String Reallocated");
		PrintArr(Strings2);
		// Add a shrink to fit to force reallocation
		Strings2.ShrinkToFit();
		Strings2.Insert(Strings2.Begin(), { "Inserted String Reallocated #1", "Inserted String Reallocated #2" });
		PrintArr(Strings2);

		std::cout << "At Arbitrary after reallocation" << std::endl << std::endl;
		// Add a shrink to fit to force reallocation
		Strings2.ShrinkToFit();
		Strings2.Insert(Strings2.Begin() + 2, ArgvStr);
		PrintArr(Strings2);
		// Add a shrink to fit to force reallocation
		Strings2.ShrinkToFit();
		Strings2.Insert(Strings2.Begin() + 2, "Inserted String Again Reallocated");
		PrintArr(Strings2);
		// Add a shrink to fit to force reallocation
		Strings2.ShrinkToFit();
		Strings2.Insert(Strings2.Begin() + 2, { "Inserted String Again Reallocated #1", "Inserted String Again Reallocated #2" });
		PrintArr(Strings2);

		std::cout << "At End after reallocation" << std::endl << std::endl;
		// Add a shrink to fit to force reallocation
		Strings2.ShrinkToFit();
		Strings2.Insert(Strings2.End(), { "Inserted String At End Reallocated #1", "Inserted String At End Reallocated #2" });
		PrintArr(Strings2);

		// Erase
		std::cout << std::endl << "Testing Erase" << std::endl << std::endl;
		PrintArr(Strings2);

		std::cout << "At front" << std::endl << std::endl;
		Strings2.Erase(Strings2.Begin());
		PrintArr(Strings2);

		std::cout << "At Arbitrary" << std::endl << std::endl;
		Strings2.Erase(Strings2.Begin() + 2);
		PrintArr(Strings2);

		std::cout << "Range At front" << std::endl << std::endl;
		Strings2.Erase(Strings2.Begin(), Strings2.Begin() + 2);
		PrintArr(Strings2);

		std::cout << "Range At Arbitrary" << std::endl << std::endl;
		Strings2.Erase(Strings2.Begin() + 4, Strings2.Begin() + 7);
		PrintArr(Strings2);

		std::cout << "Range At End" << std::endl << std::endl;
		Strings2.Erase(Strings2.End() - 3, Strings2.End());
		PrintArr(Strings2);

		// Swap
		std::cout << std::endl << "Testing Swap" << std::endl << std::endl;
		std::cout << "Before" << std::endl << std::endl;
		PrintArr(Strings0);
		PrintArr(Strings2);

		Strings0.Swap(Strings2);

		std::cout << "After" << std::endl << std::endl;
		PrintArr(Strings0);
		PrintArr(Strings2);

		// Iterators
		std::cout << std::endl << "Testing Iterators" << std::endl;
		
		std::cout << std::endl << "Iterators" << std::endl << std::endl;
		for (auto It = Strings2.begin(); It != Strings2.end(); It++)
		{
			std::cout << (*It) << std::endl;
		}
		
		std::cout << std::endl;
		
		for (std::string& Str : Strings2)
		{
			std::cout << Str << std::endl;
		}

		std::cout << std::endl << "Reverse Iterators" << std::endl << std::endl;
		for (auto It = Strings2.rbegin(); It != Strings2.rend(); It++)
		{
			std::cout << (*It) << std::endl;
		}
	}
#endif

	// TArray<vec3>
#if 0
	{
		std::cout << "Testing TArray<Vec3>" << std::endl;
		std::cout << std::endl << "Testing Constructors" << std::endl;
		// Test constructors
		TArray<Vec3> Vectors0;
		TArray<Vec3> Vectors1(5, Vec3(1.0, 1.0, 1.0));
		TArray<Vec3> Vectors2(Vectors1.begin(), Vectors1.end());
		TArray<Vec3> Vectors3 =
		{
			Vec3(1.0, 1.0, 1.0),
			Vec3(2.0, 2.0, 2.0),
			Vec3(3.0, 3.0, 3.0)
		};

		PrintArr(Vectors0);
		PrintArr(Vectors1);
		PrintArr(Vectors2);
		PrintArr(Vectors3);

		{
			// Test copy an empty array
			TArray<Vec3> Vectors4 = Vectors0;
			// Test copy an array with data
			TArray<Vec3> Vectors5 = Vectors1;

			PrintArr(Vectors4);

			std::cout << "Before move" << std::endl << std::endl;
			PrintArr(Vectors5);

			// Test move an array with data
			TArray<Vec3> Vectors6 = Move(Vectors5);

			std::cout << "After move" << std::endl << std::endl;
			PrintArr(Vectors5);
			PrintArr(Vectors6);
		}

		// Assign
		std::cout << std::endl << "Testing Assign" << std::endl;
		Vectors0.Assign(7, Vec3(5.0, 5.0, 5.0));
		PrintArr(Vectors0);

		Vectors1.Assign({ Vec3(1.0, 5.0, 5.0), Vec3(2.0, 5.0, 5.0), Vec3(3.0, 5.0, 5.0) });
		PrintArr(Vectors1);

		Vectors2.Assign(Vectors3.begin(), Vectors3.end());
		PrintArr(Vectors2);

		// Resize
		std::cout << std::endl << "Testing Resize" << std::endl << std::endl;

		// Constructing a empty Array to test resize
		TArray<Vec3> Vectors4;

		std::cout << "Before Resize" << std::endl << std::endl;
		PrintArr(Vectors4);
		PrintArr(Vectors3);
		PrintArr(Vectors1);

		Vectors4.Resize(10, Vec3(-10.0, -10.0, -10.0));
		Vectors3.Resize(0);
		Vectors1.Resize(6, Vec3(-5.0, 10.0, -15.0));

		std::cout << "After Resize" << std::endl << std::endl;
		PrintArr(Vectors4);
		PrintArr(Vectors3);
		PrintArr(Vectors1);

		std::cout << "Testing Shrinking Resize" << std::endl << std::endl;
		Vectors4.Resize(2, Vec3(-15.0, -15.0, -15.0));
		PrintArr(Vectors4);

		Vectors4.Resize(15, Vec3(23.0, 23.0, 23.0));
		PrintArr(Vectors4);

		// Reserve
		std::cout << std::endl << "Testing Reserve" << std::endl << std::endl;

		std::cout << "Before Reserve" << std::endl << std::endl;
		PrintArr(Vectors4);

		std::cout << "After Reserve" << std::endl << std::endl;
		Vectors4.Reserve(Vectors4.Capacity());
		PrintArr(Vectors4);

		std::cout << "Shrinking" << std::endl << std::endl;
		Vectors4.Reserve(5);
		PrintArr(Vectors4);

		std::cout << "Growing" << std::endl << std::endl;
		Vectors4.Reserve(10);
		PrintArr(Vectors4);

		std::cout << "Resize" << std::endl << std::endl;
		Vectors4.Resize(Vectors4.Capacity() - 2, Vec3(-1.0f, -1.0f, -1.0f));
		PrintArr(Vectors4);

		// Shrink To Fit
		std::cout << std::endl << "Testing ShrinkToFit" << std::endl;

		std::cout << std::endl << "Before ShrinkToFit" << std::endl << std::endl;
		PrintArr(Vectors4);

		Vectors4.ShrinkToFit();

		std::cout << std::endl << "After ShrinkToFit" << std::endl << std::endl;
		PrintArr(Vectors4);

		// Assignment
		std::cout << std::endl << "Testing Assignment" << std::endl << std::endl;

		Vectors3.Resize(3, Vec3(42.0, 42.0, 42.0));

		std::cout << "Before Assignment" << std::endl << std::endl;
		PrintArr(Vectors0);
		PrintArr(Vectors1);
		PrintArr(Vectors2);
		PrintArr(Vectors3);
		PrintArr(Vectors4);

		std::cout << "Vectors0 = Vectors4" << std::endl;
		Vectors0 = Vectors4;

		std::cout << "Vectors1 = Move(Vectors3)" << std::endl;
		Vectors1 = Move(Vectors3);

		std::cout << "Vectors2 = InitializerList" << std::endl << std::endl;
		Vectors2 =
		{
			Vec3(9.0, 9.0, 9.0),
			Vec3(10.0, 10.0, 10.0),
			Vec3(11.0, 11.0, 11.0)
		};

		std::cout << "After Assignment" << std::endl << std::endl;
		PrintArr(Vectors0);
		PrintArr(Vectors1);
		PrintArr(Vectors2);
		PrintArr(Vectors3);
		PrintArr(Vectors4);

		// PushBack
		std::cout << std::endl << "Testing PushBack" << std::endl << std::endl;
		for (Uint32 I = 0; I < 6; I++)
		{
			Vectors2.PushBack(Vec3(7.0, 7.0, 7.0));
		}
		PrintArr(Vectors2);

		Vec3 Vector(5.0f, -45.0f, 5.0f);
		std::cout << std::endl << "Testing PushBack" << std::endl << std::endl;
		for (Uint32 I = 0; I < 6; I++)
		{
			Vectors2.PushBack(Vector);
		}
		PrintArr(Vectors2);

		// EmplaceBack
		std::cout << std::endl << "Testing EmplaceBack" << std::endl << std::endl;
		for (Uint32 I = 0; I < 6; I++)
		{
			Vectors2.EmplaceBack(1.0, 0.0, 1.0);
		}
		PrintArr(Vectors2);

		// PopBack
		std::cout << std::endl << "Testing PopBack" << std::endl << std::endl;
		for (Uint32 I = 0; I < 3; I++)
		{
			Vectors2.PopBack();
		}
		PrintArr(Vectors2);

		// Insert
		std::cout << std::endl << "Testing Insert" << std::endl << std::endl;
		std::cout << "At front" << std::endl << std::endl;
		Vectors2.Insert(Vectors2.Begin(), Vector);
		Vectors2.Insert(Vectors2.Begin(), Vec3(-1.0, -1.0, -1.0));
		Vectors2.Insert(Vectors2.Begin(), { Vec3(1.0f, 1.0f, 1.0f), Vec3(2.0f, 2.0f, 2.0f) });
		PrintArr(Vectors2);

		std::cout << "At Arbitrary" << std::endl << std::endl;
		Vectors2.Insert(Vectors2.Begin() + 2, Vector);
		Vectors2.Insert(Vectors2.Begin() + 2, Vec3(-1.0, -1.0, -2.0));
		Vectors2.Insert(Vectors2.Begin() + 2, { Vec3(1.0f, 1.0f, 2.0f), Vec3(2.0f, 2.0f, 3.0f) });
		PrintArr(Vectors2);

		std::cout << "At End" << std::endl << std::endl;
		Vectors2.Insert(Vectors2.End(), { Vec3(1.0f, 1.0f, 3.0f), Vec3(2.0f, 2.0f, 4.0f) });
		PrintArr(Vectors2);

		std::cout << "At front after reallocation" << std::endl << std::endl;
		// Add a shrink to fit to force reallocation
		Vectors2.ShrinkToFit();
		Vectors2.Insert(Vectors2.Begin(), Vector);
		// Add a shrink to fit to force reallocation
		Vectors2.ShrinkToFit();
		Vectors2.Insert(Vectors2.Begin(), Vec3(-1.0, -1.0, -3.0));
		// Add a shrink to fit to force reallocation
		Vectors2.ShrinkToFit();
		Vectors2.Insert(Vectors2.Begin(), { Vec3(1.0f, 1.0f, 4.0f), Vec3(2.0f, 2.0f, 5.0f) });
		PrintArr(Vectors2);

		std::cout << "At Arbitrary after reallocation" << std::endl << std::endl;
		// Add a shrink to fit to force reallocation
		Vectors2.ShrinkToFit();
		Vectors2.Insert(Vectors2.Begin() + 2, Vector);
		// Add a shrink to fit to force reallocation
		Vectors2.ShrinkToFit();
		Vectors2.Insert(Vectors2.Begin() + 2, Vec3(-1.0, -1.0, -4.0));
		// Add a shrink to fit to force reallocation
		Vectors2.ShrinkToFit();
		Vectors2.Insert(Vectors2.Begin() + 2, { Vec3(1.0f, 1.0f, 5.0f), Vec3(2.0f, 2.0f, 6.0f) });
		PrintArr(Vectors2);

		std::cout << "At End after reallocation" << std::endl << std::endl;
		// Add a shrink to fit to force reallocation
		Vectors2.ShrinkToFit();
		Vectors2.Insert(Vectors2.End(), { Vec3(6.0f, 6.0f, 6.0f), Vec3(2.0f, 2.0f, 7.0f) });
		PrintArr(Vectors2);

		// Erase
		std::cout << std::endl << "Testing Erase" << std::endl << std::endl;
		PrintArr(Vectors2);

		std::cout << "At front" << std::endl << std::endl;
		Vectors2.Erase(Vectors2.Begin());
		PrintArr(Vectors2);

		std::cout << "At Arbitrary" << std::endl << std::endl;
		Vectors2.Erase(Vectors2.Begin() + 2);
		PrintArr(Vectors2);

		std::cout << "Range At front" << std::endl << std::endl;
		Vectors2.Erase(Vectors2.Begin(), Vectors2.Begin() + 2);
		PrintArr(Vectors2);

		std::cout << "Range At Arbitrary" << std::endl << std::endl;
		Vectors2.Erase(Vectors2.Begin() + 4, Vectors2.Begin() + 7);
		PrintArr(Vectors2);

		std::cout << "Range At End" << std::endl << std::endl;
		Vectors2.Erase(Vectors2.End() - 3, Vectors2.End());
		PrintArr(Vectors2);

		// Swap
		std::cout << std::endl << "Testing Swap" << std::endl << std::endl;
		std::cout << "Before" << std::endl << std::endl;
		PrintArr(Vectors0);
		PrintArr(Vectors2);

		Vectors0.Swap(Vectors2);

		std::cout << "After" << std::endl << std::endl;
		PrintArr(Vectors0);
		PrintArr(Vectors2);
	}
#endif

	// Smart Pointers
#if 1
	// TSharedPtr
	std::cout << std::endl << "----------TSharedPtr----------" << std::endl << std::endl;

	struct Base
	{
		Uint32 X;
	};

	struct Derived : public Base
	{
		Uint32 Y;
	};

	Uint32* Ptr0 = new Uint32(9);
	Uint32* Ptr1 = new Uint32(10);

	// Test nullptr
	TSharedPtr<Uint32> Null;
	Null = Ptr0; // Takes ownership of Ptr0

	TSharedPtr<Uint32> UintPtr0 = MakeShared<Uint32>(5);
	TSharedPtr<Uint32> UintPtr1 = Null;
	TSharedPtr<Uint32> UintPtr2 = TSharedPtr<Uint32>(Ptr1); // Takes ownership of Ptr1

	TSharedPtr<Derived> DerivedPtr0 = MakeShared<Derived>();
	TSharedPtr<Base> BasePtr0 = DerivedPtr0;

	TWeakPtr<Base> WeakBase0 = BasePtr0;
	TWeakPtr<Derived> WeakBase1 = DerivedPtr0;

	std::cout << "Testing Array types" << std::endl;
	TSharedPtr<Uint32[]> UintArr0 = MakeShared<Uint32[]>(5);
	TWeakPtr<Uint32[]> WeakArr = UintArr0;
	TSharedPtr<Uint32[]> UintArr1 = WeakArr.MakeShared();

	TUniquePtr<Uint32> UniqueInt = MakeUnique<Uint32>(5);
	TSharedPtr<Uint32> UintPtr3 = TSharedPtr<Uint32>(Move(UniqueInt));

	TUniquePtr<Uint32[]> UniqueUintArr = MakeUnique<Uint32[]>(5);
	
	std::cout << "Testing Index operator" << std::endl;
	WeakArr[0] = 5;
	WeakArr[1] = 6;
	std::cout << WeakArr[0] << std::endl;
	std::cout << WeakArr[1] << std::endl;

	std::cout << "Testing bool operators" << std::endl;
	std::cout << std::boolalpha << (WeakBase0 == WeakBase1) << std::endl;
	std::cout << std::boolalpha << (UintPtr0 == UintPtr1) << std::endl;
#endif
	
	// TFunction
#if 0
	std::cout << std::endl << "----------TFunction----------" << std::endl << std::endl;
	std::cout << "Testing constructors" << std::endl;
	
	struct Functor
	{
		bool operator()(Int32 In)
		{
			std::cout << "Functor " << In << std::endl;
			return true;
		}
	} Fun;
	
	TFunction<bool(Int32)> NormalFunc = Func;
	NormalFunc(5);
	std::cout << "Sizeof NormalFunc " << sizeof(NormalFunc) << "Bytes" << std::endl;
	
	A a;
	TFunction<bool(Int32)> MemberFunc = TFunction<bool(Int32)>(&a, &A::Func);
	MemberFunc(10);
	std::cout << "Sizeof MemberFunc " << sizeof(MemberFunc) << "Bytes" <<std::endl;
	
	TFunction<bool(Int32)> FunctorFunc = Fun;
	FunctorFunc(15);
	std::cout << "Sizeof FunctorFunc " << sizeof(FunctorFunc) << "Bytes" << std::endl;
	
	TFunction<bool(Int32)> LambdaFunc = [](Int32 Input) -> bool
	{
		std::cout << "Lambda " << Input << std::endl;
		return true;
	};
	LambdaFunc(20);
	std::cout << "Sizeof LambdaFunc " << sizeof(LambdaFunc) << "Bytes" << std::endl;
	
	std::cout << "Test copy constructor" << std::endl;
	
	TFunction<bool(Int32)> CopyFunc(MemberFunc);
	CopyFunc(30);
	MemberFunc(40);

	std::cout << "Test move constructor" << std::endl;
	TFunction<bool(Int32)> MoveFunc(::Move(LambdaFunc));
	MoveFunc(50);
	if (LambdaFunc)
	{
		LambdaFunc(60);
	}
	
	std::cout << "Testing Assign" << std::endl;
	NormalFunc.Assign(Func2);
	MemberFunc.Assign(&a, &A::Func2);
	
	NormalFunc(70);
	MemberFunc(80);
	
	std::cout << "Testing Swap" << std::endl;
	NormalFunc.Swap(MemberFunc);
	
	NormalFunc(90);
	MemberFunc(100);
#endif

	// String
#if 0
	String Str0;
	String Str1 = "Test String";
	
	
#endif
	
#if 0
	//Performance
	BenchMark();
#endif

	return 0;
}
