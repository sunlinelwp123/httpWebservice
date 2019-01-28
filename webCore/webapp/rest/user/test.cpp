/*#include "user.h"*/
#include "jsonUser.h"
#include <iostream>
using namespace std;

int main () {
#if IS_ORACLE
	DBconn *db =new DBconn(ORACLE_URL);
#else	
	DBconn *db =new DBconn();
#endif
	User *user = new User("Alice");	
#if 0
	/*通过username 从数据库取数据并user成员变量 */
	Connection_T con = db->getConnection();
	user->initUserUseUserNameFromDB(con);
	user->printUser();
#endif
	
	int pageSize = 0;
	if(!pageSize)pageSize = 10;
	User *p_u = new User[pageSize];
	Connection_T conn = db->getConnection();
	/*User *user, Connection_T conn, char *condition, int page, int size, int *p_total, int *p_totalPage,int *p_count*/
	int total, totalPage, count = 0;
	printf("user:%p\n", p_u);
	if(!getUsersForCondition(p_u, conn, "and status <> '0' order by user_id asc", 1, pageSize, &total, &totalPage, &count))return -1;	
	printf("total:%d totalPage:%d count%d\n", total, totalPage, count);
	int i = 0;	
	for(i = 0; i < count; i++){
		(p_u + i)->printUser();
	}
/*
	if((p_u+1)->delUser(conn)) 
	 	 printf("user@%s deleted", (p_u + 1)->getUserName());
*/	
#if 0
	/*student insert batch*/
	for(i = 0; i < 100; i++){
		char s[15]= "student";
		sprintf((s+7), "%d", i);

		char e[50]= "1234567890@";
		sprintf(e+11, "%d", i);

		char w[50]= "wcat_12345@";
                sprintf(w+11, "%d", i);
		
		char a[50]="sina_12345@";
		sprintf(a+11. "%d", i);

		char t[20]= "1111111111";
                sprintf(t+10, "%d", i);

		User *user1 = new User(s);
		user1->status = '1';
		strcpy(user1->userCasue,"nor");
		strcpy(user1->userEmail, e);
		strcpy(user1->userWcat, w);
		strcpy(user1->userSina, a);
		strcpy(user1->userPassword, "123456");
		strcpy(user1->userType, "s");
		strcpy(user1->userTelno, t);
		user1->insertUser(conn);
	}
#endif
#if 0
	/*teather insert batch*/
	for(i = 0; i < 100; i++){
		char s[15]= "teather";
		sprintf((s+7), "%d", i);

		char e[50]= "9876543211@";
		sprintf(e+11, "%d", i);

		char w[50]= "wcat_54321@";
                sprintf(w+11, "%d", i);

		char t[20]= "139876766";
                sprintf(t+10, "%d", i);

		char a[50]="sina_12345@";
		sprintf(a+11. "%d", i);

		User *user1 = new User(s);
		user1->status = '1';
		strcpy(user1->userCasue,"nor");
		strcpy(user1->userEmail, e);
		strcpy(user1->userWcat, w);
		strcpy(user1->userSina, a);
		strcpy(user1->userPassword, "123456");
		strcpy(user1->userType, "t");
		strcpy(user1->userTelno, t);
		user1->insertUser(conn);
	}
#endif
#if 1
	clog << "****************JSONTEST  START*****************"<<endl;
	JsonUser *juser =new JsonUser();
/*	char userstr[512] = "{ \"username\" : \"00012\",\"userid\":\"10016\",\"orgId\":\"0001\","
			 "\"userCasue\":\"nor\",\"userType\":\"t\",\"userEmail\":\"9876543211@12\","
			 "\"userWcat\":\"wcat_54321@12\",\"userSina\":\"wcat_54321@12\","
			 "\"userPassword\":\"123456\",\"userTelno\":\"139876766\",\"status\":'1'}";
*/
	char userstr[512]= "{"
			"\"username\": \"steather12\","
			"\"userid\": \"0004\","
			"\"orgid\": \"0001\","
			"\"usercasue\": \"nor\","
			"\"usertype\": \"t\","
			"\"useremail\": \"9876543211@12\","
			"\"userwcat\": \"wcat_54321@12\","
			"\"usersina\": \"wcat_54321@12\","
			"\"userpassword\": \"123456\","
			"\"usertelno\": \"139876766\","
			"\"status\": \"1\"}";
	if(juser->jsonStringParseUser(userstr))juser->printUser();
	/*juser->insertUser(conn);*/
	JsonUser *p_user = new JsonUser("Alice");
	if(p_user->initUserUseUserNameFromDB(conn)){
		p_user->userParseJson();
  		p_user->printUser();
	}else clog<< "12312"<<endl;
	clog << "****************JSONTEST  END*****************"<<endl;
#endif
	delete [] p_u;
	Connection_close(conn);
	return 0;
}
