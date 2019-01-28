#include <dbconn.h>
#include <stdio.h>


int main(void){
#if 1
	/*有参构造 实现数据库连接 自己传数据库地址*/
	//DBconn *db = new DBconn("mysql://172.40.83.126/three?user=root&password=tarena") ;
	//DBconn *db = new DBconn("mysql://192.168.43.108/lwp_db?user=root&password=12345") ;
	DBconn *db = new DBconn("oracle:///LWP?user=lwp&password=lwp") ;
	Connection_T con = db->getConnection();
	printf("conn success\n");
	 ResultSet_T result = Connection_executeQuery(con, "select user_id from WS_SYS_USER");
	while (ResultSet_next(result)) {//error <0 nodata=0
		const char *userId = ResultSet_getString(result, 1);
		printf("userid=%s", userId);
	}
	Connection_close(con);
	delete db;
#endif 
#if 1
	/* 无参构造 默认 172.40.83.126的库写死在代码中 TODO 配置文件读取*/
	DBconn *db1 = new DBconn();
	Connection_T conn = db->getConnection();
	printf("conn success\n");
	 ResultSet_T result1 = Connection_executeQuery(conn, "select user_id from WS_SYS_USER");
	while (ResultSet_next(result1)) {//error <0 nodata=0
		const char *userId = ResultSet_getString(result1, 1);
		printf("userid=%s", userId);
	}
	Connection_close(conn);
	delete db1;
	
#endif
	return 0;

}
