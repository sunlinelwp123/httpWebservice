#include "user.h"
#include <cjson/cJSON.h>

using namespace std;
struct JsonUser : public User{
	JsonUser();
	JsonUser(const char *);
	bool userParseJson();
	bool jsonStringParseUser(const char *jsonStr);
	void printUser();
	~JsonUser();
public:
	cJSON *userJson;  /*json object create*/
};

