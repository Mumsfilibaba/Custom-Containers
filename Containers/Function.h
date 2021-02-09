#pragma once
#include "Utilities.h"
#include "Allocator.h"

// TMemberFunction - Encapsulates a member function

template<typename T, typename TInvokable>
class TMemberFunction;

template<typename T, typename TReturn, typename... TArgs>
class TMemberFunction<T, TReturn(TArgs...)>
{
public:
    typedef TReturn(T::* TFunctionType)(TArgs...);
    
    TMemberFunction(T* InThis, TFunctionType InFunc) noexcept
        : mThis(InThis)
        , mFunc(InFunc)
    {
    }
    
    TReturn Invoke(TArgs&&... Args) noexcept
    {
        return ((*mThis).*mFunc)(::Forward<TArgs>(Args)...);
    }
    
    FORCEINLINE TReturn operator()(TArgs&&... Args) noexcept
    {
        return Invoke(::Forward<TArgs>(Args)...);
    }
    
private:
    T* mThis;
    TFunctionType mFunc;
};

 // TConstMemberFunction - Encapsulates a const member function

template<typename T, typename TInvokable>
class TConstMemberFunction;

template<typename T, typename TReturn, typename... TArgs>
class TConstMemberFunction<T, TReturn(TArgs...)>
{
public:
    typedef TReturn(T::* TFunctionType)(TArgs...) const;
    
    TConstMemberFunction(const T* InThis, TFunctionType InFunc) noexcept
        : mThis(InThis)
        , mFunc(InFunc)
    {
    }
    
    TReturn Invoke(TArgs&&... Args) noexcept
    {
        return ((*mThis).*mFunc)(::Forward<TArgs>(Args)...);
    }
    
    FORCEINLINE TReturn operator()(TArgs&&... Args) noexcept
    {
        return Invoke(::Forward<TArgs>(Args)...);
    }
    
private:
    const T*      mThis;
    TFunctionType mFunc;
};

// BindFunction

template<typename T, typename TReturn, typename... TArgs>
inline TMemberFunction<T, TReturn(TArgs...)> BindFunction(T* mThis, TReturn (T::*mFunc)(TArgs...)) noexcept
{
    return ::Move(TMemberFunction<T, TReturn(TArgs...)>(mThis, mFunc));
}

template<typename T, typename TReturn, typename... TArgs>
inline TConstMemberFunction<T, TReturn(TArgs...)> BindFunction(const T* mThis, TReturn (T::*mFunc)(TArgs...) const) noexcept
{
    return ::Move(TConstMemberFunction<T, TReturn(TArgs...)>(mThis, mFunc));
}

// TFunction - Encapsulates callables similar to std::function

template<typename TInvokable>
class TFunction;

template<typename TReturn, typename... TArgs>
class TFunction<TReturn(TArgs...)>
{
private:  
    class IFunctor
    {
    public:
        virtual ~IFunctor() = default;

        virtual TReturn Invoke(TArgs&&... Args) noexcept = 0;

        virtual IFunctor* Clone(void* Memory) noexcept = 0;
        virtual IFunctor* Move(void* Memory) noexcept = 0;
    };
    
    template<typename F>
    class TGenericFunctor : public IFunctor
    {
    public:
        TGenericFunctor(const F& InFunctor) noexcept
            : IFunctor()
            , mFunctor(InFunctor)
        {
        }

        TGenericFunctor(const TGenericFunctor& Other) noexcept
            : IFunctor()
            , mFunctor(Other.mFunctor)
        {
        }

        TGenericFunctor(TGenericFunctor&& Other) noexcept
            : IFunctor()
            , mFunctor(::Move(Other.mFunctor))
        {
            if constexpr (std::is_pointer<F>())
            {
                Other.mFunctor = nullptr;
            }
        }

        virtual TReturn Invoke(TArgs&&... Args) noexcept override final
        {
            return mFunctor(Forward<TArgs>(Args)...);
        }

        virtual IFunctor* Clone(void* Memory) noexcept override final
        {
            return new(Memory) TGenericFunctor(*this);
        }

        virtual IFunctor* Move(void* Memory) noexcept override final
        {
            return new(Memory) TGenericFunctor(::Move(*this));
        }

    private:
        F mFunctor;
    };

public:
    TFunction() noexcept
        : mFunc(nullptr)
        , mStackBuffer()
        , mStackAllocated(true)
    {
    }

