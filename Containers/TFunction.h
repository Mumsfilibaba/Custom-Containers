#pragma once
#include "TUtilities.h"

/*
 * TMemberFunction - Encapsulates a member function
 */

template<typename T, typename TInvokable>
class TMemberFunction;

template<typename T, typename TReturn, typename... TArgs>
class TMemberFunction<T, TReturn(TArgs...)>
{
public:
	typedef TReturn(T::* TFunctionType)(TArgs...);
	
	FORCEINLINE TMemberFunction(T* InThis, TFunctionType InFunc) noexcept
		: This(InThis)
		, Func(InFunc)
	{
	}
	
	FORCEINLINE TReturn Invoke(TArgs&&... Args) noexcept
	{
		return ((*This).*Func)(::Forward<TArgs>(Args)...);
	}
	
	FORCEINLINE TReturn operator()(TArgs&&... Args) noexcept
	{
		return Invoke(::Forward<TArgs>(Args)...);
	}
	
private:
	T* This;
	TFunctionType Func;
};

/*
 * TConstMemberFunction - Encapsulates a const member function
 */

template<typename T, typename TInvokable>
class TConstMemberFunction;

template<typename T, typename TReturn, typename... TArgs>
class TConstMemberFunction<T, TReturn(TArgs...)>
{
public:
	typedef TReturn(T::* TFunctionType)(TArgs...) const;
	
	FORCEINLINE TConstMemberFunction(const T* InThis, TFunctionType InFunc) noexcept
		: This(InThis)
		, Func(InFunc)
	{
	}
	
	FORCEINLINE TReturn Invoke(TArgs&&... Args) noexcept
	{
		return ((*This).*Func)(::Forward<TArgs>(Args)...);
	}
	
	FORCEINLINE TReturn operator()(TArgs&&... Args) noexcept
	{
		return Invoke(::Forward<TArgs>(Args)...);
	}
	
private:
	const T* This;
	TFunctionType Func;
};

/*
 * BindFunction
 */

template<typename T, typename TReturn, typename... TArgs>
inline TMemberFunction<T, TReturn(TArgs...)> BindFunction(T* This, TReturn (T::*Func)(TArgs...)) noexcept
{
	return ::Move(TMemberFunction<T, TReturn(TArgs...)>(This, Func));
}

template<typename T, typename TReturn, typename... TArgs>
inline TConstMemberFunction<T, TReturn(TArgs...)> BindFunction(const T* This, TReturn (T::*Func)(TArgs...) const) noexcept
{
	return ::Move(TConstMemberFunction<T, TReturn(TArgs...)>(This, Func));
}

/*
 * TFunction - Encapsulates callables
 */

template<typename TInvokable>
class TFunction;

template<typename TReturn, typename... TArgs>
class TFunction<TReturn(TArgs...)>
{
private:
	/*
	 * IFunction
	 */
	
	class IFunctor
	{
	public:
		virtual ~IFunctor() = default;

		virtual TReturn Invoke(TArgs&&... Args) noexcept = 0;

		virtual IFunctor* Clone(Void* Memory) noexcept	= 0;
		virtual IFunctor* Move(Void* Memory) noexcept	= 0;
	};

	/*
	 * TGenericFunctor
	 */
	
	template<typename F>
	class TGenericFunctor : public IFunctor
	{
	public:
		FORCEINLINE TGenericFunctor(const F& InFunctor) noexcept
			: IFunctor()
			, Functor(InFunctor)
		{
		}

		FORCEINLINE TGenericFunctor(const TGenericFunctor& Other) noexcept
			: IFunctor()
			, Functor(Other.Functor)
		{
		}

		FORCEINLINE TGenericFunctor(TGenericFunctor&& Other) noexcept
			: IFunctor()
			, Functor(::Move(Other.Functor))
		{
			if constexpr (std::is_pointer<F>())
			{
				Other.Functor = nullptr;
			}
		}

		inline virtual TReturn Invoke(TArgs&&... Args) noexcept override final
		{
			return Functor(Forward<TArgs>(Args)...);
		}

		inline virtual IFunctor* Clone(Void* Memory) noexcept override final
		{
			return new(Memory) TGenericFunctor(*this);
		}

		inline virtual IFunctor* Move(Void* Memory) noexcept override final
		{
			return new(Memory) TGenericFunctor(::Move(*this));
		}

	private:
		F Functor;
	};

public:
	FORCEINLINE TFunction() noexcept
		: Func(nullptr)
		, StackBuffer()
		, StackAllocated(true)
	{
	}

