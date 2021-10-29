#include "pch.h"
#include "hiveon.h"

#include "json.hpp"
#include <exception>
using namespace nlohmann;
//
//CHiveOn::CHiveOn(string hostname)
//{
//	cli = make_shared<httplib::Client>(hostname);
//}
//
//CHiveOn::~CHiveOn()
//{
//}

/*
* 
* response
{
  "access_token": "string",
  "token_type": "bearer",
  "expires_in": 0
}
*/
string CHiveOn::login(string email, string passwd, string twofaCode, bool remember)
{
	httplib::Client cli("https://api2.hiveos.farm");
	//httplib::Params params;
	//params.insert(pair<string, string>("login", email));
	//params.insert(pair<string, string>("password", passwd));
	//params.insert(pair<string, string>("twofa_code", twofaCode));
	////params.insert(pair<string, bool>("remember", remember));

	json jparam( map<string, string>({ {"login",email }, {"password", passwd}, {"twofa_code" , twofaCode} }));
	//jparam.push_back(pair<string, string>("remember", twofaCode));


	string jstr = jparam.dump();

	auto res = cli.Post("/api/v2/auth/login", jstr, "application/json");
	if (200 != res->status)
	{
		throw runtime_error("login failed:" + res->body);
	}

	json jres = json::parse(res->body);
	if (jres["token_type"] != "bearer")
	{
		throw runtime_error("token_type isn't bearer");
	}

	return jres["access_token"];
}

void CHiveOn::getAccountInfo()
{


}
