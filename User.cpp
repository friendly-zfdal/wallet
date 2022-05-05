#include "User.h"
#define filename "user.json"
User::User(string id, string name, string birth_date) {
	this->id = id;
	this->name = name;
	this->birth_date = birth_date;
}
User::User(string id, string name, string birth_date, vector<Card*>* accounts_list) {
	this->id = id;
	this->name = name;
	this->birth_date = birth_date;
	this->accounts = vector<Card*>(*accounts_list);
}

User::~User() {
	free(this);
}

string User::get_id() {
	return this->id;
}

vector<Card*>* User::get_accounts() {
	return &(this->accounts);
}

/* add to user info one type of account below :
		0 - cash
		1 - card
		2 - brokerage account
		3 - bank deposit					*/
int User::add_account(string card_id, int bank_id, string end_date, double money_value, int dep_type) {
	Card* new_card =new Card(card_id, this->id, bank_id, end_date, money_value, dep_type);
	
	this->accounts.push_back(new_card);
	return 0;
}

//save user account values to json file with default filename
int User::save_user() {
	json user_info;
	user_info["id"] = id;
	user_info["name"] = name;
	user_info["birth_date"] = birth_date;
	user_info["accounts_list"];
	vector<Card*>::iterator it;
	int counter = 0;
	for (it = accounts.begin(); it != accounts.end(); it++) {
		user_info["accounts_list"][to_string(counter)]["card_id"] =(*it)->card_id;
		user_info["accounts_list"][to_string(counter)]["bank_id"] = (*it)->bank_id;
		user_info["accounts_list"][to_string(counter)]["account_id"] = this->id;
		user_info["accounts_list"][to_string(counter)]["end_date"] = (*it)->end_date;
		user_info["accounts_list"][to_string(counter)]["money_value"] = (*it)->money_value;
		user_info["accounts_list"][to_string(counter)]["dep_type"] = (*it)->dep_type;
		counter++;

	}
	user_info["accounts_list"]["counter"] = counter;
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
	vector<Card*>* cards=new vector<Card*>;
	for (int i = 0; i < user_json["accounts_list"]["counter"]; i++) {
		//string card_id = user_json["accounts_list"][to_string(i)]["card_id"];
		//bool test = user_json["accounts_list"][to_string(i)]["account_id"].is_string();
		//string account_id=user_json["accounts_list"][to_string(i)]["accounts_id"];
		//int bank_id=user_json["accounts_list"][to_string(i)]["bank_id"];
		//string end_date=user_json["accounts_list"][to_string(i)]["end_date"];
		//double money_value=user_json["accounts_list"][to_string(i)]["money_value"];
		//int dep_type=user_json["accounts_list"][to_string(i)]["dep_type"];


		Card* cardToPush = new Card(user_json["accounts_list"][to_string(i)]["card_id"], user_json["accounts_list"][to_string(i)]["account_id"], user_json["accounts_list"][to_string(i)]["bank_id"], user_json["accounts_list"][to_string(i)]["end_date"],
			user_json["accounts_list"][to_string(i)]["money_value"], user_json["accounts_list"][to_string(i)]["dep_type"]);
		cards->push_back(cardToPush);
	}
	User* current = new User(user_json["id"], user_json["name"], user_json["birth_date"], cards);
	return current;
}


// check if the account's id belongs to the current user
int User::check_account(string acc_id) {
	vector<Card*>::iterator it;
	for (it = this->accounts.begin(); it != this->accounts.end(); it++) {
		if ((*it)->card_id == acc_id)
			return 1;
	}
	return 0;

}