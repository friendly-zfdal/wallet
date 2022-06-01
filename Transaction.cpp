#include "Transaction.h"
#define trans_dir "./Transactions/"
vector<Transaction*> Transaction::transactions;

string Transaction::get_time() {
	// current date/time based on current system
	time_t now = time(0);

	cout << "Number of sec since January 1,1970 is:: " << now << endl;

	tm* ltm = localtime(&now);
	string current_date;
	string hour;
	string min ;
	string sec ;
	if (to_string(ltm->tm_hour).length() == 1) {
		hour = "0" + to_string(ltm->tm_hour);
	}
	else { hour = to_string(ltm->tm_hour); }
	if (to_string(ltm->tm_min).length() == 1) {
		min = "0" + to_string(ltm->tm_min);
	}
	else { min = to_string(ltm->tm_min); }
	if (to_string(ltm->tm_sec).length() == 1) {
		sec = "0" + to_string(ltm->tm_sec);
	}
	else { sec = to_string(ltm->tm_sec); }
	
	current_date = to_string(1 + ltm->tm_mon) + "/" + to_string(ltm->tm_mday) + "/" + to_string(1900 + ltm->tm_year)/* + " " + hour + ":" + min + ":" + sec*/;
	
	return current_date;

}

string Transaction::get_date() {
	return this->date;
}
Transaction::Transaction(string date, int type, double amount, string category, string comment, string output_acc, string output_acc_type, string input_acc, string input_acc_type) {
	this->date = date;
	this->type = type;
	this->amount = amount;
	this->category = category;
	this->comment = comment;
	this->output_acc = output_acc;
	this->output_acc_type = output_acc_type;
	this->input_acc = input_acc;
	this->input_acc_type = input_acc_type;
	transactions.push_back(this);
}

int Transaction::Transaction_for_Stock(int type, double amount, string comment)
{
	//type 2 - buy 1 - sell;
	vector<Card*>::iterator it = Wallet::current_user->get_accounts()->begin();
	for (it; it != Wallet::current_user->get_accounts()->end(); it++) {
		if ((*it)->dep_type == 2)
			break;
	}
	//buy
	if (type == 2) {
		(*it)->dec(amount);
		string input_acc = "stock_buy";
		Transaction(get_time(), type, amount, Dictionaries::get_cat(21), comment, (*it)->card_id, Dictionaries::getDepType((*it)->dep_type), input_acc,input_acc);

	}
	//sell
	else if (type == 1) {
		(*it)->add(amount);
		string output_acc = "stock_sell";
		Transaction(get_time(), type, amount, Dictionaries::get_cat(21), comment, output_acc, output_acc, (*it)->card_id, Dictionaries::getDepType((*it)->dep_type));
	}


	
	
	return 0;
}

// non-classic comparator
// false values will be returned for sorting in descending order
bool compTrans(Transaction* a, Transaction* b) {
	string dateA = a->get_date();
	string dateB = b->get_date();
	vector<int> el_a;
	vector<int> el_b;
	string buff;
	//split dates to vector
	for (int i = 0; i < dateA.length(); i++) {
		if (dateA[i] != '/')
			buff += dateA[i];
		else {
			el_a.push_back(stoi(buff));
			buff = "";
		}
	}
	el_a.push_back(stoi(buff));
	buff = "";

	for (int i = 0; i < dateB.length(); i++) {
		if (dateB[i] != '/')
			buff += dateA[i];
		else {
			el_b.push_back(stoi(buff));
			buff = "";
		}
	}
	el_b.push_back(stoi(buff));
	buff = "";
	//compare years
	if (el_a[2] < el_b[2])
		return false;
	//compare months
	else if (el_a[3] == el_b[3] && el_a[0] < el_b[0])
		return false;
	//compare days
	else if (el_a[3] == el_b[3] && el_a[0] == el_b[0] && el_a[1] < el_b[1])
		return false;

	return true;
}

