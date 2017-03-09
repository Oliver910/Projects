#ifndef _CHTBASE64_H_
#define _CHTBASE64_H_

/*
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
*/

//#include "common.h"

/*base64解碼*/
int ht_b64_decode( const char* str, unsigned char* space, int size );

/*base64編碼，使用結果需要delete*/
char* ht_b64_encode(char const* origSigned, unsigned origLength) ;

#endif
