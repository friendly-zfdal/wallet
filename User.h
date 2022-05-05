#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include <vector>
#include "Card.h"

using json = nlohmann::json;

using namespace std;
class User {

private:
	string id;
	string name;
	string birth_date;
	vector <Card*> accounts;

public:
	User(string id, string name, string birth_date);
	User(string id, string name, string birth_date, vector<Card*>* accounts_list);
	~User();
	int check_account(string);
	string get_id();
	int add_account(string card_id, int bank_id, string end_date, double money_value, int dep_type);
	vector<Card*>* get_accounts();
	int save_user();
	static User* load_user(string);
};