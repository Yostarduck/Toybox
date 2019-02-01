#pragma once

#include <cassert>
#include <tb_defines.h>
#include <tb_types.h>

/**********************************************************************
*															                                  		  *
*					                  Windows Specifics							            *
*																	                                    *
**********************************************************************/

#if TB_PLATFORM == TB_PLATFORM_WINDOWS
# if defined(TB_STATIC_LIB)
#   define TB_GRAPHICS_EXPORT
# else
#	if defined(GRAPHICS_EXPORTS)
#   define TB_GRAPHICS_EXPORT __declspec(dllexport)
#   else
#     if defined(__MINGW32__)
#       define TB_GRAPHICS_EXPORT              //Linux systems don't need this
#     else
#       define TB_GRAPHICS_EXPORT __declspec(dllimport)
#     endif
#   endif
# endif
#endif

/**********************************************************************
*																	                                    *
*                   	    	  Linux Specifics							            *
*																	                                    *
**********************************************************************/

#if TB_PLATFORM == TB_PLATFORM_LINUX || TB_PLATFORM == TB_PLATFORM_OSX
//Enable GCC symbol visibility
# if defined( TB_GCC_VISIBILITY )
#   define TB_GRAPHICS_EXPORT  __attribute__ ((visibility("default")))
# else
#   define TB_GRAPHICS_EXPORT
# endif

# if TB_COMPILER == TB_COMPILER_INTEL
#   define TB_THREADLOCAL __thread          //Set the local thread for the Intel compiler
# endif
#endif
