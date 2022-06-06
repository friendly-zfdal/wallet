#include "Transaction.h"
#define trans_dir "./Transactions/"
vector<Transaction*> Transaction::transactions;

string Transaction::get_time() {
	// current date/time based on current system
	time_t now = time(0);


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
	
}
// non-classic comparator
// false values will be returned for sorting in descending order
bool Transaction::compTrans(Transaction* a, Transaction* b) {
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
			buff += dateB[i];
		else {
			el_b.push_back(stoi(buff));
			buff = "";
		}
	}
	el_b.push_back(stoi(buff));
	buff = "";
	//compare years
	if (el_a[2] < el_b[2])
		/*return true;*/
		return false;
	//compare months
	else if (el_a[2] == el_b[2] && el_a[0] < el_b[0])
		/*return true;*/
		return false;
	//compare days
	else if (el_a[2] == el_b[2] && el_a[0] == el_b[0] && el_a[1] < el_b[1])
		/*return true;*/
		return false;
	//just for descending order comparator
	else if (el_a[2] == el_b[2] && el_a[0] == el_b[0] && el_a[1] == el_b[1])
		return false;
	return true;
}

bool data_cmp(vector<int> data1, vector<int> data2) {
	//compare years
	if (data1[2] < data2[2])
		return true;

	//compare months
	else if (data1[2] == data2[2] && data1[0] < data2[0])
		return true;

	//compare days
	else if (data1[2] == data2[2] && data1[0] == data2[0] && data1[1] < data2[1])
		return true;

	//just for descending order comparator
	else if (data1[2] == data2[2] && data1[0] == data2[0] && data1[1] == data2[1])
		return true;
	
	return false;
}

void Transaction::data_gather(int months)
{
	vector<Transaction*>::iterator it = transactions.begin();
	string cur_date = Transaction::get_time();
	vector<int> el_a;
	vector<int> el_b;
	string buff;
	//split dates to vector
	for (int i = 0; i < cur_date.length(); i++) {
		if (cur_date[i] != '/')
			buff += cur_date[i];
		else {
			el_a.push_back(stoi(buff));
			buff = "";
		}
	}
	el_a.push_back(stoi(buff));
	buff = "";
	
	el_a[0] -= 6;
	if (el_a[0] <= 0) {
		el_a[0] = 12 - el_a[0];
		el_a[2] = el_a[2] - 1;
	}
	string min_date = to_string(el_a[0]) + "/" + to_string(el_a[1]) + "/" + to_string(el_a[2]);
	
	string temp_date = "";
	json raw_info;
	raw_info["counter"] = 0;
	double income = 0;
	double expense = 0;
	map<int, double> data;
	map<int, double>::iterator map_it;
	while (it != transactions.end()) {
		temp_date = (*it)->date;
		for (int i = 0; i < temp_date.length(); i++) {
			if (temp_date[i] != '/')
				buff += temp_date[i];
			else {
				el_b.push_back(stoi(buff));
				buff = "";
			}
		}
		el_b.push_back(stoi(buff));
		buff = "";
		if (data_cmp(el_a, el_b)) {
			// income/expense calculations
			if ((*it)->type == 1) {
				income += (*it)->amount;
			}
			else { expense += (*it)->amount; }
			
			//gathering only expense trans by categories
			if ((*it)->type == 2) {
				map_it = data.find(stoi((*it)->category));
				if (map_it != data.end()) {
					map_it->second = map_it->second + (*it)->amount;
				}
				else {
					data.insert(make_pair(stoi((*it)->category), (*it)->amount));
				}
			}
		}
		
		// date of transactions is earlier than "current date - period in months"
		it++;
	}

	if (data.empty()) {
		cout << "There is no suitable transactions" << endl;
			return;
	}

	fstream gathered_data;
	gathered_data.open(trans_dir + "graphics.txt");
	for(map_it=data.begin();map_it!=data.end();)
}

