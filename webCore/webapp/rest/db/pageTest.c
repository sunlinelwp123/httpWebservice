#include <stdio.h>
#include <page.h>



int main(){

	char sql[] = "select org_id,user_id,status,user_type,user_email,user_wcat,user_sina,user_password,user_telno,user_casue,user_name from WS_SYS_USER where 1= 1";
	char sql1[] = "select * from sif_sys_dict ";
	char str[1024];
/*
	int j = getSqlWithPageForOracle(sql1,str, 1, 10);
	printf("j:%d\n", j);
	j = getSqlWithPage(sql1,str,j,10);
	j = getSqlWithPage(sql1,str,j,10);
	j = getSqlWithPage(sql1,str,j,10);
*/
	URL_T url = URL_new("oracle:///LWP?user=lwp&password=lwp");
        ConnectionPool_T pool = ConnectionPool_new(url);/*设定连接池url并创建，登陆数据库，默认建立5个连接*/
        ConnectionPool_start(pool);/*开始使用该连接池 与stop对应*/
	Connection_T con = ConnectionPool_getConnection(pool);
	/*page_util(Connection_T conn, const char *query, char *select, int pageNo, int pageSize, int total, int totalPage , ResultSet_T ret)*/
	int total,totalPage;
	ResultSet_T r;
	page_util(con, sql1, 0, 2, 20 ,&total, &totalPage, &r);
	printf("total : %d\n totalPage : %d\n", total, totalPage);
	while(ResultSet_next(r)){
		printf("dict_type:%s\n", ResultSet_getString(r, 1));
	}
	Connection_close(con);
        ConnectionPool_stop(pool);
        ConnectionPool_free(&pool);

	return 0;
}

