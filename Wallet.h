#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "json.hpp"
#include "User.h"
#define settings_file "./Users/settings.json"
using json = nlohmann::json;

using namespace std;
class Wallet {
public:
	Wallet();
	static User* current_user;
	static int user_counter;
	 //save settings
	int save_wallet();


};