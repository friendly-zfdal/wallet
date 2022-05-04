#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

using namespace std;
class User {

private:
	string id;
	string name;
	string birth_date;
	map<string, int> accounts;

public:
	User(string id, string name, string birth_date);
	User(string id, string name, string birth_date, map<string,int> account_list);
	~User();
	int check_account(string);
	string get_id();
	int add_account(string, int);
	map<string, int> get_accounts();
	int save_user();
	User* load_user(string);
};