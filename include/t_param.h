#ifndef __T_PARAM_H__
#define __T_PARAM_H__

/*
 * server系统参数 
 */
/***************START SERVER******************/
#ifndef SERVER_PORT
#define SERVER_PORT 8080
#endif

#ifndef BACKLOG
#define BACKLOG 5
#endif

#ifndef SERVER_STRING
#define SERVER_STRING "Server:http/1.0.0\r\n"
#endif
/****************END SERVER******************/
/*
 * DB
 *
 */
/****************START DB********************/
#ifndef IS_ORACLE
#define IS_ORACLE 1
#endif

#ifndef ORACLE_URL 
#define ORACLE_URL "oracle:///LWP?user=lwp&password=lwp"
#endif

#ifndef MYSQL_URL
#define MYSQL_URL "mysql://172.40.83.126/three?user=root&password=tarena"
#endif 

/****************END  DB*********************/



#endif
