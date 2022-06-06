#include "Menu.h"


int Menu::startup() {

	//Dictionaries::currency_select();
	//fstream test;
	//test.open("./random.txt", ios::out);
	//test.close();
	//Dictionaries::currencies_init();
	//double test_cur=Dictionaries::currency_select();
	//cout << sizeof(char);
	//string pin = "622814";
	//string text = Encryption::mix(&pin, "Ya sashok poelsnezhok{}!)");
	//string text1 = Encryption::deMix(&pin, text);
	//Encryption::file_encrypt("./Users/116101115116.json", "622814");
	//Encryption::file_decrypt("./Users/116101115116.json", "622814");
	//Encryption::pin_encrypt(&pin);
	//string test=Encryption::pin_decrypt(&pin);

	//Wallet init
	login_check("test");
	Wallet();

	int answer;
	string login;
	string pin;
	
	if (!Wallet::last_login.empty()) {
		cout << "Is your login " + Wallet::last_login + "?\n1) Yes\n2) No\nEnter the digit:";
		int choice;
		cin >> choice;
		if (choice == 1) {
			cout << "\nEnter your pin(6 digits):";
			cin >> pin;
			if (Authorization::signIn(Wallet::last_login, pin)) {
				map<string, int>* types = Wallet::current_user->get_types();
				map<string, int>::iterator it = types->begin();
				map<string, int>* dic_type = Dictionaries::get_depTypes();
				while (it != types->end()) {
					dic_type->insert(make_pair((*it).first, (*it).second));
					it++;
				}
				user_menu();
				return 0;
			}
		}
		else {
			cout << "Then sign in or sign up with your credentials\n";
		}
		

	}
	cout << "What do you want?\n1)Sign in\n2) Sign up\n";
	cin >> answer;
	if (answer == 1) {
		cout << "Please enter your login:";
		cin >> login;
		cout << "\nEnter your pin(6 digits):";
		cin >> pin;
		if (Authorization::signIn(login, pin)) {
			map<string, int>* types = Wallet::current_user->get_types();
			map<string, int>::iterator it=types->begin();
			map<string, int>* dic_type = Dictionaries::get_depTypes();
			while (it != types->end()) {
				dic_type->insert(make_pair((*it).first, (*it).second));
				it++;
			}
			user_menu();
		}

	}
	if (answer == 2) {
		string login, id, birth_date, name, pin;
		cout << "\nEnter your login(any special symbols and symbols from english alphabet are allowed, one word):";
		cin >> login;
		if (!login_check(login)) {
			cout << "Unfortunately, this login is already taken by someone. Try to sign up with another one.";
				return 0;
		}
		id = Wallet::user_counter;
		Wallet::user_counter += 1;
		cout << "\nbirth_date in MM/DD/YYYY:";
		cin >> birth_date;
		cout << "Your name:";
		cin.ignore(1, '\n');
		getline(cin, name);
		cout << "Your pin (will be used for sign in, 6 digits):";
		cin >> pin;
		if (Authorization::signUp(login, to_string(Wallet::user_counter), birth_date, name, pin)) {
			Dictionaries::depTypesInit(Wallet::current_user);
			user_menu();
		}
	}
	return 0;
}

string Menu::acc_info()
{
	User* user = Wallet::current_user;
	vector<Card*>::iterator it = user->get_accounts()->begin();
	string output = "";

	if (user->get_accounts()->size() == 0) {
		output = "You don't have accounts yet. \nGreat reason to create a new one!\n";
		return output;
	}

	output += "Now you have " + to_string(user->get_accounts()->size()) + " account(s):\n";
	int counter = 1;
	while (it != user->get_accounts()->end()) {
		output +=to_string(counter) + ") account_id: " + (*it)->card_id + " | " + (*it)->alias + " - " + Dictionaries::getDepType((*it)->dep_type) + " | money_value: " + to_string((*it)->money_value) + " | account expiration date: "+ (*it)->end_date+"\n";
		it++;
	}
	return output;
}

