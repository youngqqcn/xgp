#pragma once



// ʹ��https
#define CPPHTTPLIB_OPENSSL_SUPPORT
#include "httplib.h"

#include <string>
#include <memory>
using namespace std;

// ��װhiveon��һЩRESTful �ӿ�
class CHiveOn
{
private:

public:
	//CHiveOn(string hostname);
	//~CHiveOn();


	// ��¼ /auth/login,  POST
	string login(string email, string passwd, string twofaCode, bool remember);

	// ��ȡ�˻���Ϣ
	void getAccountInfo();



};

