#pragma once



// 使用https
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include <string>
#include <memory>
using namespace std;

// 封装hiveon的一些RESTful 接口
class CHiveOn
{
private:

public:
	//CHiveOn(string hostname);
	//~CHiveOn();


	// 登录 /auth/login,  POST
	string login(string email, string passwd, string twofaCode, bool remember);

	// 获取账户信息
	void getAccountInfo();



};

