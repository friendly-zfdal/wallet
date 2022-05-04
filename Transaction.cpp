#include "Transaction.h"

// привязка Usera к какой-то глобал переменной либо в настройках, чтобы можно было использовать сущность текущего юзера
Transaction::Transaction(User* current_user) {
	std::fstream transactionsJson;
	transactionsJson.open(userTrans);
	json userTransactions;
	transactionsJson >> userTransactions;
	if (userTransactions.empty()) {
		userTransactions["counter"] = 0;
	}
	
	cout << "From whose account was transaction initiated?\n1) Your account\n2) Another User's account" << endl;
	string answer;
	cin >> answer;
	int counter;
	//if the output account of the transaction is current user's account then ask to select the correct one
	if (answer == "1") {
		map<string, int>::iterator it;
		counter = 1;
		cout << "Select one of your accounts:" << endl;
		for (it = current_user->get_accounts().begin(); it != current_user->get_accounts().begin(); it++) {
			cout << counter << ") " << it->first << " - " << Dictionaries::getDepType(it->second) << endl;
		}
		cin >> answer;
		it = current_user->get_accounts().begin();
		for (int i = 0; i < std::stoi(answer); i++) { it++;}
		 this->output_acc=it->first;
		 this->output_acc_type = it->second;
	}

	//else to write it manually
	else {
		cout << "Please, enter transcation's output account manually:"<<endl;
		cin >> answer;
		this->output_acc = answer;
		counter = 1;
		cout << "\nand select type of the account:"<<endl;
		map<string, int>::iterator it;
		map<string, int>* types = Dictionaries::get_depTypes();
		for (it = (*types).begin(); it != (*types).end(); it++) {
			//*dep_type_num*) *dep_type_name*
			cout << it->second << ") " << it->first << endl;
		}
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
		map<string, int>::iterator it;
		counter = 1;
		cout << "Select one of your accounts:" << endl;
		for (it = current_user->get_accounts().begin(); it != current_user->get_accounts().begin(); it++) {
			cout << counter << ") " << it->first << " - " << Dictionaries::getDepType(it->second) << endl;
		}
		cin >> answer;
		it = current_user->get_accounts().begin();
		for (int i = 0; i < std::stoi(answer); i++) { it++; }
		this->input_acc = it->first;
		this->input_acc_type = it->second;
	}

	//else to write it manually
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
	cout << "Select category of the transaction";
	
	cin >> answer;
	this->category = answer;

	cout << "Do you want to add any comment for this transaction? If no - just press Enter:" << endl;
	cin >> answer;
	this->comment = answer;

	if (current_user->check_account(this->input_acc)) {
		//plug
		//we should increase money value on one of the current user's accounts 
	}

	if (current_user->check_account(this->output_acc)) {
		//plug
		//we should decrease money value on one of the current user's accounts
	}


		
	this->date = date;
	this->type = type;
	this->amount = amount;
	this->category = category;
	this->comment=comment;
}