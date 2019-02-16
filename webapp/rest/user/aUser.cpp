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
       	if(userLog)userLog<<  ctime(&timer) << "adduser start !"<< endl;
	cJSON *input_json;
	cJSON *output_json = cJSON_CreateObject();
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
#if IS_ORACLE
	DBconn *db =new DBconn(ORACLE_URL);
#else	
	DBconn *db =new DBconn();
#endif
	JsonUser *p_u = new JsonUser();
	
       	if(userLog)userLog<<  ctime(&timer) << "parseuser start !"<< endl;
	if(!p_u->jsonStringParseUserA(inputstring, input_json)){		
		cJSON_AddStringToObject(output_json, "ret",  "error");
		cJSON_AddStringToObject(output_json, "msg",  "Json Parse Error!");
	}else{
		Connection_T conn = db->getConnection();
		if(userLog)userLog<<  ctime(&timer) << "exist user start !"<< endl;
		if(p_u->userNameExist(conn)){
			cJSON_AddStringToObject(output_json, "ret",  "error");
			cJSON_AddStringToObject(output_json, "msg",  "username already exist!");
		}else{
			if(userLog)userLog<<  ctime(&timer) << "insert user start !"<< endl;
			if(p_u->insertUser(conn)){
				cJSON_AddStringToObject(output_json, "ret",  "success");
				cJSON_AddStringToObject(output_json, "msg",  "Add User Success!");
			}else{
				cJSON_AddStringToObject(output_json, "ret",  "error");
				cJSON_AddStringToObject(output_json, "msg",  "DataBase Connect Error!");
			}
		}
	}
	output_str = cJSON_Print(output_json);
	printf("%s\r\n", output_str);
	if(userLog) userLog << "response:"<< output_str <<endl;
	cJSON_Delete(input_json);
	cJSON_Delete(output_json);
        return 0;
}
