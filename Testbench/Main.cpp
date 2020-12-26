#include <iostream>
#include <cassert>
#include <utility>
#include <vector>
#include <chrono>
#include <string>
#include <memory>
#include <type_traits>

#ifdef _WIN32
	#include <crtdbg.h>
#endif

// Tests
#include "TArray_Test.h"
#include "TSharedPtr_Test.h"
#include "TFunction_Test.h"
#include "TStaticArray_Test.h"

void BenchMark()
{
#if 1
	TArray_Benchmark();
#endif
}

int main(int Argc, const char* Argv[])
{
#ifdef _WIN32
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

#if 1
	TArray_Test(Argc, Argv);
#endif
	
	// Smart Pointers
#if 1
	TSharedPtr_Test();
#endif
	
	// TFunction
#if 1
	TFunction_Test();
#endif
	
	// TStaticArray
#if 1
	TStaticArray_Test();
#endif
	
#if 0
	//Performance
	BenchMark();
#endif

	return 0;
}
