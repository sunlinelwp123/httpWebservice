/**
 *@AUTHOR lwp
 *分页查询的各种函数
 *
 *
 */
#include <string.h>
#include <stdio.h>
#include <zdb/zdb.h>
#include <math.h>
/*oracle
 *通过下标和页容量分页 
 *s    传入sql 源sql
 *sql  返回sql 最后执行的sql
 *index 下标
 *size 页容量
 *select * from(select a.*,ROWNUM rn from(sql) a where ROWNUM<=(firstIndex+pageSize)) where rn>firstIndex
 */	
int getSqlWithIndexForOracle(const char *s, char *sql,int index,int size);
/*select o.* from (sql) o limit firstIndex,pageSize
 *for mysql
 *
 */
int getSqlWithIndexForMysql(const char *s, char *sql, int index , int size);

/*
 *mysql
 *通过页码和页容量分页
 * 
 */
int getSqlWithPage(const char *s, char *sql,int page, int pagesize);

/*
 *oracle
 *通过页码和页容量分页
 */
int getSqlWithPageForOracle(const char *s, char *sql,int page, int pagesize);

void getSqlPageNumber(const char *s, char *sql, int pagesize );
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

void page_util(Connection_T conn, const char *query, char *select/* default NULL = "*" */, int pageNo, int pageSize, int *total, int *totalPage , ResultSet_T *ret);


