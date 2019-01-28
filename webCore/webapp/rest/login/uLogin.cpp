#include <iostream>
#include <zdb/zdb.h>
#include <cstdlib>
#include <cstring>
//#include <string>
#include <cjson/cJSON.h>
#include <cstdio>
using namespace std;
/*
 *@AUTHOR LWP
 *@功能:been dao service 实现简例 一般情况是分开写的
	这里为了方便理解 写在一起
 *@table:WS_SYS_USER  对应表名 数据库实例lwp 用户lwp密码lwp (Oracle)
 *	
 */
struct User{
	
	/*username 构造 默认组织机构号为1001*/
	User(const char *user_name,const char *org_id = "1001"/*TODO org_id*/){
		setOrgId(org_id);
		setUserName(user_name);
	}
	/*无参构造*/
	User(){}
	/*深拷贝构造函数*/
	User& operator = (const User& that){
		/*TODO copy user 这里已经声明了新的内存空间 没有指针类型所以直接copy数据*/
		if(this != &that){
			delete this; 	
			strcpy( orgId, that.orgId);
			strcpy( userId, that.userId);
			strcpy( &status, &that.status);
			strcpy( userEmail, that.userEmail);
			strcpy( userWcat, that.userWcat);
			strcpy( userSina, that.userSina);
			strcpy( userPassword, that.userPassword);
			strcpy( userName, that.userName);
			strcpy( userType, that.userType);
			strcpy( userTelno, that.userTelno);
		}
		return *this;
	}
	char getStatus(){
		return status;
	}
	char *getOrgId(){
		return orgId;
	}
	char *getUserName(){
		return userName;
	}
	char *getUserId(){
		return userId;
	}
	char *getUserPassword(){
		return userPassword;
	}
	bool setOrgId(const char *org_id){
		if(strlen(org_id) > 19||strlen(org_id) <= 0){
			clog << "orgId format error" << endl;
			return false;
		}
		memset(orgId, 0, 19*sizeof(char));
		strcpy(orgId, org_id);
		return true;
	}
	bool setUserName(const char *user_name){
		if(strlen(user_name) < 5||strlen(user_name) > 20){
			clog << "userName format error" << endl;
			return false;
		}
		memset(userName, 0, 20*sizeof(char));	
		strcpy(userName, user_name);
		return true;
	}
	bool setUserId(const char *user_id){
		if(strlen(user_id) < 5||strlen(user_id) > 19){
			clog << "userId format error" << endl;
			return false;
		}
		memset(userId, 0, 19*sizeof(char));	
		strcpy(userId, user_id);
		return true;
	}
	bool setUserPassword(const char *user_password){
		if(strlen(user_password) < 5||strlen(user_password) > 20){
			clog << "userPassword format error" << endl;
			return false;
		}
		memset(userPassword, 0, 20*sizeof(char));	
		strcpy(userPassword, user_password);
		return true;
	}
	bool setStatus(const char *user_status){
		status = *user_status;
		return true;
	}
	void printUser(){
		char *printuser[10] = {orgId, userId, &status, userType, userEmail, userWcat, userSina, userName, userPassword, userTelno};
		for(int i = 0; i < 10; i++){
			clog << printuser[i]<< endl;
		}
	}
	/*检查密码暂无加密 TODO 加密?ARS||MD5*/	
	bool checkUserPassword(char *password){
		if(strcmp(userPassword, password))return true;
		return false;
	}
	/*析构函数 delete 会调用 出作用域也会执行*/
	~User(void){
		delete &orgId;
		delete &userId;
		delete &status;
		delete &userType;
		delete &userEmail;
		delete &userWcat;
		delete &userSina;
		delete &userName;
		delete &userPassword;
		delete &userTelno;
	}
private:
	char orgId[19];/*组织机构代码*/
	char userId[19];/*用户id系统生成*/
	char status;/* 用户状态 0-未使用 1-正常 2-注销 3-封停 4冻结*/
	char userType[4];/*用户类型 cat-微信注册用户 sina-新浪注册用户 norm-用户名密码注册用户 mail-邮箱注册 teph-手机注册*/
	char userEmail[50];/*邮箱*/
	char userWcat[50];/*微信号*/
	char userSina[50];/*微博帐号*/
	char userName[20];/*用户名*/
	char userPassword[20];/*密码*/
	char userTelno[20];/*电话*/
};
#if 1
struct UserJson :public User{
	/*TODO USER类的JSON化实例 继承USER*/
};

#endif
#if 1
/* 
 * @dao 层实现 oracle数据库连接 使用zdb连接池支持mysql sqlserver
 * @param user_name 登陆用username字符串
 * @param p_user user对象指针
 */
