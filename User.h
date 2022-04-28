#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>

using namespace std;
class User {

private:
	string id;
	string name;
	string birth_date;
	map<string, int> accounts;

public:
	User(string id, string name, string birth_date);
	~User();
	string get_id();
	int add_account(string, int);
	int save_account();
};