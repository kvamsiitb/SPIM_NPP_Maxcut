/************************ \cond COPYRIGHT *****************************
 *                                                                    *
 * Copyright (C) 2020 HOLOEYE Photonics AG. All rights reserved.      *
 * Contact: https://holoeye.com/contact/                              *
 *                                                                    *
 * This file is part of HOLOEYE SLM Display SDK.                      *
 *                                                                    *
 * You may use this file under the terms and conditions of the        *
 * "HOLOEYE SLM Display SDK Standard License v1.0" license agreement. *
 *                                                                    *
 **************************** \endcond ********************************/


#ifndef HOLOEYE_PLATFORMS_H
#define HOLOEYE_PLATFORMS_H

#ifdef _WIN32
    #define HOLOEYE_WINDOWS 1

    #ifdef _WIN64
        #define HOLOEYE_WINDOWS_32 0
        #define HOLOEYE_WINDOWS_64 1
    #else
        #define HOLOEYE_WINDOWS_32 1
        #define HOLOEYE_WINDOWS_64 0
    #endif

    #if defined(WINVER) && defined(_WIN32_WINNT)
        #undef HOLOEYE_WINDOWS

        #if WINVER >= 0x0A00 || _WIN32_WINNT >= 0x0A00
            #define HOLOEYE_WINDOWS 100
            #define HOLOEYE_WINDOWS_10 1
        #elif WINVER >= 0x0603 || _WIN32_WINNT >= 0x0603
            #define HOLOEYE_WINDOWS 81
            #define HOLOEYE_WINDOWS_81 1
        #elif WINVER >= 0x0602 || _WIN32_WINNT >= 0x0602
            #define HOLOEYE_WINDOWS 80
            #define HOLOEYE_WINDOWS_80 1
        #elif WINVER >= 0x0601 || _WIN32_WINNT >= 0x0601
            #define HOLOEYE_WINDOWS 70
            #define HOLOEYE_WINDOWS_7 1
        #else
            #error(Unsupported Windows Version)
        #endif
    #endif
#endif

#ifdef __APPLE__
    #define HOLOEYE_APPLE 1

    #include <TargetConditionals.h>

    #if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
        #define HOLOEYE_IOS 1
        #define HOLOEYE_MACINTOSH 0
    #elif TARGET_OS_MAC
        #define HOLOEYE_IOS 0
        #define HOLOEYE_MACINTOSH 1
    #endif
#endif

#ifdef __ANDROID__
    #define HOLOEYE_ANDROID 1
#else
    #ifdef __linux__
        #define HOLOEYE_LINUX 1
    #endif
#endif

#if !HOLOEYE_WINDOWS && !HOLOEYE_MACINTOSH && !HOLOEYE_LINUX && !HOLOEYE_IOS && !HOLOEYE_ANDROID
    #error Failed to detect platform in holoeye_platforms.h
#endif

#ifndef HOLOEYE_WINDOWS
    #define HOLOEYE_WINDOWS 0
    #define HOLOEYE_WINDOWS_32 0
    #define HOLOEYE_WINDOWS_64 0
#endif

#ifndef HOLOEYE_APPLE
    #define HOLOEYE_APPLE 0
    #define HOLOEYE_IOS 0
    #define HOLOEYE_MACINTOSH 0
#endif

#ifndef HOLOEYE_LINUX
    #define HOLOEYE_LINUX 0
#endif

#ifndef HOLOEYE_ANDROID
    #define HOLOEYE_ANDROID 0
#endif

#if HOLOEYE_WINDOWS || HOLOEYE_MACINTOSH || HOLOEYE_LINUX
    #define HOLOEYE_DESKTOP 1
    #define HOLOEYE_MOBILE 0
#else
    #define HOLOEYE_DESKTOP 0
    #define HOLOEYE_MOBILE 1
#endif

