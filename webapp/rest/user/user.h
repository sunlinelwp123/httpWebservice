#ifndef __USER_H__
#define __USER_H__
#include <t_param.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <dbconn.h>
#include <page.h>
using namespace std;
/*
 *@AUTHOR LWP
 *@功能:WS_SYS_USER数据库实现 一般情况是分开写的
	这里为了方便理解 写在一起
 *@table:WS_SYS_USER  对应表名 数据库实例lwp 用户lwp密码lwp (Oracle): "oracle:///LWP?user=lwp&password=lwp"
 *	(mysql): "mysql://172.40.83.126/three?user=root&password=tarena"
 */

struct User{
	User(const char *user_name,const char *org_id = "1001"/*TODO org_id*/);
	User& operator = (const User& that);
	User();
	~User(void);
	bool cleanUser();
	char getStatus();
	char *getOrgId();
	char *getUserName();
	char *getUserId();
	char *getUserPassword();
	char *getUserType();
	bool setUserType(const char *user_type);
	bool setOrgId(const char *org_id);
	bool setUserName(const char *user_name);
	bool setUserId(const char *user_id);
	bool setUserPassword(const char *user_password);
	bool setStatus(const char *user_status);
	virtual void printUser();
	/*检查密码暂无加密 TODO 加密?ARS||MD5*/
	virtual bool checkUserPassword(char *password);
	virtual bool initUserUseUserNameFromDB(Connection_T conn);/* 通过数据库连接池初始化user对象*/
 	bool userNameExist(Connection_T conn);
	virtual bool delUser(Connection_T conn);/*shan chu yonghu*/
	virtual bool updateUser(Connection_T conn);/*update*/
	virtual bool insertUser(Connection_T conn);/*insert */
	public: 
	char status;/* 用户状态 0-未使用 1-正常 2-注销 3-封停 4冻结*/
	char userCasue[4];/*用户类型 cat-微信注册用户 sin-新浪注册用户 nor-用户名密码注册用户 mai-邮箱注册 tel-手机注册*/
	char userEmail[50];/*邮箱*/
	char userWcat[50];/*微信号*/
	char userSina[50];/*微博帐号*/
	char userPassword[20];/*密码*/
	char userTelno[20];/*电话*/
	char userType[2];/*t:teacher;s:student;a:admin*/
	private :
	char userId[19];/*用户id系统生成*/	
	char orgId[19];/*组织机构代码*/
	char userName[20];/*用户名*/
};
/*  User * user pointer 
 *  char * condition ex:" or user_name = 'Alice' or org_id = '1001' order by user_id asc "
 *  return  ret < 0 error
 *          ret = 0 notfound
 *          ret > 0 result size
 */
bool getUsersForCondition(User *user, Connection_T conn, const char *condition, int page, int size, int *p_total, int *p_totalPage, int *p_count = 0);
	
#endif/* __USER_H__*/
