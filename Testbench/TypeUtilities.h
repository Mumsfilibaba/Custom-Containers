#pragma once
#include <cassert>

#define VALIDATE(Condition)	assert((Condition))

#if	defined(_WIN32)
	#define FORCEINLINE	__forceinline
#elif defined(__APPLE__)
	#define FORCEINLINE	__attribute__((__always_inline__))
#else
	#define FORCEINLINE inline
#endif

typedef unsigned int		Uint32;
typedef unsigned long long	Uint64;
typedef int					Int32;
typedef char				Char;

// Removes reference retrives the types
template<typename T>
struct _TRemoveReference
{
	using TType = T;
	using TConstRefType = const T;
};

template<typename T>
struct _TRemoveReference<T&>
{
	using TType = T;
	using TConstRefType = const T&;
};

template<typename T>
struct _TRemoveReference<T&&>
{
	using TType = T;
	using TConstRefType = const T&&;
};

template<typename T>
using TRemoveReference = typename _TRemoveReference<T>::TType;

// Move an object by converting it into a rvalue
template<typename T>
constexpr TRemoveReference<T>&& Move(T&& Arg) noexcept
{
	return static_cast<TRemoveReference<T>&&>(Arg);
}

// Forward an object by converting it into a rvalue from an lvalue
template<typename T>
constexpr T&& Forward(TRemoveReference<T>& Arg) noexcept
{
	return static_cast<T&&>(Arg);
}

// Forward an object by converting it into a rvalue from an rvalue
template<typename T>
constexpr T&& Forward(TRemoveReference<T>&& Arg) noexcept
{
	return static_cast<T&&>(Arg);
}
