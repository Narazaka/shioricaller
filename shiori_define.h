#ifndef SHIORI_DEFINE_H__
#define SHIORI_DEFINE_H__
//-------------------------------------------------------------------------
#if defined(WIN32)||defined(_WIN32)||defined(_Windows)||defined(__CYGWIN__)
// Win32
#	include <windows.h>
#	if defined(__BORLANDC__)||defined(__clang__)
// 		Borland C++ / clang
#		ifdef __cplusplus
#			define SHIORI_EXPORT	extern "C"
#		else
#			define SHIORI_EXPORT	extern
#		endif
#	else
// 		Visual C++ / Cygwin32 / Mingw32
#		ifdef __cplusplus
#			define SHIORI_EXPORT	extern "C" __declspec(dllexport)
#		else
#			define SHIORI_EXPORT	extern __declspec(dllexport)
#		endif
#	endif
#	define SHIORI_CALL			__cdecl
#	define MEMORY_HANDLE		HGLOBAL
#	ifdef __cplusplus
#		define SHIORI_MALLOC(len)	::GlobalAlloc(GMEM_FIXED, len)
#		define SHIORI_FREE(ptr)		::GlobalFree((HGLOBAL)ptr)
#	else
#		define SHIORI_MALLOC(len)	GlobalAlloc(GMEM_FIXED, len)
#		define SHIORI_FREE(ptr)		GlobalFree((HGLOBAL)ptr)
#	endif
#else
// Other Platform
#	ifdef __cplusplus
#		define SHIORI_EXPORT	extern "C"
#	else
#		define SHIORI_EXPORT	extern
#	endif
#	define SHIORI_CALL
#	define MEMORY_HANDLE		char *
#	define SHIORI_MALLOC(len)	malloc(len)
#	define SHIORI_FREE(ptr)		free((void *)ptr)
#endif
#ifndef BOOL
#	define BOOL					int
#	define TRUE					1
#	define FALSE				0
#endif

#endif // SHIORI_DEFINE_H__
