#include "TFunction_Test.h"

#include "../Containers/TFunction.h"

#include <iostream>

/*
 * A
 */

struct A
{
	bool Func(Int32 In)
	{
		std::cout << "MemberCall " << In << std::endl;
		return true;
	}
	
	bool ConstFunc(Int32 In) const
	{
		std::cout << "Const MemberCall " << In << std::endl;
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

/*
 * Test
 */

void TFunction_Test()
{
	std::cout << std::endl << "-------TMemberFunction-------" << std::endl << std::endl;
	std::cout << "Testing constructor and Invoke" << std::endl;
	
	A a;
	TMemberFunction A_Func = TMemberFunction<A, bool(Int32)>(&a, &A::Func);
	A_Func(32);
	
	TConstMemberFunction A_ConstFunc = TConstMemberFunction<A, bool(Int32)>(&a, &A::ConstFunc);
	A_ConstFunc(32);
	
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
	
	TFunction<bool(Int32)> MemberFunc = BindMemberFunction(&a, &A::Func);
	MemberFunc(10);
	
	TFunction<bool(Int32)> FunctorFunc = Fun;
	FunctorFunc(15);
	
	TFunction<bool(Int32)> LambdaFunc = [](Int32 Input) -> bool
	{
		std::cout << "Lambda " << Input << std::endl;
		return true;
	};
	LambdaFunc(20);
	
	A a1;
	TFunction<bool(Int32)> LambdaMemberFunc = [&](Int32 Input) -> bool
	{
		std::cout << "--Lambda Begin--" << std::endl;
		a1.Func(Input);
		a1.Func2(Input);
		std::cout << "--Lambda End--";
		return true;
	};
	LambdaMemberFunc(20);
	
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
	MemberFunc.Assign(BindMemberFunction(&a, &A::Func2));

	NormalFunc(70);
	MemberFunc(80);

	std::cout << "Testing Swap" << std::endl;
	NormalFunc.Swap(MemberFunc);

	NormalFunc(90);
	MemberFunc(100);
}