#ifdef __cplusplus
    #define HOLOEYE_CPP 1
    #define HOLOEYE_C 0

    #if __cplusplus >= 201703L || (defined(_MSC_VER) && _MSC_VER >= 1910)
        #define HOLOEYE_CPP_VER 17
        #define HOLOEYE_CPP17 1
    #elif __cplusplus >= 201402L || (defined(_MSC_VER) && _MSC_VER >= 1900)
        #define HOLOEYE_CPP_VER 14
        #define HOLOEYE_CPP14 1
    #elif __cplusplus >= 201103L || (defined(_MSC_VER) && _MSC_VER >= 1800)
        #define HOLOEYE_CPP_VER 11
        #define HOLOEYE_CPP11 1
    #else
        #error Failed to determine C++ standard.
    #endif
#else
    #define HOLOEYE_CPP 0
    #define HOLOEYE_C 1
    #define HOLOEYE_CPP_VER 0
#endif

#ifndef HOLOEYE_CPP11
    #define HOLOEYE_CPP11 0
#endif

#ifndef HOLOEYE_CPP14
    #define HOLOEYE_CPP14 0
#endif

#ifndef HOLOEYE_CPP17
    #define HOLOEYE_CPP17 0
#endif

#define HOLOEYE_CPP11_OR_HIGHER (HOLOEYE_CPP_VER >= 11)
#define HOLOEYE_CPP14_OR_HIGHER (HOLOEYE_CPP_VER >= 14)
#define HOLOEYE_CPP17_OR_HIGHER (HOLOEYE_CPP_VER >= 17)

#if HOLOEYE_CPP_VER < 11
    #define HOLOEYE_REQUIRES_CPP_11 \
        #error This code requires at least C++ 11 to compile correctly.
#else
    #define HOLOEYE_REQUIRES_CPP_11
#endif

#if HOLOEYE_CPP_VER < 14
    #define HOLOEYE_REQUIRES_CPP_14 \
        #error This code requires at least C++ 14 to compile correctly.
#else
    #define HOLOEYE_REQUIRES_CPP_14
#endif

#if HOLOEYE_CPP_VER < 17
    #define HOLOEYE_REQUIRES_CPP_17 \
        #error This code requires at least C++ 17 to compile correctly.
#else
    #define HOLOEYE_REQUIRES_CPP_17
#endif

#if defined(_MSC_VER)
    #define HOLOEYE_COMPILER_MSVC 1
    #define HOLOEYE_COMPILER_GCC 0
    #define HOLOEYE_COMPILER_CLANG 0
    #define HOLOEYE_COMPILER_MINGW 0
#elif defined(__GNUC__)
    #define HOLOEYE_COMPILER_MSVC 0
    #define HOLOEYE_COMPILER_GCC 1
    #define HOLOEYE_COMPILER_CLANG 0
    #define HOLOEYE_COMPILER_MINGW 0
#elif defined(__clang_major__)
    #define HOLOEYE_COMPILER_MSVC 0
    #define HOLOEYE_COMPILER_GCC 0
    #define HOLOEYE_COMPILER_CLANG 1
    #define HOLOEYE_COMPILER_MINGW 0
#elif defined(__MINGW32_MAJOR_VERSION) || defined(__MINGW64_MAJOR_VERSION)
    #define HOLOEYE_COMPILER_MSVC 0
    #define HOLOEYE_COMPILER_GCC 0
    #define HOLOEYE_COMPILER_CLANG 0
    #define HOLOEYE_COMPILER_MINGW 1
#endif

#if defined(__clang_major__)
    #define HOLOEYE_CODEMODEL_CLANG 1
#else
    #define HOLOEYE_CODEMODEL_CLANG 0
#endif

#if defined(NDEBUG) || defined(RELEASE) || defined(QT_NO_DEBUG) || __OPTIMIZE__
    #define HOLOEYE_RELEASE 1
    #define HOLOEYE_DEBUG 0
#else
    #define HOLOEYE_RELEASE 0
    #define HOLOEYE_DEBUG 1
#endif

#ifndef HOLOEYE_SHIPPING
    #define HOLOEYE_SHIPPING 0
#endif

/** Make sure unclean libraries can find _DEBUG. */
#if HOLOEYE_DEBUG && !defined(_DEBUG)
    #define _DEBUG 1
#endif

/** Make sure unclean libraries can find DEBUG. */
#if HOLOEYE_DEBUG && !defined(DEBUG)
    #define DEBUG 1
#endif

/** Make sure unclean libraries can find NDEBUG. */
#if HOLOEYE_RELEASE && !defined(NDEBUG)
    #define NDEBUG 1
