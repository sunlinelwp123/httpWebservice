#ifndef __COOKIE_H__
#define __COOKIE_H__

/*@AUTH LWP
 *@ACHIEVE COOKIE STRUCT
 *@SET SESSION
 **/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
/*
#ifndef COOKIE_SESSIONID
#define COOKIE_SESSIONID
#endif

#ifndef COOKIE_USERNAME
#define COOKIE_USERNAME
#endif

#ifndef COOKIE_TM
#define COOKIE_TM
#endif
*/
#ifndef COOKIESIZE
#define COOKIESIZE 255
#endif

#ifndef COOKIENAMESIZE
#define COOKIENAMESIZE 10
#endif

#ifndef COOKIEVALUESIZE
#define COOKIEVALUESIZE 50
#endif

#ifndef COOKIESPLIT
#define COOKIESPLIT ':'
#endif

#ifndef COOKIESIGN
#define COOKIESIGN '='
#endif


#ifndef SESSIONTIMELIMIT
#define SESSIONTIMELIMIT 120
#endif
/*
typedef struct CK {
	long tm; 
	char *username;
	char *sessionid;
	etc ....
} cookie;
*/
int cookieSet();
int readCookie(char *, int);

#endif
