#include <stdio.h>
#include <ctype.h>


int main (){
	char *str="abcAAA";

	char b[10];
	char *p_b=b;
		while(*str){
			*p_b++ = toupper(*str++);
		}
	printf("%s", b);
	return 0;

}
