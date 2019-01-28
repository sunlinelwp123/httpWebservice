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
       	if(userLog)userLog<<  ctime(&timer) << "Select User start !"<< endl;
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
#endif
	cJSON *input_json = cJSON_Parse(inputstring);
	/*string to json   Parse 和下面Create 都创建了malloc内存空间需要用cJSON_Delete删除*/
	if (input_json == NULL) userLog << "JsonParseError" << endl;
        if(userLog)userLog<<"request:" << cJSON_Print(input_json) << endl;
	int start = cJSON_GetObjectItemCaseSensitive(input_json, "start")->valueint;
	int pageSize  = cJSON_GetObjectItemCaseSensitive(input_json, "length")->valueint;
	if(!pageSize)pageSize = 10;
	int page = start/pageSize + 1;
#if IS_ORACLE
	DBconn *db =new DBconn(ORACLE_URL);
#else	
	DBconn *db =new DBconn();
#endif
	//User *p_u = new User[pageSize];
	User *p_u = new JsonUser[pageSize];
	Connection_T conn = db->getConnection();
	int total, totalPage, count = 0;
	if(!getUsersForCondition(p_u, conn, "and status <> '0' order by user_id asc", page, pageSize, &total, &totalPage, &count))return -1;	
	userLog << "page:" << page << "total:" << total << "totalPage:" << totalPage << "count:" << count << endl;
	int i = 0;
	cJSON *output_json = cJSON_CreateObject(); 
	cJSON *data = cJSON_AddArrayToObject(output_json, "data");
	for(i = 0; i < count; i++){
		cJSON * u = cJSON_CreateObject();
		if(((JsonUser *)(p_u+i))->userParseJson(u))cJSON_AddItemToArray(data, u);	
	}
        printf("Content-Type:application/json;charset=UTF-8\r\n\r\n");
	//printf("Transfer-Encoding: chunked\r\n");
	cJSON_AddNumberToObject(output_json, "recordsTotal",  total);
	cJSON_AddNumberToObject(output_json, "recordsFiltered",  total);
	char *str = cJSON_Print(output_json);
	printf("%s\r\n", cJSON_Print(output_json));
	if(userLog) userLog << "response:"<< str <<endl;
	cJSON_Delete(input_json);
	cJSON_Delete(output_json);
        return 0;
}
