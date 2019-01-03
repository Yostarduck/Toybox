#pragma once

#define TB_PLATFORM_WINDOWS	  1   //Windows Platform
#define TB_PLATFORM_LINUX	    2	  //Linux Platform
#define TB_PLATFORM_OSX		    3	  //Mac Platfomr
#define TB_PLATFORM_IOS		    4	  //iPhone Platform
#define TB_PLATFORM_ANDROID	  5	  //Android Platform
#define TB_PLATFORM_PS4		    6	  //Play Station 4 Platform

#define TB_COMPILER_MSVC  1   //VS Compiler
#define TB_COMPILER_GNU   2   //GCC Compiler
#define TB_COMPILER_INTEL 3   //Intel Compiler
#define TB_COMPILER_CLANG 4   //Clang Compiler

#define TB_ARCHITECTURE_X86_32 1
#define TB_ARCHITECTURE_X86_64 2

#define TB_ENDIAN_LITTLE  1	        //Little endinan
#define TB_ENDIAN_BIG	    2	        //Big endian

#define TB_ENDINAN TB_ENDIAN_LITTLE

/**********************************************************************
*																	                                    *
*                             Debug Macros							              *
*																	                                    *
**********************************************************************/

//If this is a debug build
#if defined(_DEBUG) || defined(DEBUG)
# define TB_DEBUG_MODE 1			  //Debug mode is on
#else
# define TB_DEBUG_MODE 0			  //Debug mode is off
#endif

#if TB_DEBUG_MODE
# define TB_ASSERT(x) assert(x)
# define TB_DEBUG_ONLY(x) x
# define TB_DEBUG_ONLY_PARAM(x) ,x
#else
# define TB_ASSERT(x)
# define TB_DEBUG_ONLY(x)
# define TB_DEBUG_ONLY_PARAM(x)
#endif

/**********************************************************************
*																	                                    *
*						                  Compiler									              *
*																	                                    *
**********************************************************************/

#if defined(_MSC_VER)                       //Visual Studio
# define TB_COMPILER TB_COMPILER_MSVC         //Set as Actual Compiler
# define TB_COMP_VER _MSC_VER                 //Compiler version
# define TB_THREADLOCAL __declspec(thread)    //Local Thread type
#elif defined(__GNUC__)                     //GCC Compiler
# define TB_COMPILER TB_COMPILER_GNU		      //Set as Actual Compiler
//Compiler version (computed from integrated defines)
# define TB_COMP_VER (((__GNUC__)*100) + (__GNUC_MINOR__*10) + __GNUC_PATCHLEVEL__)
# define TB_THREADLOCAL __thread              //Local Thread type
#elif defined (__INTEL_COMPILER)            //Intel compiler
# define TB_COMPILER TB_COMPILER_INTEL        //Set as Actual Compiler
# define TB_COMP_VER __INTEL_COMPILER         //Compiler version
/**
 * TB_THREADLOCAL define is down below because Intel compiler defines it
 * differently based on platform
 */
#elif defined (__clang__)                   //Clang compiler
# define TB_COMPILER TB_COMPILER_CLANG        //Set as Actual Compiler
# define TB_COMP_VER __clang_version__        //Compiler version
# define TB_THREADLOCAL __thread              //Local Thread type
#else
//No know compiler found, send the error to the output (if any)
# pragma error "No known compiler. "
#endif

/**********************************************************************
*                                 																	  *
*                           Current Platform							            *
*                       					          												  *
**********************************************************************/

#if defined(_WIN32) || defined(__WIN32__)	  //Windows OS
# define TB_PLATFORM TB_PLATFORM_WINDOWS
#elif defined(__APPLE__)					  //Apple Platform
# if defined(__MACH__)						  //OSX Platform
#	define TB_PLATFORM TB_PLATFORM_OSX
# else
#	define TB_PLATFORM TB_PLATFORM_IOS		  //IOS Platform
#endif
#elif defined(__ANDROID__)					  //Android Platform
# define TB_PLATFORM TB_PLATFORM_ANDROID
#elif defined(__ORBIS__)					  //PS4 Platform
# define TB_PLATFORM TB_PLATFORM_PS4
#else										  //Linux Platform
# define TB_PLATFORM TB_PLATFORM_LINUX
#endif

/**********************************************************************
*																	                                    *
*					              Force inline & Restrict						            *
*																	                                    *
**********************************************************************/

#if TB_COMPILER == TB_COMPILER_MSVC           //If we are compiling on Visual Studio
# if TB_COMP_VER >= 1200                      //If we are on Visual Studio 6 or higher
#   define FORCEINLINE __forceinline          //Set __forceinline
#   ifndef RESTRICT
#     define RESTRICT __restrict              //No alias hint
#   endif
# endif
#elif defined(__MINGW32__)                    //If we are on a Unix type system
# if !defined(FORCEINLINE)
#   define FORCEINLINE __inline               //Set __inline
#   ifndef RESTRICT
#     define RESTRICT                         //No alias hint
#   endif
# endif
#else                                         //Any other compiler
# define FORCEINLINE __inline                 //Set __inline
# ifndef RESTRICT
#   define RESTRICT __restrict                //No alias hint
# endif
#endif

/**********************************************************************
*																	                                    *
*							                Architecture							              *
*																	                                    *
**********************************************************************/

#if defined(__x86_64__) || defined(_M_X64)    //If this is a x64 compile
# define TB_ARCH_TYPE TB_ARCHITECTURE_X86_64
#else                                         //If it's a x86 compile
# define TB_ARCH_TYPE TB_ARCHITECTURE_X86_32
#endif

#if (TB_COMPILER == TB_COMPILER_MSVC)
#pragma warning(disable : 4201)
#pragma warning(disable : 4251)
#pragma warning(disable : 4275)
#pragma warning(disable : 4099)
#endif