int Transaction::load_trans(User* current_user, string pin) {
	string userTrans = trans_dir + Authorization::loginToASCII(current_user->get_login()) + "Trans.json";
	//decrypt and load user's transactions (we already know that pin is correct)
	Encryption::file_decrypt(userTrans, pin);
	std::fstream transactionsJson;
	transactionsJson.open(userTrans);
	json userTransactions;
	if (transactionsJson.peek() == std::ifstream::traits_type::eof()) {
		return 0;
	}
	else {
		transactionsJson >> userTransactions;
	}
	transactionsJson.close();

	//encrypt after all loading OPs
	Encryption::file_encrypt(userTrans, pin);

	//test, maybe need [key, val]
	for (auto it = userTransactions.begin(); it != userTransactions.end();it++) {
		if (it.key() == "counter")
			continue;
		Transaction(userTransactions[it.key()]["date"], userTransactions[it.key()]["type"], userTransactions[it.key()]["amount"], userTransactions[it.key()]["category"], userTransactions[it.key()]["comment"], userTransactions[it.key()]["ouptut_acc"], userTransactions[it.key()]["output_acc_type"],
			userTransactions[it.key()]["input_acc"], userTransactions[it.key()]["input_acc_type"]);
	}
	//sort after all transactions have been added
	std::sort(transactions.begin(), transactions.end(), compTrans);

	return 1;
}
void Transaction::save_trans(User* cur)
{
	string userTrans = trans_dir + Authorization::loginToASCII(cur->get_login()) + "Trans.json";
	std::ofstream test;
	test.open(userTrans);
	vector<Transaction*>::iterator it;
	json trans_json;
	int counter=0;
	//running through preloaded transactions vector
	for (it = transactions.begin(); it != transactions.end(); it++) {

	trans_json[to_string(counter)]["date"] = (*it)->date;
	trans_json[to_string(counter)]["amount"] = (*it)->amount;
	trans_json[to_string(counter)]["category"] = (*it)->category;
	trans_json[to_string(counter)]["comment"] = (*it)->comment;
	trans_json[to_string(counter)]["output_acc"] = (*it)->output_acc;
	trans_json[to_string(counter)]["output_acc"] = (*it)->output_acc_type;
	trans_json[to_string(counter)]["input_acc"] = (*it)->input_acc;
	trans_json[to_string(counter)]["input_acc_type"] = (*it)->input_acc_type;
	counter++;
	}
	trans_json["counter"] = counter;
	test << trans_json;
	test.close();

	// trans file encryption
	Encryption::pin_decrypt(&(Wallet::current_user->pin));
	Encryption::file_encrypt(userTrans, Wallet::current_user->pin);
	Encryption::pin_encrypt(&(Wallet::current_user->pin));
}


