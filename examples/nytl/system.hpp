/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 nyorain
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

///\file
///\brief Defines useful macros and functions concerning system or compiler.

#pragma once
#include <cstdint>

namespace nytl
{

/* possible macros: NYTL_OS_NAME NYTL_COMPILER_NAME
 * OS: NYTL_OS_64, NYTL_OS_WINDOWS, NYTL_OS_WINDOWS64, NYTL_OS_LINUX, NYTL_OS_ANDROID,
 *		NYTL_OS_MACOS, NYTL_OS_IOS, NYTL_OS_BSD, NYTL_OS_UNKNOWN
 * Version: NYTL_ANDROID_VERION
 *
 * Compiler: NYTL_COMPILER_64, NYTL_COMPILER_INTEL, NYTL_COMPILER_MSC, NYTL_COMPILER_GNU,
 *		NYTL_COMPILER_CLANG, NYTL_COMPILER_BORLAND, NYTL_COMPILER_MINGW, NYTL_COMPILER_MINGW64
 * Version: NYTL_GNU_VERSION, NYTL_MSC_VERSION, NYTL_INTEL_VERSION, NYTL_CLANG_VERSION,
 *		NYTL_MINGW32_VERSION, NYTL_MINGW64_VERSION
 *
 * NYTL_PRETTY_FUNCTION, NYTL_DEPRECATED
 */

//http://sourceforge.net/p/predef/wiki
//OS////////////
#if defined(_WIN32) || defined(__WIN32__)
    #define NYTL_OS_WINDOWS 1
    #define NYTL_OS_NAME "Windows"

    #ifdef _WIN64
        #define NYTL_OS_64 1
        #define NYTL_OS_WINDOWS64 1
    #endif // _WIN64

#elif defined(__linux__) || defined(linux) || defined(__linux)
    #define NYTL_OS_LINUX 1

    #ifdef __ANDROID__
        #define NYTL_OS_ANDROID 1
        #define NYTL_OS_NAME "Android"
    #else
        #define NYTL_OS_NAME "Linux"
    #endif //__ANDROID__

#elif defined(__APPLE__) || defined(__MACH__) || defined(macintosh) || defined(Macintosh)
    #define NYTL_OS_MACOS 1

    #include "TargetConditionals.h"
    #ifdef TARGET_OS_IPHONE
        #define NYTL_OS_IOS 1
        #define NYTL_OS_NAME "ios"
    #else
        #define NYTL_OS_NAME "MacOS"
    #endif //IOS

#elif defined(__FreeBSD__) || defined(__OpenBSD__)
    #define NYTL_OS_BSD 1
    #define NYTL_OS_NAME "BSD"

#else
    #define NYTL_OS_UNKOWN 1
    #define NYTL_OS_NAME "Unknown"

    //#warning NYTL::SYSTEM: unknown or unsupported OS

#endif //OS



//Compiler////
#if defined(__BORLANDC__)
    #define NYTL_COMPILER_BORLAND 1
    #define NYTL_COMPILER_NAME "Borland"


#elif defined(__MINGW32__)
    #define NYTL_COMPILER_MINGW 1
    #define NYTL_COMPILER_NAME "MINGW"

    #define NYTL_MINGW32_VERSION (_MINGW32_MAJOR_VERSION * 100 + _MINGW32_MINOR_VERSION)

    #if defined(__MINGW64__)
        #define NYTL_COMPILER_64 1
        #define NYTL_COMPILER_MINGW64 1
        #define NYTL_MINGW64_VERSION (_MINGW64_MAJOR_VERSION * 100 + _MINGW64_MINOR_VERSION)
    #endif

#elif defined(__clang__)
    #define NYTL_COMPILER_CLANG 1
    #define NYTL_COMPILER_NAME "Clang"

    #define NYTL_CLANG_VERSION ( __clang_major__ * 100 + __clang_minor__)

#elif defined(__INTEL_COMPILER)
    #define NYTL_COMPILER_INTEL 1
    #define NYTL_COMPILER_NAME "Intel"

    #define NYTL_INTEL_VERSION __INTEL_COMPILER

#elif defined(_MSC_VER)
    #define NYTL_COMPILER_MSC 1
    #define NYTL_COMPILER_NAME "MSC"

    #define NYTL_MSC_VERSION _MSC_VER

#elif defined(__GNUC__)
    #define NYTL_COMPILER_GNU 1
    #define NYTL_COMPILER_NAME "GNU"

    #define NYTL_GNU_VERSION (__GNUC__ * 100 + __GNUC_MINOR__)

#else
    #define NYTL_COMPILER_UNKNOWN 1
    #define NYTL_COMPILER_NAME "Unknown"

    //#warning NYTL::SYSTEM: unknown or unsupported Compiler

#endif //Compiler

//deprecated
#if __cplusplus >= 201402L
	#define NYTL_DEPRECATED [[deprecated]]
#else
	#if defined(__GNUC__)
		#define NYTL_DEPRECATED __attribute__ ((deprecated))
	#elif defined(_MSC_VER)
		#define NYTL_DEPRECATED __declspec(deprecated)
	#else
		//#pragma message("WARNING: You need to implement DEPRECATED for this compiler")
		#define NYTL_DEPRECATED
	#endif
#endif

//c++14 constexpr
#ifndef NYTL_CPP14_CONSTEXPR
    #if __cplusplus >= 201402L
        #define NYTL_CPP14_CONSTEXPR constexpr
    #else
        #define NYTL_CPP14_CONSTEXPR
    #endif
#endif //NYTL_cpp14_constexpr

//nytl pretty function
#if defined(NYTL_COMPILER_CLANG) || defined(NYTL_COMPILER_GNU) || __GNUC__
	#define NYTL_PRETTY_FUNCTION __PRETTY_FUNCTION__
#elif defined(_MSC_VER)
	#define NYTL_PRETTY_FUNCTION __FUNCTION__
#else
	#define NYTL_PRETTY_FUNCTION __func__
#endif

///\ingroup utility
constexpr const char* osName()
{
    return NYTL_OS_NAME;
}

///\ingroup utility
constexpr const char* compilerName()
{
    return NYTL_COMPILER_NAME;
}

///\ingroup utility
constexpr bool is64Bit()
{
    #ifdef NYTL_OS_64
        return 1;
    #else
        return 0;
    #endif // NYTL_OS_64
}

namespace detail
{
    static constexpr std::uint32_t dummyEndianTest = 0x1;
}

///\ingroup utility
inline bool littleEndian()
{
    return (((std::uint8_t*)&detail::dummyEndianTest)[0] == 1);
}

///\ingroup utility
inline bool bigEndian()
{
	return !littleEndian();
}

}