	FORCEINLINE TFunction(std::nullptr_t) noexcept
		: Func(nullptr)
		, StackBuffer()
		, StackAllocated(true)
	{
	}

	template<typename F>
	FORCEINLINE TFunction(F Functor) noexcept
		: Func(nullptr)
		, StackBuffer()
		, StackAllocated(true)
	{
		Construct(::Forward<F>(Functor));
	}

	FORCEINLINE TFunction(const TFunction& Other) noexcept
		: Func(nullptr)
		, StackBuffer()
		, StackAllocated(true)
	{
		CopyConstruct(Other);
	}

	FORCEINLINE TFunction(TFunction&& Other) noexcept
		: Func(nullptr)
		, StackBuffer()
		, StackAllocated(true)
	{
		MoveConstruct(::Forward<TFunction>(Other));
	}

	FORCEINLINE ~TFunction()
	{
		Release();
	}

	FORCEINLINE void Swap(TFunction& Other) noexcept
	{
		TFunction TempFunc(*this);
		*this = Other;
		Other = TempFunc;
	}

	template<typename F>
	FORCEINLINE void Assign(F&& Functor) noexcept
	{
		Release();
		Construct(::Forward<F>(Functor));
	}
	
	FORCEINLINE TReturn Invoke(TArgs&&... Args) noexcept
	{
		VALIDATE(Func != nullptr);
		return Func->Invoke(Forward<TArgs>(Args)...);
	}

	FORCEINLINE TReturn operator()(TArgs&&... Args) noexcept
	{
		return Invoke(Forward<TArgs>(Args)...);
	}

	FORCEINLINE explicit operator bool() const noexcept
	{
		return (Func != nullptr);
	}

	FORCEINLINE TFunction& operator=(const TFunction& Other) noexcept
	{
		if (this != &Other)
		{
			Release();
			CopyConstruct(Other);
		}

		return *this;
	}

	FORCEINLINE TFunction& operator=(TFunction&& Other) noexcept
	{
		if (this != &Other)
		{
			Release();
			MoveConstruct(Other);
		}

		return *this;
	}

	FORCEINLINE TFunction& operator=(std::nullptr_t) noexcept
	{
		Release();
		return *this;
	}

private:
	FORCEINLINE void Release() noexcept
	{
		if (Func)
		{
			if (StackAllocated)
			{
				Func->~IFunctor();
			}
			else
			{
				delete Func;
			}

			Func = nullptr;
		}
	}
	
	template<typename F>
	FORCEINLINE TEnableIf<std::is_invocable_v<F, TArgs...>> Construct(F&& Functor) noexcept
	{
		if constexpr (CanStackAllocate<F>())
		{
			Func = new(reinterpret_cast<Void*>(StackBuffer)) TGenericFunctor<F>(::Forward<F>(Functor));
			StackAllocated = true;
		}
		else
		{
			Func = new TGenericFunctor<F>(::Forward<F>(Functor));
			SizeInBytes		= sizeof(TGenericFunctor<F>);
			StackAllocated	= false;
		}
	}
	
	FORCEINLINE void MoveConstruct(TFunction&& Other) noexcept
	{
		if (Other.StackAllocated)
		{
			Func = Other.Func->Move(reinterpret_cast<Void*>(StackBuffer));
			StackAllocated = true;
		}
		else
		{
			Func = Other.Func->Move(::malloc(Other.SizeInBytes));
			SizeInBytes		= Other.SizeInBytes;
			StackAllocated	= false;
		}
	}
	
	FORCEINLINE void CopyConstruct(const TFunction& Other) noexcept
	{
		if (Other.StackAllocated)
		{
			Func = Other.Func->Clone(reinterpret_cast<Void*>(StackBuffer));
			StackAllocated = true;
		}
		else
		{
			Func = Other.Func->Clone(::malloc(Other.SizeInBytes));
			SizeInBytes		= Other.SizeInBytes;
			StackAllocated	= false;
		}
	}
	
	template<typename F>
	FORCEINLINE static constexpr bool CanStackAllocate() noexcept
	{
		constexpr UInt32 StackSize 		= sizeof(StackBuffer);
		constexpr UInt32 FunctorSize 	= sizeof(TGenericFunctor<F>);
		return FunctorSize <= StackSize;
	}

private:
	IFunctor* Func = nullptr;
	Bool StackAllocated = true;
	union
	{
		Byte StackBuffer[23];
		UInt32 SizeInBytes;
	};
};
