#include "TArray_Test.h"

#include "../Containers/TArray.h"

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

/*
* A very OO clock 
*/

struct Clock
{
	friend struct ScopedClock;

public:
	Clock()
		: Duration(0)
		, TotalDuration(0)
	{
	}

	inline void Reset()
	{
		Duration = 0;
		TotalDuration = 0;
	}

	inline Int64 GetLastDuration() const
	{
		return Duration;
	}

	inline Int64 GetTotalDuration() const
	{
		return TotalDuration;
	}

private:
	inline void AddDuration(Int64 InDuration)
	{
		Duration = InDuration;
		TotalDuration += Duration;
	}

	Int64 Duration		= 0;
	Int64 TotalDuration	= 0;
};

struct ScopedClock
{
	ScopedClock(Clock& InParent)
		: Parent(InParent)
		, t0(std::chrono::high_resolution_clock::now())
		, t1()
	{
		t0 = std::chrono::high_resolution_clock::now();
	}

	~ScopedClock()
	{
		t1 = std::chrono::high_resolution_clock::now();
		Parent.AddDuration(std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0).count());
	}

	Clock& Parent;
	std::chrono::steady_clock::time_point t0;
	std::chrono::steady_clock::time_point t1;
};

/*
 * Vec3
 */

struct Vec3
{
	Vec3()
		: x(0.0)
		, y(0.0)
		, z(0.0)
	{
	}

	Vec3(Double InX, Double InY, Double InZ)
		: x(InX)
		, y(InY)
		, z(InZ)
	{
	}

	Double x;
	Double y;
	Double z;

	operator std::string() const
	{
		return std::to_string(x) + ", " + std::to_string(y) + ", " + std::to_string(z);
	}
};

/*
 * PrintArr
 */

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
#define PrintArr(Arr) PrintArr(Arr, #Arr)

/*
 * Benchmark
 */

void TArray_Benchmark()
{
	// Performance
	std::cout << std::endl << "Benchmark (std::string)" << std::endl;
	const UInt32 TestCount = 100;

	// Insert
#if 1
	{
		const UInt32 Iterations = 10000;
		std::cout << std::endl << "Insert (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				std::vector<std::string> Strings0;
				
				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Strings0.insert(Strings0.begin(), "My name is jeff");
				}
			}

			std::cout << "std::vector:" << Clock.GetTotalDuration() / TestCount << "ns" << std::endl;
		}

		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				TArray<std::string> Strings1;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Strings1.Insert(Strings1.Begin(), "My name is jeff");
				}
			}

			std::cout << "TArray     :" << Clock.GetTotalDuration() / TestCount << "ns" << std::endl;
		}
	}
#endif

#if 1
	// Emplace
	{
		const UInt32 Iterations = 10000;
		std::cout << std::endl << "Emplace (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				std::vector<std::string> Strings0;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Strings0.emplace(Strings0.begin(), "My name is jeff");
				}
			}

			std::cout << "std::vector:" << Clock.GetTotalDuration() / TestCount << "ns" << std::endl;
		}

		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				TArray<std::string> Strings1;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Strings1.Emplace(Strings1.begin(), "My name is jeff");
				}
			}
			std::cout << "TArray     :" << Clock.GetTotalDuration() / Iterations << "ns" << std::endl;
		}
	}
#endif

#if 1
	// PushBack
	{
		const UInt32 Iterations = 10000;
		std::cout << std::endl << "PushBack (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				std::vector<std::string> Strings0;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Strings0.push_back("My name is jeff");
				}
			}

			std::cout << "std::vector:" << Clock.GetTotalDuration() / TestCount << "ns" << std::endl;
		}

		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				TArray<std::string> Strings1;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Strings1.PushBack("My name is jeff");
				}
			}
			std::cout << "TArray     :" << Clock.GetTotalDuration() / Iterations << "ns" << std::endl;
		}
	}
#endif

#if 1
	// EmplaceBack
	{
		const UInt32 Iterations = 10000;
		std::cout << std::endl << "EmplaceBack (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				std::vector<std::string> Strings0;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Strings0.emplace_back("My name is jeff");
				}
			}

			std::cout << "std::vector:" << Clock.GetTotalDuration() / TestCount << "ns" << std::endl;
		}

		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				TArray<std::string> Strings1;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Strings1.EmplaceBack("My name is jeff");
				}
			}
			std::cout << "TArray     :" << Clock.GetTotalDuration() / Iterations << "ns" << std::endl;
		}
	}
#endif

	std::cout << std::endl << "Benchmark (Vec3)" << std::endl;

	// Insert
