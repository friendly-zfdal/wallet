#include "User.h"

User::User(string id, string name, string birth_date, string pin, string login) {
	this->login = login;
	this->id = id;
	this->name = name;
	this->birth_date = birth_date;
	this->pin = pin;
	Encryption::pin_encrypt(&(this->pin));
	ofstream file_out;
	file_out.open("./Users/users.txt", std::ios_base::app);
	file_out << this->login << endl;
	file_out.close();
}
User::User(string id, string name, string birth_date, vector<Card*>* accounts_list, string pin, string login) {
	this->id = id;
	this->name = name;
	this->birth_date = birth_date;
	this->accounts = vector<Card*>(*accounts_list);
	Encryption::pin_encrypt(&pin);
	this->pin = pin;
	this->login = login;
}

User::~User() {
	free(this);
}

string User::get_login() {
	return this->login;
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
int User::add_account(string card_id, int bank_id, string end_date, double money_value, int dep_type,string  alias) {
	Card* new_card =new Card(card_id, this->id, bank_id, end_date, money_value, dep_type, alias);
	
	this->accounts.push_back(new_card);
	return 0;
}

//save user account values to json file "user_name.json" filename
int User::save_user(string login_ASCII) {
	json user_info;
	user_info["id"] = id;
	user_info["login"] = login;
	user_info["name"] = name;
	user_info["birth_date"] = birth_date;
	//pin will be encrypted with pid which will be lost after closing the program, so we need to decrypt it before closing the program
	user_info["pin"] = Encryption::pin_decrypt(&(this->pin));
	//for sign up saving case (kostyl zhestki)
	Encryption::pin_encrypt(&(this->pin));
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
		user_info["accounts_list"][to_string(counter)]["alias"] = (*it)->alias;

		counter++;

	}
	user_info["accounts_list"]["counter"] = counter;
	map <string, int>::iterator map_it;
	counter = 0;
	for (map_it = categories.begin(); map_it != categories.end(); map_it++) {
		user_info["categories"][to_string((*map_it).second)] = (*map_it).first;
		counter++;
	}
	user_info["categories"]["counter"] = counter;

	counter = 0;
	for (map_it = dep_types.begin(); map_it != dep_types.end(); map_it++) {
		user_info["dep_types"][to_string((*map_it).second)] = (*map_it).first;
		counter++;
	}
	user_info["dep_types"]["counter"] = counter;


	ofstream user_file;
	user_file.open(user_directory + login_ASCII + ".json");
	user_file << user_info << endl;
	user_file.close();
	//encryption after saving
	Encryption::file_encrypt(user_directory + login_ASCII + ".json", user_info["pin"]);
	return 0;
}

// to load user info just provide the saved json file's name and pin for decryption/encryption OPs
User* User::load_user(string filename, string pin) {
	//decrypt and load to json
	Encryption::file_decrypt(filename, pin);
	std::ifstream user_file(filename);
	json user_json;
	
	try {
		user_file >> user_json;
	}
	catch(json::exception) {
		cout << "Password is incorrect";
		Encryption::file_encrypt(filename, pin);
		return NULL;
	}
	map<string, int>::iterator it;
	vector<Card*>* cards=new vector<Card*>;

	//encrypting file back after loading to json
	Encryption::file_encrypt(filename, pin);

	//check pin
	if (pin != user_json["pin"]) {
		cout << "provided pin is incorrect";
		return NULL;
	}

	//add accounts array
	for (int i = 0; i < user_json["accounts_list"]["counter"]; i++) {
		Card* cardToPush = new Card(user_json["accounts_list"][to_string(i)]["card_id"], user_json["accounts_list"][to_string(i)]["account_id"], user_json["accounts_list"][to_string(i)]["bank_id"], user_json["accounts_list"][to_string(i)]["end_date"],
			user_json["accounts_list"][to_string(i)]["money_value"], user_json["accounts_list"][to_string(i)]["dep_type"], user_json["accounts_list"][to_string(i)]["alias"]);
		cards->push_back(cardToPush);
	}
	//create user
	User* current = new User(user_json["id"], user_json["name"], user_json["birth_date"], cards, pin, user_json["login"]);

	map<string, string>::iterator test;
	//check if user has custom cats
		for (auto& [key, val] : user_json["categories"].items()) {
			if (key == "counter")
				continue;
			current->categories.insert(make_pair(val, stoi(key)));
		}
	
	// check if user has custom deposit types for accounts
	for (auto& [key, val] : user_json["dep_types"].items()) {
		if (key == "counter")
			continue;
		current->dep_types.insert(make_pair(val, stoi(key)));
	}




	//return loaded user
	return current;
}

