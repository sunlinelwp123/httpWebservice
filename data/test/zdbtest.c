#include <zdb/zdb.h>
#include <stdio.h>

void main(void){

	URL_T url = URL_new("oracle:///LWP?user=lwp&password=lwp"); 
	ConnectionPool_T pool = ConnectionPool_new(url); 
	ConnectionPool_start(pool); 
	Connection_T con = ConnectionPool_getConnection(pool);
	printf("conn success\n");
	 ResultSet_T result = Connection_executeQuery(con, "select user_id from WS_SYS_USER"); 
	while (ResultSet_next(result)) {//error <0 nodata=0	
		const char *userId = ResultSet_getString(result, 1);
		printf("userid=%s", userId); 
	}
}