#endif

/** Make sure unclean libraries can find RELEASE. */
#if HOLOEYE_RELEASE && !defined(RELEASE)
    #define RELEASE 1
#endif

#if HOLOEYE_CPP
    #define HOLOEYE_CCODE_BEGIN() extern "C" {
    #define HOLOEYE_CCODE_END() }
#else
    #define HOLOEYE_CCODE_BEGIN()
    #define HOLOEYE_CCODE_END()
#endif

#define HOLOEYE_BOOL int
#define HOLOEYE_NULL 0
#define HOLOEYE_TRUE 1
#define HOLOEYE_FALSE 0

#define HOLOEYE_PI  3.1415926535897932384626433832795
#define HOLOEYE_PIF 3.1415926535897932384626433832795f

#define HOLOEYE_DEG2RAD(value) (value * (HOLOEYE_PI / 180.0))
#define HOLOEYE_DEG2RADF(value) (value * (HOLOEYE_PIF / 180.0f))

#define HOLOEYE_RAD2DEG(value) (value * (180.0 / HOLOEYE_PI))
#define HOLOEYE_RAD2DEGF(value) (value * (180.0f / HOLOEYE_PIF))

/** Prevent any min max troubles. */
#if HOLOEYE_WINDOWS && !defined(NOMINMAX)
    #define NOMINMAX
#endif

#if defined(_MSC_VER)
    #if _MSC_VER < 1900
        /** Because VS2013 only supports a subset of C++ 11 we have to define some macros for it. */
        #define HOLOEYE_NOEXCEPT
        #define HOLOEYE_CONSTEXPR
        #define HOLOEYE_CONSTEXPR_IF
        #define HOLOEYE_CONSTEXPR_LAMBDA
        #define HOLOEYE_CONSTEXPR_SIMPLE
    #elif _MSC_VER < 1910
        /** VS2015 supports the new constexpr. */
        #define HOLOEYE_NOEXCEPT noexcept
        #define HOLOEYE_CONSTEXPR
        #define HOLOEYE_CONSTEXPR_IF
        #define HOLOEYE_CONSTEXPR_LAMBDA
        #define HOLOEYE_CONSTEXPR_SIMPLE constexpr
    #elif _MSC_VER < 1911
        /** VS2017 supports the new constexpr N3652. */
        #define HOLOEYE_NOEXCEPT noexcept
        #define HOLOEYE_CONSTEXPR constexpr
        #define HOLOEYE_CONSTEXPR_IF
        #define HOLOEYE_CONSTEXPR_LAMBDA
        #define HOLOEYE_CONSTEXPR_SIMPLE constexpr
    #else
        /** VS2017 15.3 supports the new constexpr P0292R2 for if statements. */
        #define HOLOEYE_NOEXCEPT noexcept
        #define HOLOEYE_CONSTEXPR constexpr
        #define HOLOEYE_CONSTEXPR_IF constexpr
        #define HOLOEYE_CONSTEXPR_LAMBDA constexpr
        #define HOLOEYE_CONSTEXPR_SIMPLE constexpr
    #endif
#else
    #if HOLOEYE_CPP17
        #define HOLOEYE_NOEXCEPT noexcept
        #define HOLOEYE_CONSTEXPR constexpr
        #define HOLOEYE_CONSTEXPR_IF constexpr
        #define HOLOEYE_CONSTEXPR_LAMBDA constexpr
        #define HOLOEYE_CONSTEXPR_SIMPLE constexpr
    #elif HOLOEYE_CPP14
        #define HOLOEYE_NOEXCEPT noexcept
        #define HOLOEYE_CONSTEXPR constexpr
        #define HOLOEYE_CONSTEXPR_IF
        #define HOLOEYE_CONSTEXPR_LAMBDA
        #define HOLOEYE_CONSTEXPR_SIMPLE constexpr
    #elif HOLOEYE_CPP11
        #define HOLOEYE_NOEXCEPT noexcept
        #define HOLOEYE_CONSTEXPR
        #define HOLOEYE_CONSTEXPR_IF
        #define HOLOEYE_CONSTEXPR_LAMBDA
        #define HOLOEYE_CONSTEXPR_SIMPLE constexpr
    #else
        #define HOLOEYE_NOEXCEPT
        #define HOLOEYE_CONSTEXPR
        #define HOLOEYE_CONSTEXPR_IF
        #define HOLOEYE_CONSTEXPR_LAMBDA
        #define HOLOEYE_CONSTEXPR_SIMPLE
    #endif