#if 1
	{
		const UInt32 Iterations = 10000;
		std::cout << std::endl << "Insert (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				std::vector<Vec3> Vectors0;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Vectors0.insert(Vectors0.begin(), Vec3(3.0, 5.0, -6.0));
				}
			}

			std::cout << "std::vector:" << Clock.GetTotalDuration() / TestCount << "ns" << std::endl;
		}

		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				TArray<Vec3> Vectors1;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Vectors1.Insert(Vectors1.Begin(), Vec3(3.0, 5.0, -6.0));
				}
			}
			std::cout << "TArray     :" << Clock.GetTotalDuration() / Iterations << "ns" << std::endl;
		}
	}
#endif

#if 1
	// Emplace
	{
		const UInt32 Iterations = 10000;
		std::cout << std::endl << "Emplace (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				std::vector<Vec3> Vectors0;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Vectors0.emplace(Vectors0.begin(), 3.0, 5.0, -6.0);
				}
			}

			std::cout << "std::vector:" << Clock.GetTotalDuration() / TestCount << "ns" << std::endl;
		}

		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				TArray<Vec3> Vectors1;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Vectors1.Emplace(Vectors1.Begin(), double(j + 1), 5.0, -6.0);
				}
			}
			std::cout << "TArray     :" << Clock.GetTotalDuration() / Iterations << "ns" << std::endl;
		}
	}
#endif

#if 1
	// PushBack
	{
		const UInt32 Iterations = 10000;
		std::cout << std::endl << "PushBack (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				std::vector<Vec3> Vectors0;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Vectors0.push_back(Vec3(3.0, 5.0, -6.0));
				}
			}

			std::cout << "std::vector:" << Clock.GetTotalDuration() / TestCount << "ns" << std::endl;
		}

		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				TArray<Vec3> Vectors1;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Vectors1.PushBack(Vec3(3.0, 5.0, -6.0));
				}
			}
			std::cout << "TArray     :" << Clock.GetTotalDuration() / Iterations << "ns" << std::endl;
		}
	}
#endif

#if 1
	// EmplaceBack
	{
		const UInt32 Iterations = 10000;
		std::cout << std::endl << "EmplaceBack (Iterations=" << Iterations << ", TestCount=" << TestCount << ")" << std::endl;
		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				std::vector<Vec3> Vectors0;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Vectors0.emplace_back(3.0, 5.0, -6.0);
				}
			}

			std::cout << "std::vector:" << Clock.GetTotalDuration() / TestCount << "ns" << std::endl;
		}

		{
			Clock Clock;
			for (UInt32 i = 0; i < TestCount; i++)
			{
				TArray<Vec3> Vectors1;

				ScopedClock ScopedClock(Clock);
				for (UInt32 j = 0; j < Iterations; j++)
				{
					Vectors1.EmplaceBack(3.0, 5.0, -6.0);
				}
			}

			std::cout << "TArray     :" << Clock.GetTotalDuration() / Iterations << "ns" << std::endl;
		}
	}
#endif
}

/*
 * Test
 */

void TArray_Test(Int32 Argc, const Char** Argv)
{
#if 1
	std::cout << std::endl << "----------TArray----------" << std::endl << std::endl;
	{
		std::string ArgvStr = Argc > 0 ? Argv[0] : "Filepath";

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
		for (UInt32 I = 0; I < 6; I++)
		{
			Strings2.PushBack("This is Pushed String #" + std::to_string(I));
		}
		PrintArr(Strings2);

		std::cout << std::endl << "Testing PushBack" << std::endl << std::endl;
		for (UInt32 I = 0; I < 6; I++)
		{
			Strings2.PushBack(ArgvStr);
		}
		PrintArr(Strings2);

		// EmplaceBack
		std::cout << std::endl << "Testing EmplaceBack" << std::endl << std::endl;
		for (UInt32 I = 0; I < 6; I++)
		{
			Strings2.EmplaceBack("This is an Emplaced String #" + std::to_string(I));
		}
		PrintArr(Strings2);

		// PopBack
		std::cout << std::endl << "Testing PopBack" << std::endl << std::endl;
		for (UInt32 I = 0; I < 3; I++)
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
#if 1
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
		for (UInt32 I = 0; I < 6; I++)
		{
			Vectors2.PushBack(Vec3(7.0, 7.0, 7.0));
		}
		PrintArr(Vectors2);

		Vec3 Vector(5.0f, -45.0f, 5.0f);
		std::cout << std::endl << "Testing PushBack" << std::endl << std::endl;
		for (UInt32 I = 0; I < 6; I++)
		{
			Vectors2.PushBack(Vector);
		}
		PrintArr(Vectors2);

		// EmplaceBack
		std::cout << std::endl << "Testing EmplaceBack" << std::endl << std::endl;
		for (UInt32 I = 0; I < 6; I++)
		{
			Vectors2.EmplaceBack(1.0, 0.0, 1.0);
		}
		PrintArr(Vectors2);

		// PopBack
		std::cout << std::endl << "Testing PopBack" << std::endl << std::endl;
		for (UInt32 I = 0; I < 3; I++)
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
}
