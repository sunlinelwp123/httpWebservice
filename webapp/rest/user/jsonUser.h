#include "user.h"
#include <cjson/cJSON.h>

using namespace std;
struct JsonUser : public User{
	JsonUser();
	JsonUser(const char *);
	bool userParseJson(cJSON *);
	bool jsonStringParseUser(const char *jsonStr,cJSON *);
	void printUser(cJSON *);
	~JsonUser();
};

