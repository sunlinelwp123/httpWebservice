#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>

int main(int argc,char *argv[]){
//	char *inputstring = "{\"name\":\"laowang\"}";
	char *inputstring = "{"
         "\"username\": \"teather12\","
	 "\"userid\": \"10016\","
         "\"orgId\": \"0001\","
         "\"userCasue\": \"nor\","
         "\"userType\": \"t\","
         "\"userEmail\": \"9876543211@12\","
         "\"userWcat\": \"wcat_54321@12\","
         "\"userSina\": \"wcat_54321@12\","
         "\"userPassword\": \"123456\","
         "\"userTelno\": \"139876766\","
         "\"status\": \"1\"}";	
	cJSON *monitor_json = cJSON_Parse(inputstring);
	printf("%s\n" ,cJSON_Print(monitor_json));
	const cJSON *username = NULL;
	username = cJSON_GetObjectItemCaseSensitive(monitor_json, "username");
	if (cJSON_AddStringToObject(monitor_json, "sex", "男") == NULL){
        	exit(0);
   	}
	printf("Content_Length:%d;",strlen(cJSON_Print(monitor_json)));
	printf("Content:%s" ,cJSON_Print(monitor_json));
	cJSON_Delete(monitor_json);
	printf("\r\n\r\n");
	return 0;
}