// привязка Usera к какой-то глобал переменной либо в настройках, чтобы можно было использовать сущность текущего юзера
Transaction::Transaction(User* current_user) {

	if (transactions.size() == 0) {
		Encryption::pin_decrypt(&(current_user->pin));
		load_trans(current_user, current_user->pin);
		Encryption::pin_encrypt(&(current_user->pin));

	}

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

	
	else {//just a plug because we're not monitoring other users accounts
		this->output_acc = "Another user's account";
		this->output_acc_type = "0";
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
		// now it's just a plug because we're not monitoring other users accounts
		this->output_acc = "Another user's account";
		this->output_acc_type = "0";
		//cout << "Please, enter transcation's output account manually:" << endl;
		//cin >> answer;
		//this->input_acc = answer;
		//counter = 1;
		//cout << "\nand select type of the account:" << endl;
		//map<string, int>::iterator it;
		//map<string, int>* types = Dictionaries::get_depTypes();
		//for (it = (*types).begin(); it != (*types).end(); it++) {
		//	//*dep_type_num*) *dep_type_name*
		//	cout << it->second << ") " << it->first << endl;
		//}
		//cin >> answer;
		//this->input_acc_type = answer;
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
	
	cout << "\nDo you want to enter the date when the transaction was performed? \n1)Current date \n2)Enter manually\n";
	cin >> answer;
	if (answer == "1") {
		this->date = get_time();
	}
	else {
		cout << "\nWrite it manually in format MM/DD/YYYY HH/MM in 24-hour clock format" << endl;
		getline(cin, answer);
		this->date = answer;
	}

	//just adding new trans to vector in runtime instead of writing each new one to the transactions file
	transactions.push_back(this);
	std::sort(transactions.begin(), transactions.end(), compTrans);

	//std::ofstream test;
	//test.open(userTrans);
	//userTransactions[to_string(userTransactions["counter"])]["date"] = this->date;
	//userTransactions[to_string(userTransactions["counter"])]["amount"] = this->amount;
	//userTransactions[to_string(userTransactions["counter"])]["category"] = this->category;
	//userTransactions[to_string(userTransactions["counter"])]["comment"] = this->comment;
	//userTransactions[to_string(userTransactions["counter"])]["output_acc"] = this->output_acc;
	//userTransactions[to_string(userTransactions["counter"])]["output_acc"] = this->output_acc_type;
	//userTransactions[to_string(userTransactions["counter"])]["input_acc"] = this->input_acc;
	//userTransactions[to_string(userTransactions["counter"])]["input_acc_type"] = this->input_acc_type;
	//userTransactions["counter"] =userTransactions["counter"]+1;
	//test << userTransactions;
	//test.close();


	
}

Transaction* Transaction::trans_select(User* current_user) {
	//check if we have any transactions for this user
	if (transactions.size() == 0) {
		Encryption::pin_decrypt(&(current_user->pin));
		load_trans(current_user, current_user->pin);
		Encryption::pin_encrypt(&(current_user->pin));
		if (transactions.size() == 0) {
			cout << "There are no transactions yet for your accounts\n";
			return NULL;
		}
	}

	//all transactions should be sorted to the moment when we will launch this function
	vector<Transaction*>::iterator it = transactions.begin();
	vector<Transaction*>::iterator temp_it;
	vector<Transaction*> temp;
	int counter = 0;
	int answer;
	while (it != transactions.end()) {
		if (counter == 5) {
			cout << "Select one of the listed transactions on this page, press '6' to move to the next page or '7' to exit selection menu:";
			
			cin >> answer;
			//exit
			if (answer == 7)
				return NULL;
			//next page of transactions
			else if (answer == 6) {
				counter = 0;
				continue;
			}
			//user has selected one of listed transactions, so we need to return selected one
			else break;
		}

		//list each transaction
		cout << counter + 1 + ") date:" + (*it)->date + ";output account:" + (*it)->output_acc + " - " + Dictionaries::getDepType(stoi((*it)->output_acc_type)) + "; input account:" + (*it)->input_acc + " - " + Dictionaries::getDepType(stoi((*it)->input_acc_type)) +
			"; amount" + to_string((*it)->amount) + "; category:" +/*should be cat presented as phrase*/(*it)->category + "; comment:" + (*it)->comment << endl;

		counter++;
		it++;
	}

	//select specific transaction and return it
	switch (answer) {
	case 1:
		return *it -= 5;
	case 2:
		return *it -= 4;
	case 3:
		return *it -= 3;
	case 4:
		return *it -= 2;
	case 5:
		return *it -= 1;
	case 6:
		return NULL;
	default:
		cout << "The input is incorrect" << endl;
		return NULL;
	}



	return NULL;
}

void Transaction::trans_edit(User* current_user) {
	Transaction* trans=trans_select(current_user);
	cout << "What do you want to change?\n1)date\n2)output acc\n3)input acc\n4)amount\n5)category\n6)comment" << endl;
	int answer;
	cin >> answer;
	cout << "Enter the new value:";
	string new_value;
	switch (answer) {
		case 1:
			cin >> new_value;
			trans->date = new_value;
			break;
		case 2:
			cin >> new_value;
			trans->output_acc = new_value;
			if (current_user->check_account(new_value)) {
				vector<Card*> accounts = *((*current_user).get_accounts());
				vector<Card*>::iterator it;
				
				for (it = accounts.begin(); it != accounts.end(); it++) {
					if ((*it)->account_id == new_value) {
						trans->output_acc_type = (*it)->dep_type;
					}
				}
			}
			break;
		case 3:
			cin >> new_value;
			trans->input_acc = new_value;
			if (current_user->check_account(new_value)) {
				vector<Card*> accounts = *((*current_user).get_accounts());
				vector<Card*>::iterator it;

				for (it = accounts.begin(); it != accounts.end(); it++) {
					if ((*it)->account_id == new_value) {
						trans->input_acc_type = (*it)->dep_type;
					}
				}
			}
			break;
		case 4:
			cin >> new_value;
			trans->amount = stod(new_value);
			break;
		case 5: {
			map<string, int> cats = current_user->get_cats();
			map<string, int>::iterator map_it;
			int counter = 0;
			//print cats
			for (map_it = cats.begin(); map_it != cats.end(); map_it++) {
				cout << counter + 1 + ") " + map_it->first << endl;
				counter++;
			}
			cin >> new_value;
			//
			for (map_it = cats.begin(); map_it != cats.end(); map_it++) {
				if (counter == (stoi(new_value) - 1)) {
					trans->category = map_it->second;
					break;
				}
				counter++;
			}
		}
			break;
		case 6:
			cin.ignore(1, '\n');
			getline(cin, new_value);
			trans->comment = new_value;
			break;
	}



}

void Transaction::trans_delete(User* current_user) {
	Transaction* ptr_toDelete=trans_select(current_user);
	vector<Transaction*>::iterator it;
	for (it = transactions.begin(); it != transactions.end(); it++) {
		if (*it == ptr_toDelete) {
			transactions.erase(it);
			cout << "Transaction has been successfully deleted\n";
				return;
		}
	}
	cout << "Something went wrong. Transaction hasn't been deleted yet.\n";
	return;
	
}