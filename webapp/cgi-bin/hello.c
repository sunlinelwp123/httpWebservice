#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int main(int argc,char *argv[]){
	int length;
	char *method;
	char *inputstring;
	printf("cgi-start\n");
	method = getenv("REQUEST_METHOD");//将返回赋予method指针
	printf("method:%s\n", method);
	if(method == NULL) return 1;
	if(!strcmp(method, "POST")){
		length = atoi(getenv("CONTENT_LENGTH"));
		if(length !=0){
			inputstring = malloc(sizeof(char)*length);//stdin不带缓存
			fread(inputstring, sizeof(char), length, stdin);//从标准输入里读取一定数据
		}
	}else if(!strcmp(method, "GET")){
		inputstring =getenv("QUERY_STRING");
		length = strlen(inputstring);
	}
	printf("%S%C%C","Content-Type:text/html;charset=UTF-8", 13, 10);
	printf("<title><title>");
	printf("<h3> 111111</h3>");	
	if(length == 0)return 0;
}

