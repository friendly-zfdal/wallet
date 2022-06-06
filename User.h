#pragma once

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include <vector>
#include "Card.h"
#include "Encryption.h"
#define user_directory "./Users/"

using json = nlohmann::json;

using namespace std;
class User {

private:

	string login;
	string id;
	string name;
	string birth_date;
	vector <Card*> accounts;
	
	map <string, int> categories;
	map <string, int> dep_types;
	int changeLogin();
	int changeName();
	int changeDate();
	int changePin();

public:
	string pin;
	User(string id, string name, string birth_date, string pin, string login);
	User(string id, string name, string birth_date, vector<Card*>* accounts_list, string pin, string login);
	string get_login();
	~User();
	int check_account(string);
	int remove_acc(vector <Card*>::iterator acc);
	int changeUserProps();
	string get_id();
	int add_account(string card_id, int bank_id, string end_date, double money_value, int dep_type, string alias);
	vector<Card*>* get_accounts();
	int save_user(string loginASCII);
	static User* load_user(string, string);
	map<string, int>* get_types();
	map<string, int>* get_cats();
	int add_cat(string cat);
	string get_name();
	int unique_account(string id);
};