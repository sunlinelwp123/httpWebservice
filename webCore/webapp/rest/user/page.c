/**
 *@AUTHOR lwp
 *分页查询的各种函数
 *重点是page_util
 *
 */
#include <page.h>
/*oracle
 *通过下标和页容量分页 
 *s    传入sql 源sql
 *sql  返回sql 最后执行的sql
 *index 下标
 *size 页容量
 *select * from(select a.*,ROWNUM rn from(sql) a where ROWNUM<=(firstIndex+pageSize)) where rn>firstIndex
 */
int getSqlWithIndexForOracle(const char *s, char *sql,int index,int size){
	int j= 0;
	j = sprintf(sql, "%s", "select * from(select a.*,ROWNUM rn from(" );
	j += sprintf(sql + j,"%s", s);
	j += sprintf(sql + j,"%s",") a where ROWNUM<=(");
	j += sprintf(sql + j,"%d", index);	
	j += sprintf(sql + j,"%s", "+");	
	j += sprintf(sql + j, "%d", size);
	j += sprintf(sql + j,"%s", ")) where rn>");
	j += sprintf(sql + j,"%d", index);
	return (index + size);
}
/*select o.* from (sql) o limit firstIndex,pageSize
 *for mysql
 *
 */
int getSqlWithIndexForMysql(const char *s, char *sql, int index , int size){
	int j=0;
	j = sprintf(sql, "%s", "select o.* from (");
	j += sprintf(sql + j, "%s", s);
	j += sprintf(sql + j, "%s", ") o limit ");
	j += sprintf(sql + j, "%d", index);
	j += sprintf(sql + j, "%s", ",");
	j += sprintf(sql + j, "%d", size);
	printf( "%s\n",sql);
	return (index + size);
}

/*
 *mysql
 *通过页码和页容量分页
 * 
 */
int getSqlWithPage(const char *s, char *sql,int page, int pagesize){
	int index = (page-1)*pagesize;
	int next = getSqlWithIndexForMysql(s,sql,index,pagesize);
	return next/pagesize + 1;
}

/*
 *oracle
 *通过页码和页容量分页
 */
int getSqlWithPageForOracle(const char *s, char *sql,int page, int pagesize){
	int index = (page-1)*pagesize;
	int next = getSqlWithIndexForOracle(s,sql,index,pagesize);
	return next/pagesize + 1;
}

void getSqlPageNumber(const char *s, char *sql, int pagesize ){
	int j = 0 ;
	j = sprintf(sql, "%s", "select count(1) from (");
	j += sprintf(sql + j,"%s", s);
	j += sprintf(sql + j,"%s", ")");		
}
/*
 *   param     Pi_Conn            
 *	       Pi_Query           查询语句
 *             Pi_Select          查询字段
 *             Pi_Page_No         查询分页号
 *             Pi_Page_Size       每页条数
 *
 *             Po_Total           总记录数
 *             Po_Total_Page      总页数
 *             Po_Query_Page      分页查询语句
 *             ResultSet_T ret   out
 * ResultSet_T r = Connection_executeQuery(con, "SELECT count(*) FROM employees");
 * printf("Number of employees: %s\n", ResultSet_next(r) ? ResultSet_getString(r, 1) : "none");
 *
 *
 */
void page_util(Connection_T conn, const char *query, char *select/* default NULL = "*" */, int pageNo, int pageSize, int *total, int *totalPage , ResultSet_T *ret){
	ResultSet_T r = Connection_executeQuery(conn, "select count(1) from (%s ) a", query);
	*total = (ResultSet_next(r) ? ResultSet_getInt(r, 1) : 0);
	pageNo = (pageNo ? pageNo : 1);
	pageSize = (pageSize ? pageSize : 10);
	*totalPage =  (int)ceil( *total / (double)pageSize);//整型计算结果也是整型1/2=0；如果有一个数是double则结果为double
	int rowMin = (pageNo - 1) * pageSize + 1;
	int rowMax = pageNo * pageSize;
	select = (char *)(select ? select :"*");
	/*printf("select :%s\n  sql: %s\n rowmax: %d\n rowmin: %d\n total:%d\n totalpage :%d\n", select, query, rowMax, rowMin, *total, *totalPage);*/
#if IS_ORACLE
	printf("connect ...oracle\n");	
	*ret = Connection_executeQuery(conn, 
			"select %s from (select pagequery.*, rownum rn from ( %s ) pagequery where rownum <= %d ) where rn >=  %d ",
		       	select, query, rowMax, rowMin);
#else 
	printf("connect ...mysql\n");	
	*ret = Connection_executeQuery(conn, 
			"select %s from ( %s ) a limit %d,%d ",
		       	select, query, rowMin, pageSize);
#endif 
}


#if 0
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
#endif