map<string, int>* User::get_types()
{
	return &(this->dep_types);
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

int User::remove_acc(vector<Card*>::iterator acc)
{
	this->accounts.erase(acc);
	return 0;
}

int User::changeLogin() {
	string cur_login = this->login;
	cout << "Please type your new login and press Enter" << endl;
	string login;
	cin >> login;
	this->login = login;

	//delete user file with previous login (the new one will be created and saved during app closing)
	// have to release func for login transformation there due to include loop :(
	string temp;
	for (int i = 0; i < cur_login.length(); i++) {
		temp+= to_string((int)cur_login[i]);
	}
	std::filesystem::remove("./Users/" + temp + ".json");
	/////////////////////////////////////////////////////////////

	cout << "Your login has been successfully changed from " << cur_login << " to " << this->login << endl;
	return 0;
}

int User::changeName() {
	cout << "your current name is " << this->name << ", do you really want to change it?";
	cout << "1)Yes\n2)No" << endl;
	int answer;
	cin >> answer;
	string name;
	if (answer == 1) {
		cout << "Enter your new name: ";
		cin >> name;
		this->name = name;
		cout << "\nYour name has been successfully changed to " << this->name << endl;
	}
	else {
		return 1;
	}
	return 0;
}

int User::changeDate() {
	cout << "Enter the new date of your birth:";
	string date;
	cin >> date;
	this->birth_date = date;
	cout << "Your birth date has been successfully changed" << endl;
	return 0;
}

int User::changePin() {
	string pin = Encryption::pin_decrypt(&(this->pin));
	Encryption::pin_encrypt(&(this->pin));
	cout << "Enter your current pin" << endl;
	string old_pin;
	string new_pin;
	string new_pin_cmp;
	cin >> old_pin;
	if (old_pin == pin) {
		cout << "Now enter the new one:";
		cin >> new_pin;
		cout << "\n One more time:";
		cin >> new_pin_cmp;
		if (new_pin == new_pin_cmp) {
			this->pin = new_pin;
			Encryption::pin_encrypt(&(this->pin));
			new_pin = "";
			new_pin_cmp = "";
			cout << "\nYour pin has been successfully changed" << endl;
		}
		else { cout << "Entered pins are not equal" << endl;}
	}
	else { cout << "Entered pin is incorrect" << endl;}
	return 0;
}

int User::changeUserProps() {
	cout << "Which field do you want to change?" << endl;
	cout << "1) Login\n 2) Name\n 3) Birth date\n 4) Pin code\n 5) Back to start page" << endl;
	int a;
	cin >> a;
	switch (a) {
		case 1:
			changeLogin();
			break;
		case 2:
			changeName();
			break;
		case 3:
			changeDate();
			break;
		case 4:
			changePin();
			break;
		case 5:
			break;
		default:
			cout << "Incorrect input value" << endl;
	}
	return 0;
}

map<string, int>* User::get_cats() {
	return &(this->categories);
}



int User::add_cat(string cat)
{
	categories.insert(make_pair(cat, categories.size()));
	
	return 0;
}

string User::get_name()
{
	return name;
}

int User::unique_account(string id)
{
	//if account with provided id already exists in user's profile then return 0
	vector<Card*>::iterator it = accounts.begin();
	while (it != accounts.end()) {
		if ((*it)->card_id == id)
			return 0;
		it++;
	}
	//else 1
	return 1;
}


