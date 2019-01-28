#ifndef __DBCONN_H_
#include <zdb/zdb.h>

using namespace std;

struct DBconn{
	DBconn(const char* url_str);
	DBconn();	
	~DBconn(void);
	Connection_T getConnection();
	void connClose(Connection_T conn);
private:
	URL_T url;
	ConnectionPool_T pool;
};

#endif// __DBCONN_H_ 
