#include "Menu.h"
#include "Source_Egor.h"

int Menu::startup() {
	string pin = "622814";
	string text = Encryption::mix(&pin, "Ya sashok poelsnezhok{}!)");
	string text1 = Encryption::deMix(&pin, text);
	Encryption::file_encrypt("./Users/116101115116.json", "622814");
	Encryption::file_decrypt("./Users/116101115116.json", "622814");
	Encryption::pin_encrypt(&pin);
	Encryption::pin_decrypt(&pin);
	Wallet();
	cout << "What do you want?\n1)Sign in\n2) Sign up\n";
	int answer;
	string login;
	//string pin;
	cin >> answer;
	if (answer == 1) {
		cout << "Please enter your login:";
		cin >> login;
		cout << "\nEnter your pin(6 digit):";
		cin >> pin;
		if (Authorization::signIn(login, pin)) {
			return 1;
		}

	}
	if (answer == 2) {
		string login, id, birth_date, name, pin;
		cout << "\nEnter your login(any special symbols and symbols from english alphabet are allowed, one word):";
		cin >> login;
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
			return 1;
		}
	}
	return 0;
}

int Menu::user_menu() {
	cout << "\n1)Account change\n2)Transactions\n3)Accounts\n4)Stocks\n5)Reports\n6)Exit" << endl;
	int answer;
	cin >> answer;
	switch (answer) {
	case 1:
		(*(Wallet::current_user)).changeUserProps();
		break;
	case 2:
		cout << "\n1)Add transaction\n2)Delete transaction\n3)Modify transaction\n";
		int trans;
		cin >> trans;
		switch (trans) {
		case 1:
			Transaction::Transaction(Wallet::current_user);
			break;
		case 2:
			Transaction::trans_delete(Wallet::current_user);
			break;
		case 3:
			Transaction::trans_edit(Wallet::current_user);
			break;
		}
	case 3:
		cout << "\n1)Add account\n2)Delete account\n3)Modify account\n";
		int acc;
		cin >> acc;
		switch (acc) {
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
			cout << "Enter alias:";
			cin >> alias;
			account_id = Wallet::current_user->get_id();
			cout << "Enter bank name";
			map<string, int> banks = *Dictionaries::getBanks();
			cin >> bank;
			map<string, int>::iterator it;
			it = banks.find(bank);
			if (it != banks.end()) {
				bank_id = it->second;
				break;
			}
			else {
				Dictionaries::addBank(bank);
				bank_id = Dictionaries::getBanks()->size() - 1;
			}
			
			cout << "Enter money value:";
			cin >> money_value;
			cout << "Enter the expire date for this account(MM/DD/YYYY):";
			cin >> end_date;
			cout << "Enter account type:";
			map<string, int>::iterator map_it = Dictionaries::get_depTypes()->begin();
			while (map_it != Dictionaries::get_depTypes()->end()) {
				cout << map_it->second + ") " + map_it->first << endl;
				map_it++;
			}
			cin >> dep_type;
			Wallet::current_user->add_account(card_id, bank_id, end_date, money_value, dep_type, alias);
			}
				break;
			case 2:
			{
				vector<Card*> cards = *Wallet::current_user->get_accounts();
				vector<Card*>::iterator card_it=cards.begin();
				while (card_it != cards.end()) {
					cout << (*card_it)->get_card_info();
				}
			}
			//delete
				break;
			case 3:
			//modify
				break;
		}
		break;
	case 4:
		Stock::mainMenu();
		break;
	case 5:
		break;
	case 6:
		break;
		//save account, additional user types, transactions and settings

	}
	return 0;
}