int Menu::login_check(string login)
{
	fstream file;
	file.open("./Users/users.txt");
	string str;
	while (getline(file, str)) {
		if (str == login)
			return 0;
	}
	return 1;
}

int Menu::user_menu() {
	cout << "\nWelcome, " + Wallet::current_user->get_name() + "!\n";
	while (1) {
		cout << acc_info();
		cout << "\n1)Profile change\n2)Transactions\n3)Accounts\n4)Stocks\n5)Reports\n6)Other settings(account types, categories)\n7)Exit" << endl;
		int answer;
		cin >> answer;
		switch (answer) {
		case 1:
			Wallet::current_user->changeUserProps();
			break;
		case 2:
		{
			cout << "\n1)Add transaction\n2)Delete transaction\n3)Modify transaction\n4)Back to start page\n";
			int trans;
			cin >> trans;
			switch (trans) {
			case 1: {
				if (Wallet::current_user->get_accounts()->size() == 0) {
					cout << "\n\nYou don't have accounts yet. Try to create one!\n\n";
					break;
				}
				Transaction* trans = new Transaction(Wallet::current_user);
				Transaction::transactions.push_back(trans);
				std::sort(Transaction::transactions.begin(), Transaction::transactions.end(), Transaction::compTrans);
			}
				break;
			case 2:
				Transaction::trans_delete(Wallet::current_user);
				break;
			case 3:
				Transaction::trans_edit(Wallet::current_user);
				break;
			case 4:
				break;
			}
		}
		break;
		case 3:
		{
			cout << "\n1)Add account\n2)Delete account\n3)Modify account\n4)Back to start page\n";
			int acc;
			cin >> acc;
			switch (acc) {
				//add
			case 1: {
				string alias;
				string card_id;
				string account_id;
				int bank_id;
				string bank;
				double money_value;
				string end_date;
				int dep_type;
				cout << "Enter card id:";
				cin >> card_id;
				if (Wallet::current_user->unique_account(card_id) == 0) {
					cout << "\nYou already have the account with this id! Try another one\n\n";
					break;
				}
				cout << "Enter alias:";
				cin >> alias;
				account_id = Wallet::current_user->get_id();
				cout << "Enter bank name:";
				cin.ignore(1, '\n');
				getline(cin, bank);
				//map<string, int>::iterator it;
				bank_id = Dictionaries::addBank(bank);

				cout << "Enter money value in USD ($):";
				cin >> money_value;
				cout << "Enter the expire date for this account(MM/DD/YYYY):";
				cin >> end_date;
				cout << "Select account type:" << endl;
				map<string, int>::iterator map_it = Dictionaries::get_depTypes()->begin();
				int counter = 0;
				while (map_it != Dictionaries::get_depTypes()->end()) {
					cout << to_string(counter + 1) + ") " + map_it->first << endl;
					map_it++;
					counter++;
				}

				cin >> dep_type;
				if (dep_type<1 || dep_type>counter) {
					cout << "\nincorrect input\n";
					continue;
				}
				advance(map_it, -(counter - dep_type + 1));
				dep_type = map_it->second;

				Wallet::current_user->add_account(card_id, bank_id, end_date, money_value, dep_type, alias);
			}
				  break;
				  //delete
			case 2:
			{
				vector<Card*> cards = *Wallet::current_user->get_accounts();
				vector<Card*>::iterator card_it = cards.begin();
				int counter = 0;
				cout << "\nSelect one of your accounts:" << endl;
				while (card_it != cards.end()) {
					cout << to_string(counter + 1) + ") " + (*card_it)->get_card_info();
					card_it++;
					counter++;
				}
				int choice;
				cin >> choice;

				if (choice<1 || choice>counter) {
					cout << "\nincorrect input\n";
					continue;
				}
				Wallet::current_user->get_accounts()->erase(Wallet::current_user->get_accounts()->begin() + (choice - 1));

			}

			break;
			//modify
			case 3: {
				vector<Card*> cards = *Wallet::current_user->get_accounts();
				vector<Card*>::iterator card_it = cards.begin();
				int counter = 0;
				cout << "\nSelect one of your accounts:" << endl;
				while (card_it != cards.end()) {
					cout << to_string(counter + 1) + ") " + (*card_it)->get_card_info();
					card_it++;
					counter++;
				}
				int choice;
				cin >> choice;

				if (choice<1 || choice>counter) {
					cout << "\nincorrect input\n";
					break;
				}
				//need to check
				advance(card_it, -(counter - choice + 1));

				cout << "\n Which field do you want to modify?" << endl;
				cout << "1)alias\n2)account type\n3)expiration date\n4)money value\nSelect:";
				string answer;
				cin >> answer;

				// проверка ввода

				switch (stoi(answer)) {
				case 1: {
					cout << "\nEnter new alias for this account:" << endl;
					cin.ignore(1, '\n');
					getline(cin, answer);
					string temp = (*card_it)->alias;
					(*card_it)->alias = answer;
					cout << "Alias of this account has been successfully changed from \"" + temp + "\" to \"" + answer + "\"" << endl;
				}
					  break;
				case 2: {
					cout << "Current type of this account is " + Dictionaries::getDepType((*card_it)->dep_type) + "\nSelect new type for this account" << endl;
					cin.ignore(1, '\n');
					map<string, int> types = *(Dictionaries::get_depTypes());
					map<string, int>::iterator it = types.begin();
					counter=0;
					while (it != types.end()) {
						cout << to_string(counter + 1) + ") " + it->first << endl;
						it++;
						counter++;
					}
					string choice;
					cin >> choice;
					//проверка
					advance(it, -(counter - stoi(choice) + 1));
					int temp = (*card_it)->dep_type;
					(*card_it)->dep_type = it->second;
					cout << "Account's type has been successfully changed from \"" + Dictionaries::getDepType(temp) + "\" to \"" + it->first + "\"" << endl;
				}
					  break;
				case 3: {
					cout << "Current expiration date of this account is " + (*card_it)->end_date << endl;
					cout << "Enter new date(MM/DD/YYYY):";
					cin.ignore(1, '\n');
					string date;
					cin >> date;
					(*card_it)->end_date = date;
					cout << "Account's expiartion date havs been successfully changed" << endl;
				}
					  break;
				case 4: {
					cout << "Currently " + to_string((*card_it)->money_value) + "$ on this account" << endl;
					cout << "Enter new value:";
					cin.ignore(1, '\n');
					string value;
					cin >> value;
					(*card_it)->money_value = stod(value);
					cout << "Account's money value has been changed to " + value + " $" << endl;
				}
					  break;
				default: {
					cout << "Incorrect input, try one more time" << endl;
					break;
				}

				}
				break;


			}
			//back to menu	 
			case 4:
				break;
			}
		}
		break;
		case 4:
			Stock::mainMenu();
			break;
		case 5:
			
			//Max's part with reports
			break;
		case 6: {
			cout << "In this section you can add custom values:\n1)Account type\n2)Category\n3)Back to the start menu\n";
			string answer;
			cin >> answer;
			switch (stoi(answer)) {
			case 1:
			{
				string type;
				cout << "What type of account do you want to add?" << endl;
				cin.ignore(1, '\n');
				getline(cin, type);
				Dictionaries::add_depTypes(type, Wallet::current_user);
			}
			break;
			case 2:
			{
				string category;
				cout << "What category do you want to add?" << endl;
				cin.ignore(1, '\n');
				getline(cin, category);
				Wallet::current_user->add_cat(category);
			}
			break;
			case 3:
			{
				break;
			}
			break;
			default:
				cout << "incorrect input" << endl;
				break;
			}
		}
			break;
		case 7:
			Wallet::current_user->save_user(Authorization::loginToASCII(Wallet::current_user->get_login()));
			Transaction::save_trans(Wallet::current_user);
			Wallet::save_wallet();
			return 0;
			break;
			//save account, additional user types, transactions and settings

		}
	}
	return 0;
}

