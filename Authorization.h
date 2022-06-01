#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <filesystem>
#include "Transaction.h"
#include "Wallet.h"

using namespace std;

class Authorization
{
public:
	static int signIn(string login, string pin);
	static int signUp(string login,string id, string birth_date, string name, string pin);
	static string loginToASCII(string login);
	static string loginFromASCII(string login);
	
};

