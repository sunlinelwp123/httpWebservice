#include "dbconn.h"


DBconn::DBconn(const char *url_str){
		url = URL_new(url_str);
		pool = ConnectionPool_new(url);/*设定连接池url并创建，登陆数据库，默认建立5个连接*/
		ConnectionPool_start(pool);/*开始使用该连接池 与stop对应*/
		//Connection_T con = ConnectionPool_getConnection(pool);/* 建立数据连接 与close对应*/
	}
DBconn::DBconn(){
		url = URL_new("mysql://172.40.83.126/three?user=root&password=tarena");
		pool = ConnectionPool_new(url);/*设定连接池url并创建，登陆数据库，默认建立5个连接*/
		ConnectionPool_start(pool);/*开始使用该连接池 与stop对应*/
}
DBconn::~DBconn(void){
		//Connection_close(con);
		ConnectionPool_stop(pool);
		ConnectionPool_free(&pool);
	}
Connection_T DBconn::getConnection(){
		return ConnectionPool_getConnection(pool);
	}
void DBconn::connClose(Connection_T conn){
		 Connection_close(conn);
	}	


