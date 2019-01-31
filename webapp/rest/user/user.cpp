#include "user.h"
using namespace std;
/*
*@AUTHOR LWP
*@功能:  user实例实现简例实现了部分dao层数据库操作 一般情况是分开写的这里为了方便理解 写在一起
*@table:WS_SYS_USER  对应表名 数据库实例lwp 用户lwp密码lwp (Oracle/mysql)
*	
*/

/*username 构造 默认组织机构号为1001*/
User::User(const char *user_name,const char *org_id/*TODO org_id*/){
	setOrgId(org_id);
	setUserName(user_name);
}
/*无参构造*/
User::User(){}
/*深拷贝构造函数*/
User& User::operator = (const User& that){
	/*TODO copy user 这里已经声明了新的内存空间 没有指针类型所以直接copy数据*/
	if(this != &that){
		cleanUser();
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
char User::getStatus(){
	return status;
}
char * User::getOrgId(){
	return orgId;
}
char * User::getUserName(){
	return userName;
}
char * User::getUserId(){
	return userId;
}
char * User::getUserPassword(){
	return userPassword;
}
bool User::setOrgId(const char *org_id){
	if(strlen(org_id) > 19||strlen(org_id) <= 0){
		clog << "orgId format error" << endl;
		return false;
	}
	memset(orgId, 0, 19*sizeof(char));
	strcpy(orgId, org_id);
	return true;
}
bool User::setUserName(const char *user_name){
	if(strlen(user_name) < 2||strlen(user_name) > 20){
		clog << "userName format error" << endl;
		return false;
	}
	memset(userName, 0, 20*sizeof(char));	
	strcpy(userName, user_name);
	return true;
}
bool User::setUserId(const char *user_id){
	if(strlen(user_id) < 3||strlen(user_id) > 19){
		clog << "userId format error" << endl;
		return false;
	}
	memset(userId, 0, 19*sizeof(char));	
	strcpy(userId, user_id);
	return true;
}
bool User::setUserPassword(const char *user_password){
	if(strlen(user_password) < 5||strlen(user_password) > 20){
		clog << "userPassword format error" << endl;
		return false;
	}
	memset(userPassword, 0, 20*sizeof(char));	
	strcpy(userPassword, user_password);
	return true;
}
bool User::setStatus(const char *user_status){
	status = *user_status;
	return true;
}
void User::printUser(){
	printf("start print User@%p\n", this);
#if 0
	char *printuser[10] = {this->orgId, this->userId, this->userCasue, this->userType, this->userEmail, this->userWcat, this->userSina, this->userName, this->userPassword, this->userTelno};
	//clog <<this->userName<<this->userPassword<<this->userType<<endl;
//	char *printuser[10] = {orgId,userId, userCasue,userType,userEmail,userWcat,userSina,userName,userPassword, userTelno};
	for(int i = 0; i < 10; i++){
		clog << this << "||" << printuser[i]<< endl;
	}
#else 
	clog << this << "@userName:"<< this->userName << endl;
	clog << this << "@userId:"<< this->userId << endl;
	clog << this << "@orgId:"<< this->orgId << endl;
	clog << this << "@userCasue:"<< this->userCasue << endl;
	clog << this << "@userType:"<< this->userType << endl;
	clog << this << "@userEmail:"<< this->userEmail << endl;
	clog << this << "@userWcat:"<< this->userWcat << endl;
	clog << this << "@userSina:"<< this->userSina << endl;
	clog << this << "@userPassword:"<< this->userPassword << endl;
	clog << this << "@userTelno:"<< this->userTelno << endl;
	clog << this << "@status:"<< this->status << endl;
#endif
	printf("end print User@%p\n", this);
}
/*检查密码暂无加密 TODO 加密?ARS||MD5*/	
bool User:: checkUserPassword(char *password){
	if(strcmp(userPassword, password))return true;
	return false;
}
bool User::cleanUser(){
	try{
		memset(orgId, 0, 19*sizeof(char));
		memset(userId, 0, 19*sizeof(char));
		status='0';
		memset(userType, 0, 2*sizeof(char));
		memset(userEmail, 0, 50*sizeof(char));
		memset(userWcat, 0, 50*sizeof(char));
		memset(userSina, 0, 50*sizeof(char));
		memset(userName, 0, 20*sizeof(char));
		memset(userPassword, 0, 20*sizeof(char));
		memset(userTelno, 0, 20*sizeof(char));
		memset(userCasue, 0, 4*sizeof(char));
	}catch(int ex){
		return false;
	}
	return true;
}
/*析构函数 delete 会调用 出作用域也会执行*/
User::~User(void){
	/*
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
	delete &userCasue;
	*/
}
/**
 *
 *删除该用户
 *
 *
 */
bool User::delUser(Connection_T conn){
	if(userName == NULL)return false;
	clog<< "userName"<<userName<<endl;
	try{
		Connection_execute(conn, "update WS_SYS_USER set status = '0' where user_name = '%s'", userName);
		/*Connection_execute(conn, "delete from ws_sys_user where user_name = '%s'", userName); 真删除 */
		Connection_commit(conn);
	}catch(int ex){
		return false;
	}
	return true;
}

/**
 *
 *更新用户数据
 *
 */
bool User::updateUser(Connection_T conn){
	if(userName == NULL)return false;
	try{
		Connection_execute(conn, 
			"update WS_SYS_USER set status = '%s', user_email = '%s', user_wcat='%s', user_sina = '%s', user_password = '%s',user_telno = '%s',user_type = '%s' where user_name = '%s'",
				status, userEmail, userWcat, userSina, userPassword, userTelno, userType, userName);
		 Connection_commit(conn);
	}catch(int ex){
		printf("update error ,ex:%d\n", ex);
		return false;
	}

}
/*
 *
 * 新增用户数据
 *
 */
bool User::insertUser(Connection_T conn){
	if(userName == NULL)return false;
	try{
		Connection_execute(conn,
#if IS_ORACLE 	
				"INSERT INTO WS_SYS_USER "
				"(user_name, org_id, user_id, status,user_casue, user_email, "
				 "user_wcat, user_sina, user_password,user_telno,user_type) "
				"VALUES "
			       	"('%s', '0001', SEQ_WS_SYS_USER_USER_ID.Nextval, '%c', '%s', '%s', '%s', '%s', '%s', '%s', '%s') ",
#else
				"INSERT INTO WS_SYS_USER "
				"(user_name, org_id, user_id, status,user_casue, user_email, "
				 "user_wcat, user_sina, user_password,user_telno,user_type) "
				"VALUES "
			       	"('%s', '0001', (select max(user_id)+1 from WS_SYS_USER a), '%c', '%s', '%s', '%s', '%s', '%s', '%s', '%s') ",
#endif	
      				userName, status, userCasue, userEmail, userWcat, userSina, userPassword, userTelno, userType);
		printf("%s :insert end\n", userName);
		Connection_commit(conn);
	}catch(int ex){
		printf("insert error ,ex:%d\n", ex);
		return false;
	}
}

/*
 *
 * 使用name 从数据库读取数据到User对象
 *
 *
 */
bool User::initUserUseUserNameFromDB(Connection_T conn){
	if(userName == NULL)return false;
	ResultSet_T result = Connection_executeQuery(conn,
		  "select org_id,user_id,status,user_type,user_email,user_wcat,user_sina,user_password,user_telno,user_casue,user_name from WS_SYS_USER where user_name ='%s'",
		  userName);
	if(ResultSet_isnull(result, 1)){
		/*未找到数据 关闭连接 关闭连接池 释放连接池*/
		Connection_close(conn);
		return false;
	}
	if(ResultSet_next(result)) {
		if(cleanUser()){
			/* or   strcpy(orgId, ResultSet_getStringByName(result, "org_id")*/
			strcpy(orgId, ResultSet_getString(result, 1)==NULL?"":ResultSet_getString(result, 1));
			strcpy(userId, ResultSet_getString(result, 2)==NULL?"":ResultSet_getString(result, 2));
			strcpy(&status, ResultSet_getString(result, 3)==NULL?"":ResultSet_getString(result, 3));
			strcpy(userType, ResultSet_getString(result, 4)==NULL?"":ResultSet_getString(result, 4));
			strcpy(userEmail, ResultSet_getString(result, 5)==NULL?"":ResultSet_getString(result, 5));
			strcpy(userWcat, ResultSet_getString(result, 6)==NULL?"":ResultSet_getString(result, 6));
			strcpy(userSina, ResultSet_getString(result, 7)==NULL?"":ResultSet_getString(result, 7));
			strcpy(userPassword, ResultSet_getString(result, 8)==NULL?"":ResultSet_getString(result, 8));
			strcpy(userTelno, ResultSet_getString(result, 9)==NULL?"":ResultSet_getString(result, 9));
			strcpy(userCasue, ResultSet_getString(result, 10)==NULL?"":ResultSet_getString(result, 10));
			strcpy(userName, ResultSet_getString(result, 11)==NULL?"":ResultSet_getString(result, 11));
		/*	strcpy(userCasue, ResultSet_getStringByName(result, "user_casue")==NULL?"":ResultSet_getStringByName(result, "user_casue"));*/
		/*	printf("userCasue:%s\n",userCasue);
			clog<<userName<<orgId<<userId<<userPassword <<endl;*/
		}else{
			Connection_close(conn);
			return false;
		}
	}else{
		Connection_close(conn);
		return false;
	}
	return true;
}

bool getUsersForCondition(User *user, Connection_T conn,const char *condition, int page, int size, int *p_total, int *p_totalPage, int *p_count){
	
        ResultSet_T result;
	char sql[1024] = "select org_id,user_id,status,user_type,user_email,user_wcat,user_sina,user_password,user_telno,user_casue,user_name from WS_SYS_USER where 1= 1 ";
	sprintf((sql + strlen(sql)), "%s", condition ? condition : "");
	clog<< sql<<endl;
 	page_util(conn, sql, 0, page, size, p_total, p_totalPage, &result);
	/*page_util(con, sql1, 0, 2, 20 ,&total, &totalPage, &r);
	ResultSet_T result = Connection_executeQuery(conn,
		  "select org_id,user_id,status,user_type,user_email,user_wcat,user_sina,user_password,user_telno,user_casue,user_name from WS_SYS_USER where 1= 1 %s",
		  condition==NULL?"":condition); 
	*/
	if(ResultSet_isnull(result, 1)){
		/*未找到数据 关闭连接 关闭连接池 释放连接池*/
		Connection_close(conn);
		return false;
	}
	while(ResultSet_next(result)) {
		if(user->cleanUser()){
			/* or   strcpy(orgId, ResultSet_getStringByName(result, "org_id")*/
			user->setOrgId(ResultSet_getString(result, 1)==NULL?"":ResultSet_getString(result, 1));
			user->setUserId(ResultSet_getString(result, 2)==NULL?"":ResultSet_getString(result, 2));
			strcpy(&user->status, ResultSet_getString(result, 3)==NULL?"":ResultSet_getString(result, 3));
			strcpy(user->userType, ResultSet_getString(result, 4)==NULL?"":ResultSet_getString(result, 4));
			strcpy(user->userEmail, ResultSet_getString(result, 5)==NULL?"":ResultSet_getString(result, 5));
			strcpy(user->userWcat, ResultSet_getString(result, 6)==NULL?"":ResultSet_getString(result, 6));
			strcpy(user->userSina, ResultSet_getString(result, 7)==NULL?"":ResultSet_getString(result, 7));
			strcpy(user->userPassword, ResultSet_getString(result, 8)==NULL?"":ResultSet_getString(result, 8));
			strcpy(user->userTelno, ResultSet_getString(result, 9)==NULL?"":ResultSet_getString(result, 9));
			strcpy(user->userCasue, ResultSet_getString(result, 10)==NULL?"":ResultSet_getString(result, 10));
			user->setUserName(ResultSet_getString(result, 11)==NULL?"":ResultSet_getString(result, 11));
		}else{
			Connection_close(conn);
			return false;
		}
		(*p_count)++;
		user++;
	}
	Connection_close(conn);
	return true;
}
