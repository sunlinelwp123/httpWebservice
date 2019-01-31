#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <list>
#include <deque>
#include <vector>
#include <queue>
#include "jsonUser.h"
using namespace std;


int main(){
	ofstream userLog("./logs/user.log", ios::app);
	 time_t timer = time(NULL);
       	if(userLog)userLog<<  ctime(&timer) << "fUser start !"<< endl;

	cJSON *input_json;
	cJSON *output_json = cJSON_CreateObject();
        cJSON *data = cJSON_AddArrayToObject(output_json, "data");
	int start, pageSize, page, total, totalPage, count = 0;
	char *q_userid, *q_username, *q_usertype, *output_str;
	char c_str[1024]={0};
	char *condition = c_str;
#if 1
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
#endif
	input_json = cJSON_Parse(inputstring);
	/*string to json   Parse 和下面Create 都创建了malloc内存空间需要用cJSON_Delete删除*/
	if (input_json == NULL) userLog << "JsonParseError" << endl;
        if(userLog)userLog<< "request:" << cJSON_Print(input_json) << endl;
	start = cJSON_GetObjectItemCaseSensitive(input_json, "start")->valueint;
	pageSize  = cJSON_GetObjectItemCaseSensitive(input_json, "length")->valueint;
	if(!pageSize)pageSize = 10;
	page = start/pageSize + 1;
	if(cJSON_HasObjectItem(input_json ,"q_userid")){
		q_userid=cJSON_GetObjectItemCaseSensitive(input_json, "q_userid")->valuestring;
		if(strlen(q_userid) >= 1){
			condition = strcat(condition, " and user_id like '%");
			condition = strcat(condition, q_userid);
			condition = strcat(condition ,"%' ");
		}
	}
	if(cJSON_HasObjectItem(input_json ,"q_username")){
		q_username=cJSON_GetObjectItemCaseSensitive(input_json, "q_username")->valuestring;
		if(strlen(q_username) >= 1){
			condition = strcat(condition, " and user_name like '%");
			condition = strcat(condition, q_username);
			condition = strcat(condition ,"%' ");
		}
	}
	if(cJSON_HasObjectItem(input_json ,"q_usertype")){
		q_usertype=cJSON_GetObjectItemCaseSensitive(input_json, "q_usertype")->valuestring;
		if(strlen(q_usertype) >= 1){
			condition = strcat(condition, " and user_type = '");
			condition = strcat(condition, q_usertype);
			condition = strcat(condition ,"' ");
		}
	}
	condition = strcat(condition, " and status <> '0' order by user_id asc ");
	if(userLog)userLog << condition << endl;

#if IS_ORACLE
	DBconn *db =new DBconn(ORACLE_URL);
#else	
	DBconn *db =new DBconn();
#endif
	User *p_u = new JsonUser[pageSize];
	Connection_T conn = db->getConnection();
	if(!getUsersForCondition(p_u, conn, condition, page, pageSize, &total, &totalPage, &count)){
		userLog<< "DB connection error"<< endl;
		printf("%s\r\n", cJSON_Print(output_json));
		return -1;
	}	
	userLog << "page:" << page << "total:" << total << "totalPage:" << totalPage << "count:" << count << endl;
	int i;
	for(i = 0; i < count; i++){
		cJSON * u = cJSON_CreateObject();
		if(((JsonUser *)(p_u+i))->userParseJson(u))cJSON_AddItemToArray(data, u);	
	}
	//printf("Transfer-Encoding: chunked\r\n");
	cJSON_AddNumberToObject(output_json, "recordsTotal",  total);
	cJSON_AddNumberToObject(output_json, "recordsFiltered",  total);

	output_str = cJSON_Print(output_json);
	printf("%s\r\n", output_str);
	//fwrite(content_type, sizeof(char), strlen(content_type), stdout);
	//fwrite(output_str, sizeof(char), strlen(output_str), stdout);
	//fwrite("\r\n",sizeof(char), 2 ,stdout);
	if(userLog) userLog << "response:"<< output_str <<endl;
	cJSON_Delete(input_json);
	cJSON_Delete(output_json);
        return 0;
}