bool getOneUser(const char *user_name,User *p_user){
	p_user -> setUserName(user_name);
	URL_T url = URL_new("oracle:///LWP?user=lwp&password=lwp");/*或者 url 为"mysql://localhost/test?user=root&password=swordfish" */
	ConnectionPool_T pool = ConnectionPool_new(url);/*设定连接池url并创建，登陆数据库，默认建立5个连接*/
	ConnectionPool_start(pool);/*开始使用该连接池 与stop对应*/
	Connection_T con = ConnectionPool_getConnection(pool);/* 建立数据连接 与close对应*/
	clog << "conn success:" << user_name << endl;
	/*Connection_execute(连接,执行sql，参数.....) */
	/*执行查询sql*/
	ResultSet_T result = Connection_executeQuery(con, "select user_id,user_password,status from WS_SYS_USER where user_name ='%s'",user_name);
	if(ResultSet_isnull(result, 1)){
		/*未找到数据 关闭连接 关闭连接池 释放连接池*/
		Connection_close(con);
		ConnectionPool_stop(pool);
		ConnectionPool_free(&pool);
		return false;
	}
	while (ResultSet_next(result)) {//error <0 nodata=0     
		/*取出数据放入USER中*/
		p_user->setUserId(ResultSet_getString(result, 1));
		p_user->setUserPassword(ResultSet_getString(result, 2));
		p_user->setStatus(ResultSet_getString(result, 3));
		cerr << "id" << p_user->getUserId() << "password" << p_user->getUserPassword() << "status" << p_user->getStatus() << endl;
	}
	Connection_close(con);
	ConnectionPool_stop(pool);
	ConnectionPool_free(&pool);
	return true;
}	
#endif
#if 0 //test
int main(){
	User user;
	if(!getOneUser("alice", &user))return 0;
	printf("%s\n",user.getUserId());
	char * userId =user.getUserId();
	cJSON *json = cJSON_CreateObject();
	if(cJSON_AddStringToObject(json, "userid", userId) == NULL)return 0;
	if(cJSON_AddStringToObject(json, "userpassword", user.getUserPassword()) == NULL)return 0;
	//if(cJSON_AddItemToObject(json,"test", user.getUserPassword())==null)return 0;	
	printf("%s", cJSON_Print(json));
	cJSON_Delete(json);
	return 0;
}

#endif
#if 1
/*
 *@main方法 利用stdin，stdout实现输入和输出 从环境变量获取程序需要的环境变量，继承父进程环境变量
 */
int main(){
	int length; /*接收数据长度*/
	char *method;/*POST GET方法*/
	char *inputstring; /*数据字符串*/

	method = getenv("REQUEST_METHOD");//将返回赋予method指针
	 //printf("method:%s\n", method);
	 //if(method == NULL) return 1;
	if(!strcmp(method, "POST")){
		length = atoi(getenv("CONTENT_LENGTH"));
		//printf("%d", length);
		if(length != 0){
			inputstring = (char *)malloc(sizeof(char)*length);//stdin不带缓存
			fread(inputstring, sizeof(char), length, stdin);//从标准输入里读 取一定数据
			//printf("input%s",inputstring);
		}
	}else if(!strcmp(method, "GET")){
		inputstring = getenv("QUERY_STRING");
		length = strlen(inputstring);
	}
	printf("%S%C%C","Content-Type:application/json;",13,17);
	printf("\r\n");
	cJSON *input_json = cJSON_Parse(inputstring);
	/*string to json   Parse 和下面Create 都创建了malloc内存空间需要用cJSON_Delete删除*/
	if (input_json == NULL)return 0;/*TODO exception 处理 */
	cJSON *output_json = cJSON_CreateObject(); /*json object create*/
	if (output_json == NULL)return 0;
	cJSON *username = NULL;
	username = cJSON_GetObjectItemCaseSensitive(input_json, "username");
	/* char *p_uname = cJSON_Print(username);*/
	/*clog<< "usernamestr: " << cJSON_GetStringValue(username)<<endl;
	cerr<<cJSON_Print(input_json)<<endl;	*/
	User *user = new User;
	if(!getOneUser(cJSON_GetStringValue(username), user)){//这里需要把json对象的string值取出来 否则会带引号
		cJSON_AddStringToObject(output_json,"nameischeck","false");// name = false 用户可用 将数据放入到outputjson结构体中
		cJSON_AddStringToObject(output_json, "userid", user->getUserId());
		cJSON_AddStringToObject(output_json, "pwcheck", "false");	
		
	}else{
		cJSON_AddStringToObject(output_json,"nameischeck","true");// name = true
		cJSON_AddStringToObject(output_json, "userid", user->getUserId());
		cJSON_AddStringToObject(output_json, "pwcheck","false");	
	}
	//printf("Content_Length:%d;\r\n",strlen(cJSON_Print(output_json)));
	printf("%s" ,cJSON_Print(output_json));/*输出json数据需要转成json字符串*/
	cJSON_Delete(input_json);
	cJSON_Delete(output_json);
	delete user;
	printf("\r\n");
	return 0;
}
#endif
