#include "Transaction.h"

// привязка Usera к какой-то глобал переменной либо в настройках, чтобы можно было использовать сущность текущего юзера
Transaction::Transaction(User* current_user) {
	std::fstream transactionsJson;
	transactionsJson.open(userTrans);
	json userTransactions;
	if (transactionsJson.peek() == std::ifstream::traits_type::eof()) {
		userTransactions["counter"] = 0;
	}
	else {
		transactionsJson >> userTransactions;
	}
	transactionsJson.close();
	
	cout << "From whose account was transaction initiated?\n1) Your account\n2) Another User's account" << endl;
	string answer;
	cin >> answer;
	int counter;
	//if the output account of the transaction is current user's account then ask to select the correct one
	if (answer == "1") {
		

		counter = 1;
		cout << "Select one of your accounts:" << endl;
		for (auto it = current_user->get_accounts()->begin(); it != current_user->get_accounts()->end(); it++) {
			cout << counter << ") " << (*it)->card_id << " - " << Dictionaries::getDepType((*it)->dep_type) << endl;
			counter++;
		}
		cin >> answer;
		auto it = current_user->get_accounts()->begin();
		for (int i = 1; i < std::stoi(answer); i++) { ++it;}
		 this->output_acc= (*it)->card_id;
		 this->output_acc_type = (*it)->dep_type;
	}

	//else to write it manually
	else {
		cout << "Please, enter transcation's output account manually:"<<endl;
		cin >> answer;
		this->output_acc = answer;
		counter = 1;
		cout << "\nand select type of the account:"<<endl;
		map<string, int>::iterator it;
		//map<string, int>* types = Dictionaries::get_depTypes();
		//for (it = (*types).begin(); it != (*types).end(); it++) {
		//	//*dep_type_num*) *dep_type_name*
		//	cout << it->second << ") " << it->first << endl;
		//}
		cin >> answer;
		this->output_acc_type = answer;
	}

	cout << "\nHow much money was transferred?"<<endl;
	cin >> answer;
	//stod converts string to double
	this->amount = std::stod(answer);
	
	cout << "\nWhere was the money transferred to?\n 1) To one of your accounts\n 2) To another User's account" << endl;
	cin >> answer;
	//if the output account of the transaction is current user's account then ask to select the correct one
	if (answer == "1") {


		counter = 1;
		cout << "Select one of your accounts:" << endl;
		for (auto it = current_user->get_accounts()->begin(); it != current_user->get_accounts()->end(); it++) {
			cout << counter << ") " << (*it)->card_id << " - " << Dictionaries::getDepType((*it)->dep_type) << endl;
			counter++;
		}
		cin >> answer;
		auto it = current_user->get_accounts()->begin();
		for (int i = 1; i < std::stoi(answer); i++) { it++; }
		this->input_acc = (*it)->card_id;
		this->input_acc_type = (*it)->dep_type;
	}

	////else to write it manually
	else {
		cout << "Please, enter transcation's output account manually:" << endl;
		cin >> answer;
		this->input_acc = answer;
		counter = 1;
		cout << "\nand select type of the account:" << endl;
		map<string, int>::iterator it;
		map<string, int>* types = Dictionaries::get_depTypes();
		for (it = (*types).begin(); it != (*types).end(); it++) {
			//*dep_type_num*) *dep_type_name*
			cout << it->second << ") " << it->first << endl;
		}
		cin >> answer;
		this->input_acc_type = answer;
	}

	//category selection plug
	cout << "Select category of the transaction:";
	
	cin >> answer;
	this->category = answer;

	cout << "Do you want to add any comment for this transaction? If no - just press Enter:" << endl;
	cin >> answer;
	this->comment = answer;

	if (current_user->check_account(this->input_acc)) {
	
		for (auto it = current_user->get_accounts()->begin(); it != current_user->get_accounts()->end(); it++) {
			if ((*it)->card_id == input_acc) {
				(*it)->money_value += amount;
				break;
			}
		}
	}

	if (current_user->check_account(this->output_acc)) {

		for (auto it = current_user->get_accounts()->begin(); it != current_user->get_accounts()->end(); it++) {
			if ((*it)->card_id == output_acc) {
				(*it)->money_value -= amount;
				break;
			}
		}
	}


	//make fun for date	
	this->date = date;
	this->type = type;
	this->amount = amount;
	this->category = category;
	this->comment=comment;
	std::ofstream test;
	test.open(userTrans);
	userTransactions[to_string(userTransactions["counter"])]["date"] = this->date;
	userTransactions[to_string(userTransactions["counter"])]["amount"] = this->amount;
	userTransactions[to_string(userTransactions["counter"])]["category"] = this->category;
	userTransactions[to_string(userTransactions["counter"])]["comment"] = this->comment;
	userTransactions[to_string(userTransactions["counter"])]["output_acc"] = this->output_acc;
	userTransactions[to_string(userTransactions["counter"])]["output_acc"] = this->output_acc_type;
	userTransactions[to_string(userTransactions["counter"])]["input_acc"] = this->input_acc;
	userTransactions[to_string(userTransactions["counter"])]["input_acc_type"] = this->input_acc_type;
	userTransactions["counter"] =userTransactions["counter"]+1;
	test << userTransactions;
	test.close();


	
}