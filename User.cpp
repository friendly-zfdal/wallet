#include "User.h"
#define filename "user.json"
User::User(string id, string name, string birth_date) {
	this->id = id;
	this->name = name;
	this->birth_date = birth_date;
}
User::User(string id, string name, string birth_date, map<string, int> account_list) {
	this->id = id;
	this->name = name;
	this->birth_date = birth_date;
	this->accounts = account_list;
}

User::~User() {
	free(this);
}

string User::get_id() {
	return this->id;
}

map <string, int> User::get_accounts() {
	return this->accounts;
}

/* add to user info one type of account below :
		0 - cash
		1 - card
		2 - brokerage account
		3 - bank deposit					*/
int User::add_account(string account_id, int type) {
	this->accounts[account_id]=type;
	return 0;
}

//save user account values to json file with default filename
int User::save_user() {
	json user_info;
	user_info["id"] = id;
	user_info["name"] = name;
	user_info["birth_date"] = birth_date;
	user_info["accounts_list"];
	map <string, int> ::iterator it;
	for (it=accounts.begin(); it != accounts.end(); it++) {  
		user_info["accounts_list"][it->first] = it->second;
	}
	ofstream user_file;
	user_file.open(filename);
	user_file << user_info << endl;
	user_file.close();
	return 0;
}

// to load user info just provide the saved json file's name
User* User::load_user(string a) {
	std::ifstream user_file(a);
	json user_json;
	user_file >> user_json;
	map<string, int>::iterator it;
	map < string, int > test= user_json["accounts_list"];
	User current = User(user_json["id"], user_json["name"], user_json["birth_date"], user_json["accounts_list"]);
	return &current;
}


// check if the account's id belongs to the current user
int User::check_account(string acc_id) {
	map<string, int>::iterator it;
	for (it = this->accounts.begin(); it != this->accounts.end(); it++) {
		if (it->first == acc_id)
			return 1;
	}
	return 0;

}