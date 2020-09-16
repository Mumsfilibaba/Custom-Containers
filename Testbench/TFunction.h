#include "TypeUtilities.h"

/*
 * TFunction - Saves function pointers and is callable
 */
template<typename TCallable>
class TFunction;

template<typename TReturn, typename... TArgs>
class TFunction<TReturn(TArgs...)>
{
private:
    // Handles the basecase
    class IFunction
    {
    public:
		virtual ~IFunction() = default;
        virtual TReturn Execute(TArgs... Args) noexcept = 0;
    };
	
    // Member functions
	template<typename T>
    class MemberFunction : public IFunction
    {
    public:
		typedef T* TPtr;
        typedef TReturn(T::*FunctionType)(TArgs...);

        inline MemberFunction(TPtr InThis, FunctionType InFunc) noexcept
            : IFunction()
            , Func(InFunc)
			, This(InThis)
        {
        }

        inline virtual TReturn Execute(TArgs... Args) noexcept override final
        {
			return ((*This).*Func)(Forward<TArgs>(Args)...);
        }

    private:
        FunctionType Func;
		TPtr This;
    };
	
	// Generic functors
	template<typename F>
	class GenericFunctor : public IFunction
	{
	public:
        inline GenericFunctor(const F& InFunctor) noexcept
            : Functor(InFunctor)
        {
        }

        inline virtual TReturn Execute(TArgs... Args) noexcept override final
        {
			return Functor(Forward<TArgs>(Args)...);
        }
		
	private:
		F Functor;
	};

public:
	inline TFunction() noexcept
		: StackBuffer()
		, Func(nullptr)
	{
		memset(StackBuffer, 0, sizeof(StackBuffer));
	}
	
	inline TFunction(std::nullptr_t) noexcept
		: StackBuffer()
		, Func(nullptr)
	{
		memset(StackBuffer, 0, sizeof(StackBuffer));
	}
	
	template<typename F>
    inline TFunction(F Functor) noexcept
        : StackBuffer()
        , Func(nullptr)
    {
		constexpr Uint32 StackSize = sizeof(StackBuffer);
		constexpr Uint32 FunctorSize = sizeof(GenericFunctor<F>);
		
		VALIDATE(FunctorSize <= StackSize);
		
        new(reinterpret_cast<VoidPtr>(StackBuffer)) GenericFunctor<F>(Functor);
        Func = reinterpret_cast<IFunction*>(StackBuffer);
    }
	
	template<typename T>
    inline TFunction(T* This, TReturn(T::*MemberFunc)(TArgs...)) noexcept
        : StackBuffer()
        , Func(nullptr)
    {
		constexpr Uint32 StackSize 	= sizeof(StackBuffer);
		constexpr Uint32 FuncSize 	= sizeof(MemberFunction<T>);
		
		VALIDATE(FuncSize <= StackSize);
		
        new(reinterpret_cast<VoidPtr>(StackBuffer)) MemberFunction<T>(This, MemberFunc);
        Func = reinterpret_cast<IFunction*>(StackBuffer);
    }
	
	inline TFunction(const TFunction& Other) noexcept
		: StackBuffer()
		, Func(nullptr)
	{
		memcpy(StackBuffer, Other.StackBuffer, sizeof(StackBuffer));
		Func = reinterpret_cast<IFunction*>(StackBuffer);
	}
	
	inline TFunction(TFunction&& Other) noexcept
		: StackBuffer()
		, Func(nullptr)
	{
		memcpy(StackBuffer, Other.StackBuffer, sizeof(StackBuffer));
		Func = reinterpret_cast<IFunction*>(StackBuffer);
		
		memset(Other.StackBuffer, 0, sizeof(StackBuffer));
		Other.Func = nullptr;
	}
	
	inline ~TFunction()
	{
		InternalRelease();
	}
	
	void Swap(TFunction& Other)
	{
		Byte TempBuffer[56];
		memcpy(TempBuffer, StackBuffer, sizeof(StackBuffer));
		
		memcpy(StackBuffer, Other.StackBuffer, sizeof(StackBuffer));
		Func = reinterpret_cast<IFunction*>(StackBuffer);
		
		memcpy(Other.StackBuffer, TempBuffer, sizeof(StackBuffer));
		Other.Func = reinterpret_cast<IFunction*>(Other.StackBuffer);
	}
	
	template<typename F>
	void Assign(F&& Functor)
	{		
		constexpr Uint32 StackSize = sizeof(StackBuffer);
		constexpr Uint32 FunctorSize = sizeof(GenericFunctor<F>);
		
		VALIDATE(FunctorSize <= StackSize);

		InternalRelease();
		
        new(reinterpret_cast<VoidPtr>(StackBuffer)) GenericFunctor<F>(Functor);
        Func = reinterpret_cast<IFunction*>(StackBuffer);
	}
	
	template<typename T>
	void Assign(T* This, TReturn(T::*MemberFunc)(TArgs...))
	{
		constexpr Uint32 StackSize 	= sizeof(StackBuffer);
		constexpr Uint32 FuncSize 	= sizeof(MemberFunction<T>);
		
		VALIDATE(FuncSize <= StackSize);
		
		InternalRelease();
		
		new(reinterpret_cast<VoidPtr>(StackBuffer)) MemberFunction<T>(This, MemberFunc);
		Func = reinterpret_cast<IFunction*>(StackBuffer);
	}
	
	TReturn Call(TArgs... Args) noexcept
	{
		VALIDATE(Func != nullptr);
		return Func->Execute(Forward<TArgs>(Args)...);
	}
	
	TReturn operator()(TArgs... Args) noexcept
	{
		return Call(Forward<TArgs>(Args)...);
	}

	explicit operator bool() const noexcept
	{
		return (Func != nullptr);
	}
	
	TFunction& operator=(const TFunction& Other) noexcept
	{
		if (this != &Other)
		{
			InternalRelease();
			
			memcpy(StackBuffer, Other.StackBuffer, sizeof(StackBuffer));
			Func = reinterpret_cast<IFunction*>(StackBuffer);
		}
		
		return *this;
	}
	
	TFunction& operator=(TFunction&& Other) noexcept
	{
		if (this != &Other)
		{
			InternalRelease();
			
			memcpy(StackBuffer, Other.StackBuffer, sizeof(StackBuffer));
			Func = reinterpret_cast<IFunction*>(StackBuffer);
			
			memset(Other.StackBuffer, 0, sizeof(StackBuffer));
		}
		
		return *this;
	}
	
private:
	void InternalRelease()
	{
		if (Func)
		{
			Func->~IFunction();
		}
	}
	
    Byte StackBuffer[56];
    IFunction* Func;
};
