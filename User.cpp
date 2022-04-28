#include "User.h"
#define filename "user.txt"
User::User(string id, string name, string birth_date) {
	this->id = id;
	this->name = name;
	this->birth_date = birth_date;
}

string User::get_id() {
	return this->id;
}

int User::add_account(string account_id, int type) {
	this->accounts[account_id]=type;
	return 0;
}

int User::save_account() {
	string user_info = "id=" + this->id + ";\nname=" + this->name + ";\nbirth_date=" + birth_date +";\n";
	map <string, int> ::iterator it;
	for (it=accounts.begin(); it != accounts.end(); it++) {  
		user_info += "account_id:"+it->first + "\taccount_type:" + to_string(it->second) + ";\n";
	}
	ofstream user_file;
	user_file.open(filename);
	user_file << user_info << endl;
	user_file.close();
	return 0;
}