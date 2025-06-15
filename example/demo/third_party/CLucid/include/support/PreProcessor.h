//
// Created by weichuanqi on 2025/6/5.
//

#ifndef GENERATORS_PREPROCESSOR_H
#define GENERATORS_PREPROCESSOR_H

#if defined(_MSC_VER)
#if _MSC_VER >= 1910
#define COMPILER_VC 15
#elif _MSC_VER >= 1900
#define COMPILER_VC 14
#elif _MSC_VER >= 1800
#define COMPILER_VC 12
#elif _MSC_VER >= 1700
#define COMPILER_VC 11
#elif _MSC_VER >= 1600
#define COMPILER_VC 10
#elif _MSC_VER >= 1500
#define COMPILER_VC 9
#else
#error "Unknown VC version"
#endif
#elif defined(__clang__)
#define COMPILER_CLANG 1
#if defined(__clang_major__)
#define COMPILER_CLANG_MAJOR __clang_major__
#elif defined(_clang_major)
#define COMPILER_CLANG_MAJOR _clang_major
#else
#define COMPILER_CLANG_MAJOR 0
#endif
#elif defined(__GNUC__) // note: __clang__ implies __GNUC__
#define COMPILER_GCC 1
#else
//#error "Unknown compiler"
#endif

/**
Operating system defines, see http://sourceforge.net/p/predef/wiki/OperatingSystems/
*/

#if defined(_XBOX_ONE)
#define PLATFORM_XBOXONE 1
#elif defined(_GAMING_XBOX) || defined(_GAMING_XBOX_SCARLETT)
#define PLATFORM_XBOX_SERIES_X 1
#elif defined(WINAPI_FAMILY) && WINAPI_FAMILY == WINAPI_FAMILY_APP
#define PLATFORM_UWP 1
#elif defined(_WIN64) // note: _XBOX_ONE implies _WIN64
#define PLATFORM_WIN64 1
#elif defined(_WIN32) // note: _M_PPC implies _WIN32
#define PLATFORM_WIN32 1
#elif defined(__ANDROID__)
#define PLATFORM_ANDROID 1
#elif defined(__linux__) || defined(__EMSCRIPTEN__) // note: __ANDROID__ implies __linux__
#define PLATFORM_LINUX 1
#elif defined(__APPLE__) && (defined(__arm__) || defined(__arm64__))
#define PLATFORM_IOS 1
#elif defined(__APPLE__)
#define PLATFORM_OSX 1
#elif defined(__ORBIS__)
#define PLATFORM_PS4 1
#elif defined(__NX__)
#define PLATFORM_SWITCH 1
#else
//#error "Unknown operating system"
#endif

/**
Architecture defines, see http://sourceforge.net/p/predef/wiki/Architectures/
*/
#if defined(__x86_64__) || defined(_M_X64) // ps4 compiler defines _M_X64 without value
#define ARCH_X64 1
#elif defined(__i386__) || defined(_M_IX86) || defined(__EMSCRIPTEN__)
#define ARCH_X86 1
#elif defined(__arm64__) || defined(__aarch64__) || defined(_M_ARM64)
#define ARCH_A64 1
#elif defined(__arm__) || defined(_M_ARM)
#define ARCH_ARM 1
#elif defined(__ppc__) || defined(_M_PPC) || defined(__CELLOS_LV2__)
#define ARCH_PPC 1
#else
//#error "Unknown architecture"
#endif

/**
SIMD defines
*/
#if defined(__i386__) || defined(_M_IX86) || defined(__x86_64__) || defined(_M_X64) || \
        (defined(__EMSCRIPTEN__) && defined(__SSE2__))
#define SIMD_SSE2 1
#endif
#if defined(_M_ARM) || defined(__ARM_NEON__) || defined(__ARM_NEON)
#define SIMD_NEON 1
#endif
#if defined(_M_PPC) || defined(__CELLOS_LV2__)
#define SIMD_VMX 1
#endif

/**
family shortcuts
*/
// compiler
#define COMPILER_FAMILY_GCC (COMPILER_CLANG || COMPILER_GCC)
// os
#define PLATFORM_FAMILY_WINDOWS (PLATFORM_WIN32 || PLATFORM_WIN64 || PLATFORM_UWP)
#define PLATFORM_FAMILY_MICROSOFT (PLATFORM_XBOXONE || PLATFORM_FAMILY_WINDOWS || PLATFORM_XBOX_SERIES_X)
#define PLATFORM_FAMILY_LINUX (PLATFORM_LINUX || PLATFORM_ANDROID)
#define PLATFORM_FAMILY_APPLE (PLATFORM_IOS || PLATFORM_OSX)                  // equivalent to #if __APPLE__
#define PLATFORM_FAMILY_UNIX (PLATFORM_FAMILY_LINUX || PLATFORM_FAMILY_APPLE) // shortcut for unix/posix platforms
#if defined(__EMSCRIPTEN__)
#define PLATFORM_FAMILY_EMSCRIPTEN 1
#else
#define PLATFORM_FAMILY_EMSCRIPTEN 0
#endif
// architecture
#define ARCH_FAMILY_INTEL (ARCH_X64 || ARCH_X86)
#define ARCH_FAMILY_ARM (ARCH_ARM || ARCH_A64)
#define ARCH_FAMILY_P64 (ARCH_X64 || ARCH_A64) // shortcut for 64-bit architectures

/**
Force inline macro
*/
#if COMPILER_VC
#define FORCE_INLINE __forceinline
#elif PLATFORM_LINUX // Workaround; Fedora Core 3 do not agree with force inline and PxcPool
#define FORCE_INLINE inline
#elif FFA_GCC_FAMILY
#define FORCE_INLINE inline __attribute__((always_inline))
#else
#define FORCE_INLINE inline
#endif

#endif //GENERATORS_PREPROCESSOR_H