    TFunction(std::nullptr_t) noexcept
        : mFunc(nullptr)
        , mStackBuffer()
        , mStackAllocated(true)
    {
    }

    template<typename F>
    TFunction(F Functor) noexcept
        : mFunc(nullptr)
        , mStackBuffer()
        , mStackAllocated(true)
    {
        InternalConstruct(::Forward<F>(Functor));
    }

    TFunction(const TFunction& Other) noexcept
        : mFunc(nullptr)
        , mStackBuffer()
        , mStackAllocated(true)
    {
        InternalCopyConstruct(Other);
    }

    TFunction(TFunction&& Other) noexcept
        : mFunc(nullptr)
        , mStackBuffer()
        , mStackAllocated(true)
    {
        InternalMoveConstruct(::Forward<TFunction>(Other));
    }

    ~TFunction()
    {
        InternalRelease();
    }

    void Swap(TFunction& Other) noexcept
    {
        TFunction TempFunc(*this);
        *this = Other;
        Other = TempFunc;
    }

    template<typename F>
    void Assign(F&& Functor) noexcept
    {
        InternalRelease();
        InternalConstruct(::Forward<F>(Functor));
    }
    
    TReturn Invoke(TArgs&&... Args) noexcept
    {
        VALIDATE(mFunc != nullptr);
        return mFunc->Invoke(Forward<TArgs>(Args)...);
    }

    TReturn operator()(TArgs&&... Args) noexcept
    {
        return Invoke(Forward<TArgs>(Args)...);
    }

    explicit operator bool() const noexcept
    {
        return (mFunc != nullptr);
    }

    TFunction& operator=(const TFunction& Other) noexcept
    {
        if (this != &Other)
        {
            InternalRelease();
            InternalCopyConstruct(Other);
        }

        return *this;
    }

    TFunction& operator=(TFunction&& Other) noexcept
    {
        if (this != &Other)
        {
            InternalRelease();
            InternalMoveConstruct(Other);
        }

        return *this;
    }

    TFunction& operator=(std::nullptr_t) noexcept
    {
        InternalRelease();
        return *this;
    }

private:
    void InternalRelease() noexcept
    {
        if (mFunc)
        {
            if (mStackAllocated)
            {
                mFunc->~IFunctor();
            }
            else
            {
                delete mFunc;
            }

            mFunc = nullptr;
        }
    }
    
    template<typename F>
    TEnableIf<std::is_invocable_v<F, TArgs...>> InternalConstruct(F&& Functor) noexcept
    {
        if constexpr (CanStackAllocate<F>())
        {
            mFunc = new(reinterpret_cast<void*>(mStackBuffer)) TGenericFunctor<F>(::Forward<F>(Functor));
            mStackAllocated = true;
        }
        else
        {
            mFunc = new TGenericFunctor<F>(::Forward<F>(Functor));
            mSizeInBytes    = sizeof(TGenericFunctor<F>);
            mStackAllocated = false;
        }
    }
    
    void InternalMoveConstruct(TFunction&& Other) noexcept
    {
        if (Other.mStackAllocated)
        {
            mFunc = Other.mFunc->Move(reinterpret_cast<void*>(mStackBuffer));
            mStackAllocated = true;
        }
        else
        {
            mFunc = Other.mFunc->Move(::malloc(Other.mSizeInBytes));
            mSizeInBytes    = Other.mSizeInBytes;
            mStackAllocated = false;
        }
    }
    
    void InternalCopyConstruct(const TFunction& Other) noexcept
    {
        if (Other.mStackAllocated)
        {
            mFunc = Other.mFunc->Clone(reinterpret_cast<void*>(mStackBuffer));
            mStackAllocated = true;
        }
        else
        {
            mFunc = Other.mFunc->Clone(::malloc(Other.mSizeInBytes));
            mSizeInBytes    = Other.mSizeInBytes;
            mStackAllocated = false;
        }
    }
    
    template<typename F>
    static constexpr bool CanStackAllocate() noexcept
    {
        constexpr UInt32 StackSize   = sizeof(mStackBuffer);
        constexpr UInt32 FunctorSize = sizeof(TGenericFunctor<F>);
        return FunctorSize <= StackSize;
    }

private:
    IFunctor* mFunc      = nullptr;
    Bool mStackAllocated = true;
    union
    {
        Byte   mStackBuffer[23];
        UInt32 mSizeInBytes;
    };
};
