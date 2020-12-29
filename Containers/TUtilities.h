#pragma once
#include "Types.h"

#include <type_traits>

/*
 * Validation
 */

#ifndef VALIDATE
	#include <cassert>
	#define VALIDATE(Condition)	assert((Condition))
#endif

/*
 * Unreferenced Variable
 */

#ifndef UNREFERENCED_VARIABLE
	#define UNREFERENCED_VARIABLE(Variable) ((void)(Variable))
#endif

/*
 * Forceinline
 */

#ifndef FORCEINLINE

#ifndef _DEBUG
#if	defined(_WIN32)
	#define FORCEINLINE	__forceinline
#elif defined(__APPLE__)
	#define FORCEINLINE	__attribute__((__always_inline__))
#else
	#define FORCEINLINE inline
#endif // Platform
#else
	#define FORCEINLINE inline
#endif // Debug
#endif // Forceinline

/*
 * TRemoveReference - Removes reference and retrives the types
 */

template<typename T>
struct _TRemoveReference
{
	using TType = T;
};

template<typename T>
struct _TRemoveReference<T&>
{
	using TType = T;
};

template<typename T>
struct _TRemoveReference<T&&>
{
	using TType = T;
};

template<typename T>
using TRemoveReference = typename _TRemoveReference<T>::TType;

/*
 * TRemoveExtent - Removes array type
 */

template<typename T>
struct _TRemoveExtent
{
	using TType = T;
};

template<typename T>
struct _TRemoveExtent<T[]>
{
	using TType = T;
};

template<typename T, size_t SIZE>
struct _TRemoveExtent<T[SIZE]>
{
	using TType = T;
};

template<typename T>
using TRemoveExtent = typename _TRemoveExtent<T>::TType;

/*
 * Move
 */

// Move an object by converting it into a rvalue
template<typename T>
constexpr TRemoveReference<T>&& Move(T&& Arg) noexcept
{
	return static_cast<TRemoveReference<T>&&>(Arg);
}

/*
 * Forward
 */

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

/*
 * TEnableIf
 */

template<Bool B, typename T = Void>
struct _TEnableIf
{
};

template<typename T>
struct _TEnableIf<true, T>
{
	using TType = T;
};

template<Bool B, typename T = Void>
using TEnableIf = typename _TEnableIf<B, T>::TType;

/*
 * TIsArray - Check for array type
 */

template<typename T>
struct _TIsArray
{
	static constexpr Bool Value = false;
};

template<typename T>
struct _TIsArray<T[]>
{
	static constexpr Bool Value = true;
};

template<typename T, Int32 N>
struct _TIsArray<T[N]>
{
	static constexpr Bool Value = true;
};

template<typename T>
inline constexpr Bool TIsArray = _TIsArray<T>::Value;
