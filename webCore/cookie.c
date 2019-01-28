/*@AUTH LWP
 *@ACHIEVE COOKIE STRUCT
 *@SET SESSION
 * HTTP_COOKIE="tm=145674532:username=Alice:sessionid=e10adc3949ba59abbe56e057f20f883e:....."
 **/
#include "cookie.h"
/*
typedef struct CK {
	long tm; 
	char *username;
	char *sessionid;
	etc ....
} cookie;
*/
int cookieSet(){
	char tmbuf[19];
        char username[19];
	char sessionid[49];
	char *cookieStr = getenv("HTTP_COOKIE");
	int cookieSize = strlen(cookieStr);
	printf("size:%d\n", cookieSize);
	readCookie(cookieStr, cookieSize);
	return 0;
 }

int readCookie(char * cookieStr, int cookieSize){
	char envName[COOKIENAMESIZE]= "";
	char envValue[COOKIEVALUESIZE]="";
	int i=0,flag=0;
	char *p_name =envName;
	char *p_value=envValue;	
 	for(i=0;i< 2;i++){
                while(*cookieStr){
                        if(!cookieSize)break;
                        if(!flag){
				*p_name++ = toupper(*cookieStr++);
			}else{
				*p_value++ = *cookieStr++;
			}
                        cookieSize--;        
                        if(*cookieStr == COOKIESIGN){
                                flag = 1;
				break;
                        }
                        if(*cookieStr == COOKIESPLIT){ 
				flag = 0;
				break;
                        }
                }
		cookieStr++;
		cookieSize--;
        }
	printf("envname:%s,envvalue:%s\n",envName,envValue);
	setenv(envName, envValue, 0);	
	printf("size:%d\n", cookieSize);
	if(cookieSize > 1){
		readCookie(cookieStr, cookieSize);
	}
	return 0;
}
#if 0
void main(){
	cookieSet();
	printf("sessionid:%s, tm:%s, username:%s\n",
			getenv("SESSIONID"),
			getenv("TM"),
			getenv("USERNAME")
			);
}
#endif

