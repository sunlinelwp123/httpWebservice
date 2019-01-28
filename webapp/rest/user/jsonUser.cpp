#include "jsonUser.h"


JsonUser::JsonUser(const char *username){
	setUserName(username);
}
JsonUser::JsonUser(){
}

bool JsonUser::userParseJson(cJSON * userJson){
		if (strlen(getUserName()) == 0)return false;
		cJSON_AddStringToObject(userJson,"username", getUserName());// userName = "" 将数据放入到userJson结构体中
		cJSON_AddStringToObject(userJson,"userid", getUserId());
		cJSON_AddStringToObject(userJson,"orgid", getOrgId());
		char s[2];
		sprintf(s,"%c\0", status);
		cJSON_AddStringToObject(userJson,"status", s);
		cJSON_AddStringToObject(userJson,"useremail", userEmail);
		cJSON_AddStringToObject(userJson,"userwcat", userWcat);
		cJSON_AddStringToObject(userJson,"usersina", userSina);
		cJSON_AddStringToObject(userJson,"usertelno", userTelno);
		cJSON_AddStringToObject(userJson,"usertype", userType);
		cJSON_AddStringToObject(userJson,"usercasue", userCasue);
		/*cJSON_AddStringToObject(userJson,"userpassword", userPassWord);*/
		return true;
	}
bool JsonUser::jsonStringParseUser(const char *jsonStr,cJSON * userJson){
		userJson = cJSON_Parse(jsonStr);
		cJSON *username = NULL;
		cJSON *userid = NULL;
		cJSON *orgid = NULL;
		cJSON *status = NULL;
		cJSON *useremail = NULL;
		cJSON *userwcat = NULL;
		cJSON *usersina = NULL;
		cJSON *usertelno = NULL;
		cJSON *usertype = NULL;
		cJSON *usercasue = NULL;
		cJSON *userpassword = NULL;
		cleanUser();
		try{
			username = cJSON_GetObjectItemCaseSensitive(userJson, "username");
			if(username){setUserName(cJSON_GetStringValue(username));}else{return false;}
			orgid = cJSON_GetObjectItemCaseSensitive(userJson, "orgid");
			if(orgid)setOrgId(cJSON_GetStringValue(orgid));
			status = cJSON_GetObjectItemCaseSensitive(userJson, "status");
			if(status)setStatus(cJSON_GetStringValue(status));
			useremail = cJSON_GetObjectItemCaseSensitive(userJson, "useremail");
			if(useremail)strcpy(this->userEmail, cJSON_GetStringValue(useremail));
			userwcat = cJSON_GetObjectItemCaseSensitive(userJson, "userwcat");
			if(userwcat)strcpy(this->userWcat,cJSON_GetStringValue(userwcat));
			usersina = cJSON_GetObjectItemCaseSensitive(userJson, "usersina");
			if(usersina)strcpy(this->userSina, cJSON_GetStringValue(usersina));
			usertelno = cJSON_GetObjectItemCaseSensitive(userJson, "usertelno");
			if(usertelno)strcpy(this->userTelno, cJSON_GetStringValue(usertelno));
			usertype = cJSON_GetObjectItemCaseSensitive(userJson, "usertype");
			if(usertype)strcpy(this->userType, cJSON_GetStringValue(usertype));
			usercasue = cJSON_GetObjectItemCaseSensitive(userJson, "usercasue");
			if(usercasue)strcpy(this->userCasue, cJSON_GetStringValue(usercasue));
			userpassword = cJSON_GetObjectItemCaseSensitive(userJson, "userpassword");
			if(userpassword)strcpy(this->userPassword, cJSON_GetStringValue(userpassword));
			userid = cJSON_GetObjectItemCaseSensitive(userJson, "userid");
			if(userid)setUserId(cJSON_GetStringValue(userid));
		}catch(int ex){
			clog<< "usercpy error"<<endl;
			return false;
		}
		return true;
	}

void JsonUser::printUser(cJSON *userJson){
		cout<<"user@"<< cJSON_Print(userJson)<<endl;/*输出json数据需要转成json字符串*/
	}

JsonUser::~JsonUser(){
	}
