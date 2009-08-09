/*
 * Copyright (C) 2005-2009 MaNGOS <http://getmangos.com/>
 *
 * Copyright (C) 2008-2009 Ribon <http://www.dark-resurrection.de/wowsp/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef RIBON_DEFINE_H
#define RIBON_DEFINE_H

#include <sys/types.h>

#include <ace/Basic_Types.h>
#include <ace/ACE_export.h>

#include "Platform/CompilerDefs.h"

#define RIBON_LITTLEENDIAN 0
#define RIBON_BIGENDIAN    1

#if !defined(RIBON_ENDIAN)
#  if defined (ACE_BIG_ENDIAN)
#    define RIBON_ENDIAN RIBON_BIGENDIAN
#  else //ACE_BYTE_ORDER != ACE_BIG_ENDIAN
#    define RIBON_ENDIAN RIBON_LITTLEENDIAN
#  endif //ACE_BYTE_ORDER
#endif //RIBON_ENDIAN

#if PLATFORM == PLATFORM_WINDOWS
#  define RIBON_EXPORT __declspec(dllexport)
#  define RIBON_LIBRARY_HANDLE HMODULE
#  define RIBON_LOAD_LIBRARY(a) LoadLibrary(a)
#  define RIBON_CLOSE_LIBRARY FreeLibrary
#  define RIBON_GET_PROC_ADDR GetProcAddress
#  define RIBON_IMPORT __cdecl
#  define RIBON_SCRIPT_EXT ".dll"
#  define RIBON_SCRIPT_NAME "RibonScript"
#  define RIBON_PATH_MAX MAX_PATH
#else //PLATFORM != PLATFORM_WINDOWS
#  define RIBON_LIBRARY_HANDLE void*
#  define RIBON_EXPORT export
#  define RIBON_LOAD_LIBRARY(a) dlopen(a,RTLD_NOW)
#  define RIBON_CLOSE_LIBRARY dlclose
#  define RIBON_GET_PROC_ADDR dlsym
#  if defined(__APPLE_CC__) && defined(BIG_ENDIAN)
#    define RIBON_IMPORT __attribute__ ((longcall))
#  elif defined(__x86_64__)
#    define RIBON_IMPORT
#  else
#    define RIBON_IMPORT __attribute__ ((cdecl))
#  endif //__APPLE_CC__ && BIG_ENDIAN
#  if defined(__APPLE_CC__)
#    define RIBON_SCRIPT_EXT ".dylib"
#    if defined(DO_SCRIPTS)
#      define RIBON_SCRIPT_NAME "../lib/libribonscript"
#    else
#      define RIBON_SCRIPT_NAME "../lib/libriboninterface"
#    endif // DO_SCRIPTS
#  else
#    define RIBON_SCRIPT_EXT ".so"
#    if defined(DO_SCRIPTS)
#      define RIBON_SCRIPT_NAME "libribonscript"
#    else
#      define RIBON_SCRIPT_NAME "libriboninterface"
#    endif // DO_SCRIPTS
#  endif //__APPLE_CC__
#  define RIBON_PATH_MAX PATH_MAX
#endif //PLATFORM

#if PLATFORM == PLATFORM_WINDOWS
#  ifdef RIBON_WIN32_DLL_IMPORT
#    define RIBON_DLL_DECL __declspec(dllimport)
#  else //!RIBON_WIN32_DLL_IMPORT
#    ifdef RIBON_WIND_DLL_EXPORT
#      define RIBON_DLL_DECL __declspec(dllexport)
#    else //!RIBON_WIND_DLL_EXPORT
#      define RIBON_DLL_DECL
#    endif //RIBON_WIND_DLL_EXPORT
#  endif //RIBON_WIN32_DLL_IMPORT
#else //PLATFORM != PLATFORM_WINDOWS
#  define RIBON_DLL_DECL
#endif //PLATFORM

#if PLATFORM == PLATFORM_WINDOWS
#  define RIBON_DLL_SPEC __declspec(dllexport)
#  ifndef DECLSPEC_NORETURN
#    define DECLSPEC_NORETURN __declspec(noreturn)
#  endif //DECLSPEC_NORETURN
#else //PLATFORM != PLATFORM_WINDOWS
#  define RIBON_DLL_SPEC
#  define DECLSPEC_NORETURN
#endif //PLATFORM

#if !defined(DEBUG)
#  define RIBON_INLINE inline
#else //DEBUG
#  if !defined(RIBON_DEBUG)
#    define RIBON_DEBUG
#  endif //RIBON_DEBUG
#  define RIBON_INLINE
#endif //!DEBUG

#if COMPILER == COMPILER_GNU
#  define ATTR_NORETURN __attribute__((noreturn))
#  define ATTR_PRINTF(F,V) __attribute__ ((format (printf, F, V)))
#else //COMPILER != COMPILER_GNU
#  define ATTR_NORETURN
#  define ATTR_PRINTF(F,V)
#endif //COMPILER == COMPILER_GNU

typedef ACE_INT64 int64;
typedef ACE_INT32 int32;
typedef ACE_INT16 int16;
typedef ACE_INT8 int8;
typedef ACE_UINT64 uint64;
typedef ACE_UINT32 uint32;
typedef ACE_UINT16 uint16;
typedef ACE_UINT8 uint8;

#if COMPILER != COMPILER_MICROSOFT
typedef uint16      WORD;
typedef uint32      DWORD;
#endif //COMPILER

typedef uint64 OBJECT_HANDLE;

//#define MULTI_THREAD_MAP
#ifdef MULTI_THREAD_MAP
#define MAP_BASED_RAND_GEN
#endif

#define MaNGOS              Ribon
#define MANGOS_DLL_DECL     RIBON_DLL_DECL
#define MANGOS_DLL_SPEC     RIBON_DLL_SPEC
#define GetMangosString     GetRibonString

#if defined(MANGOS_DEBUG) || defined(RIBON_DEBUG)
#  ifndef RIBON_DEBUG
#    define RIBON_DEBUG
#  endif
#  ifndef MANGOS_DEBUG
#    define MANGOS_DEBUG
#  endif
#endif


#endif //RIBON_DEFINE_H

