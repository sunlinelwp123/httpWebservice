#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include "jsonUser.h"
using namespace std;


int main(){
	ofstream userLog("./logs/user.log", ios::app);
	 time_t timer = time(NULL);
       	if(userLog)userLog<<  ctime(&timer) << "delUser start !"<< endl;
	cJSON *input_json;
	cJSON *output_json = cJSON_CreateObject();
	cJSON *uname_json;
	char  *output_str;
        int length; /*接收数据长度*/
        char *method;/*POST GET方法*/
        char *inputstring; /*数据字符串*/
        method = getenv("REQUEST_METHOD");//将返回赋予method指针
        if(!strcmp(method, "POST")){
                length = atoi(getenv("CONTENT_LENGTH"));
                if(length != 0){
                        inputstring = (char *)malloc(sizeof(char)*length);//stdin不带缓存
                        fread(inputstring, sizeof(char), length, stdin);//从标准输入里读 取一定数据
                }
        }else if(!strcmp(method, "GET")){
                inputstring = getenv("QUERY_STRING");
                length = strlen(inputstring);
        }
        printf("Content-Type:application/json;charset=UTF-8\r\n\r\n");
	input_json = cJSON_Parse(inputstring);
	if (input_json == NULL) userLog << "JsonParseError" << endl;
        if(userLog)userLog<< "request:" << cJSON_Print(input_json) << endl;	
	uname_json = cJSON_GetObjectItemCaseSensitive(input_json,"username");
#if IS_ORACLE
	DBconn *db =new DBconn(ORACLE_URL);
#else	
	DBconn *db =new DBconn();
#endif
	User *p_u = new User(uname_json->valuestring);
	Connection_T conn = db->getConnection();
	if(p_u->delUser(conn))
		cJSON_AddStringToObject(output_json, "ret",  "success");
	else
		cJSON_AddStringToObject(output_json, "ret",  "error");
	output_str = cJSON_Print(output_json);
	printf("%s\r\n", output_str);
	if(userLog) userLog << "response:"<< output_str <<endl;
	cJSON_Delete(input_json);
	cJSON_Delete(output_json);
        return 0;
}
