#pragma once
#include "Wallet.h"
#include "Authorization.h"
#include "Transaction.h"
#include "Dictionaries.h"
#include "Source_Egor.h"
using namespace std;
class Menu
{
public:
	static int startup();
	static int user_menu();
	static string acc_info();
	static int login_check(string login);
};