int Transaction::Transaction_for_Stock(int type, double amount, string comment)
{
	//type 2 - buy 1 - sell;
	vector<Card*>::iterator it = Wallet::current_user->get_accounts()->begin();
	for (it; it != Wallet::current_user->get_accounts()->end(); it++) {
		if ((*it)->dep_type == 2)
			break;
	}
	Transaction* trans;
	//buy
	if (type == 2) {
		(*it)->dec(amount);
		string input_acc = "stock_buy";
		trans= new Transaction(get_time(), type, amount, "21", comment, (*it)->card_id, Dictionaries::getDepType((*it)->dep_type), input_acc, input_acc);
		std::sort(transactions.begin(), transactions.end(), compTrans);
	}
	//sell
	else if (type == 1) {
		(*it)->add(amount);
		string output_acc = "stock_sell";
		trans = new Transaction(get_time(), type, amount, "21", comment, output_acc, output_acc, (*it)->card_id, Dictionaries::getDepType((*it)->dep_type));
		std::sort(transactions.begin(), transactions.end(), compTrans);
	}


	
	
	return 0;
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
		userTransactions[it.key()]["date"];
		userTransactions[it.key()]["type"];
		userTransactions[it.key()]["amount"];
		userTransactions[it.key()]["category"];
		userTransactions[it.key()]["comment"];
		userTransactions[it.key()]["ouptut_acc"];
		userTransactions[it.key()]["output_acc_type"];
		userTransactions[it.key()]["input_acc"];
		userTransactions[it.key()]["input_acc_type"];
		Transaction* trans= new Transaction(userTransactions[it.key()]["date"], userTransactions[it.key()]["type"], userTransactions[it.key()]["amount"], userTransactions[it.key()]["category"], userTransactions[it.key()]["comment"], userTransactions[it.key()]["output_acc"], userTransactions[it.key()]["output_acc_type"],
			userTransactions[it.key()]["input_acc"], userTransactions[it.key()]["input_acc_type"]);
		transactions.push_back(trans);
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
		trans_json[to_string(counter)]["type"] = (*it)->type;
		trans_json[to_string(counter)]["date"] = (*it)->date;
		trans_json[to_string(counter)]["amount"] = (*it)->amount;
		trans_json[to_string(counter)]["category"] = (*it)->category;
		trans_json[to_string(counter)]["comment"] = (*it)->comment;
		trans_json[to_string(counter)]["output_acc"] = (*it)->output_acc;
		trans_json[to_string(counter)]["output_acc_type"] = (*it)->output_acc_type;
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
		counter = 0;
		cout << "Select one of your accounts:" << endl;
		for (auto it = current_user->get_accounts()->begin(); it != current_user->get_accounts()->end(); it++) {
			cout << to_string(counter+1) + ") " + (*it)->card_id + " - " + (*it)->get_card_info() + " - " << Dictionaries::getDepType((*it)->dep_type) << endl;
			counter++;
		}
		cin >> answer;
		auto it = current_user->get_accounts()->begin();
		for (int i = 1; i < std::stoi(answer); i++) { ++it;}
		 this->output_acc= (*it)->card_id;
		 this->output_acc_type = to_string((*it)->dep_type);
	}

	
	else {//just a plug because we're not monitoring other users accounts
		this->output_acc = "Another user's account";
		this->output_acc_type = "0";
	}

	cout << "\nHow much money was transferred?"<<endl;
	cin >> answer;
	//stod converts string to double
	this->amount = std::stod(answer);
	double rate = Dictionaries::currency_select();
	//convert amount from any currency to USD $
	this->amount = this->amount * rate;
	
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
		this->input_acc_type = to_string((*it)->dep_type);
	}

	////else to write it manually
	else {
		// now it's just a plug because we're not monitoring other users accounts
		this->input_acc = "Another user's account";
		this->input_acc_type = "0";
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

	//category selection
	this->category=to_string(Dictionaries::cat_select(Wallet::current_user));


	cout << "Do you want to add any comment for this transaction? If no - just press Enter:" << endl;
	cin.ignore(1, '\n');
	getline(cin, answer);
	this->comment = answer;

	if (current_user->check_account(this->input_acc)) {
	
		for (auto it = current_user->get_accounts()->begin(); it != current_user->get_accounts()->end(); it++) {
			if ((*it)->card_id == input_acc) {
				(*it)->money_value += amount;
				this->type = 1;
				break;
			}
		}
	}

	if (current_user->check_account(this->output_acc)) {

		for (auto it = current_user->get_accounts()->begin(); it != current_user->get_accounts()->end(); it++) {
			if ((*it)->card_id == output_acc) {
				(*it)->money_value -= amount;
				this->type = 2;
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
		cin.ignore(1, '\n');
		getline(cin, answer);
		this->date = answer;
	}

	//just adding new trans to vector in runtime instead of writing each new one to the transactions file
//	transactions.push_back(this);
	//std::sort(transactions.begin(), transactions.end(), compTrans);

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
		//list each transaction
		cout << to_string(counter + 1) + ") date:" + (*it)->date + ";output account:" + (*it)->output_acc + " - " + Dictionaries::getDepType(stoi((*it)->output_acc_type)) + "; input account:" + (*it)->input_acc + " - " + Dictionaries::getDepType(stoi((*it)->input_acc_type)) +
			"; amount" + to_string((*it)->amount) + "; category:" +/*should be cat presented as phrase*/(*it)->category + "; comment:" + (*it)->comment << endl;
		counter++;
		it++;

		if (counter == 5 || it==transactions.end()) {
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

	}


	//next page of transactions
	if (answer == 6) {
		cout << "No more transactions" << endl;
		return NULL;
	}


	//select specific transaction and return it
	advance(it, -(counter - answer + 1));
	return *it;


}

void Transaction::trans_edit(User* current_user) {
	Transaction* trans=trans_select(current_user);
	if (trans == NULL) {
		return;
	}
	//vector<Transaction*>::iterator it;
	

	cout << "What do you want to change?\n1)date\n2)output acc\n3)input acc\n4)amount\n5)category\n6)comment" << endl;
	int answer;
	cin >> answer;
	
	string new_value;
	switch (answer) {
	case 1:
		cout << "Enter the new value (in MM/DD/YYYY format):";
		cin >> new_value;
		trans->date = new_value;
		std::sort(transactions.begin(), transactions.end(), compTrans);
		break;
	case 2: {
		cout << "New output account for this transaction will be:\n1) Your account\n2) Another User's account" << endl;
		string answer;
		cin >> answer;
		int counter;
		//if the output account of the transaction is current user's account then ask to select the correct one
		if (answer == "1") {
			counter = 0;
			cout << "Select one of your accounts:" << endl;
			for (auto it = current_user->get_accounts()->begin(); it != current_user->get_accounts()->end(); it++) {
				cout << to_string(counter + 1) + ") " + (*it)->card_id + " - " + (*it)->get_card_info() + " - " << Dictionaries::getDepType((*it)->dep_type) << endl;
				counter++;
			}
			cin >> answer;
			auto it = current_user->get_accounts()->begin();
			for (int i = 1; i < std::stoi(answer); i++) { ++it; }
			trans->output_acc = (*it)->card_id;
			trans->output_acc_type = to_string((*it)->dep_type);
		}


		else {//just a plug because we're not monitoring other users accounts
			trans->output_acc = "Another user's account";
			trans->output_acc_type = "0";
		}
	}
		  break;
	case 3:
	{
		cout << "New input account for this transaction will be:\n1) Your account\n2) Another User's account" << endl;
		string answer;
		cin >> answer;
		int counter;
		//if the output account of the transaction is current user's account then ask to select the correct one
		if (answer == "1") {
			counter = 0;
			cout << "Select one of your accounts:" << endl;
			for (auto it = current_user->get_accounts()->begin(); it != current_user->get_accounts()->end(); it++) {
				cout << to_string(counter + 1) + ") " + (*it)->card_id + " - " + (*it)->get_card_info() + " - " << Dictionaries::getDepType((*it)->dep_type) << endl;
				counter++;
			}
			cin >> answer;
			auto it = current_user->get_accounts()->begin();
			for (int i = 1; i < std::stoi(answer); i++) { ++it; }
			trans->input_acc = (*it)->card_id;
			trans->input_acc_type = to_string((*it)->dep_type);
		}


		else {//just a plug because we're not monitoring other users accounts
			trans->input_acc = "Another user's account";
			trans->input_acc_type = "0";
		}
	}
	break;
	case 4:
		cout << "Enter the new value in USD ($):";
		cin >> new_value;
		trans->amount = stod(new_value);
		break;
	case 5:
		trans->category = to_string(Dictionaries::cat_select(Wallet::current_user));
		break;
	case 6:
		cout << "Enter new comment for this transaction" << endl;
		cin.ignore(1, '\n');
		getline(cin, new_value);
		trans->comment = new_value;
		break;
	default:
		cout << "incorrect input" << endl;
		return;
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