#endif

#if HOLOEYE_WINDOWS
    #define HOLOEYE_EXPORT __declspec(dllexport)
    #define HOLOEYE_IMPORT __declspec(dllimport)
#endif

#if HOLOEYE_LINUX || HOLOEYE_APPLE
    #define HOLOEYE_EXPORT __attribute__((visibility("default")))
    #define HOLOEYE_IMPORT
#endif

#if HOLOEYE_BUILDING_LIBRARY
    #define HOLOEYE_API HOLOEYE_EXPORT
#else
    #define HOLOEYE_API HOLOEYE_IMPORT
#endif

#if HOLOEYE_CPP11_OR_HIGHER
    #define HOLOEYE_DELETED_FUNC(PrivateBody, AccessAfter) = delete; AccessAfter:
#else
    #define HOLOEYE_DELETED_FUNC(PrivateBody, AccessAfter) private: { PrivateBody; } AccessAfter:
#endif

#if HOLOEYE_CPP11_OR_HIGHER
    #define HOLOEYE_LIKELY(condition) __builtin_expect(!!(condition), 1)
    #define HOLOEYE_UNLIKELY(condition) __builtin_expect(!!(condition), 0)
#else
    #define HOLOEYE_LIKELY(condition) condition
    #define HOLOEYE_UNLIKELY(condition) condition
#endif

#ifdef _MSC_VER
    #define HOLOEYE_RESTRICT __restrict
#else
    #if HOLOEYE_C
        #define HOLOEYE_RESTRICT restrict
    #else
        #define HOLOEYE_RESTRICT __restrict__
    #endif
#endif

#define HOLOEYE_UNUSED(var) (void)var

#if HOLOEYE_CPP
    #define HOLOEYE_DEFARG(def) = def
#else
    #define HOLOEYE_DEFARG(def) 
#endif

#if (defined(_MSC_VER) && _MSC_VER > 1900) || (!defined(_MSC_VER) && HOLOEYE_CPP11_OR_HIGHER)
    #define HOLOEYE_ALIGNAS(align) alignas(align)
#else
    #define HOLOEYE_ALIGNAS(align)

    #ifndef _ENABLE_ATOMIC_ALIGNMENT_FIX
        #define _ENABLE_ATOMIC_ALIGNMENT_FIX
    #endif
#endif

#if defined(_MSC_VER)
    #define HOLOEYE_FUNCTIONNAME __FUNCTION__
#else
    #define HOLOEYE_FUNCTIONNAME __func__
#endif

#if HOLOEYE_WINDOWS
    #define __HOLOEYE_STR(str) L##str
    #define HOLOEYE_STR(str) __HOLOEYE_STR(str)
#else
    #define __HOLOEYE_STR(str) str
    #define HOLOEYE_STR(str) str
#endif

#if HOLOEYE_WINDOWS
    #define __HOLOEYE_USTR(str) L##str
#else
    #define __HOLOEYE_USTR(str) u##str
#endif

#define HOLOEYE_USTR(str) __HOLOEYE_USTR(str)

#define __HOLOEYE_CONCAT(a, b) a##b
#define HOLOEYE_CONCAT(a, b) __HOLOEYE_CONCAT(a, b)

#if HOLOEYE_CODEMODEL_CLANG
    #define HOLOEYE_NOTNULL __attribute__((nonnull))
    #define HOLOEYE_NORETURN __attribute__((noreturn))
    #define HOLOEYE_ASSUME(cond) __builtin_assume(cond)
#else
    #define HOLOEYE_NOTNULL
    #define HOLOEYE_NORETURN

    #if HOLOEYE_COMPILER_MSVC
        #define HOLOEYE_ASSUME(cond) __assume(cond)
    #else
        #define HOLOEYE_ASSUME(cond)
    #endif
#endif

#endif // HOLOEYE_PLATFORMS_H
