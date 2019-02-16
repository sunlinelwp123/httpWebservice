#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../include/cjson/cJSON.h"

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
		//printf("%d", length);
		if(length !=0){
			inputstring = malloc(sizeof(char)*length);//stdin不带缓存
			fread(inputstring, sizeof(char), length, stdin);//从标准输入里读取一定数据
			//printf("input%s",inputstring);
		}
	}else if(!strcmp(method, "GET")){
		inputstring =getenv("QUERY_STRING");
		length = strlen(inputstring);
	}
	printf("%S%C%C","Content-Type:application/json;", 13, 17);
	printf("\r\n");

	cJSON *monitor_json = cJSON_Parse(inputstring);
	const cJSON *username = NULL;
	username = cJSON_GetObjectItemCaseSensitive(monitor_json, "username");
	if (cJSON_AddStringToObject(monitor_json, "sex", "男") == NULL){
        	exit(0);
   	}
	//printf("Content_Length:%d;\r\n",strlen(cJSON_Print(monitor_json)));
	printf("%s" ,cJSON_Print(monitor_json));
	cJSON_Delete(monitor_json);
	printf("\r\n\r\n");
	if(length == 0)return 0;
	return 0;
}






