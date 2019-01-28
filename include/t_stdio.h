#ifndef __T_STDIO_H__
#define __T_STDIO_H__
#include <stdio.h>
#define E_MSG(str ,val) do{perror(str);return val;}while(0)
#endif
