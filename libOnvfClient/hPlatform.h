#ifndef _HPLATFORM_H_
#define _HPLATFORM_H_

#ifdef BUILDING_ON_MSVC			// whan building on MSVC, BUILDING_ON_MSVC should be defined on complier's parameters 
#ifdef LIBIPCAM_EXPORTS
#define LIBIPCAM_API 	extern "C" __declspec(dllexport)
#define LIBIPCAM_CLASS  __declspec(dllexport)
#else
#define LIBIPCAM_API 	extern "C" __declspec(dllimport)
#define LIBIPCAM_CLASS  __declspec(dllimport)
#endif
#else
#define LIBIPCAM_API 	extern "C"
#define LIBIPCAM_CLASS
//__attribute__ ( (aligned(2)) );
//__attribute__ ( (__packed__))
//#define PACK_ALIGNED_PUSH
//#define PACK_ALIGNED_POP
#endif





#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef unsigned char      HT_U8;
typedef unsigned short     HT_U16;
typedef unsigned int       HT_U32;
typedef unsigned long long HT_U64;

typedef char      HT_S8;
typedef short     HT_S16;
typedef int       HT_S32;
typedef long long HT_S64;



//typedef HT_U32 WORD;
//typedef HT_U64 DWORD;
typedef HT_U8    BYTE;
//typedef HT_U16   WORD;
//typedef HT_U32   DWORD;
typedef HT_U64   QWORD;
typedef HT_S64   LONGLONG ;
typedef HT_S32   LONG ;




//轉換使用windows變數
typedef HT_U32 SIZE_T;
typedef HT_U8  BYTE;

//特別定義
#ifndef	FALSE
#define FALSE  (0)
#define TRUE   !(FALSE)
#endif


//#include "libIPcamTypeDef.h"


#endif				// end of #ifndef _HPLATFORM_H_

