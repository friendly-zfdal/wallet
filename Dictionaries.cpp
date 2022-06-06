#include "Dictionaries.h"
#define bankFile "./Dictionaries/banks.json"
#define currencyFile "./Dictionaries/currencies.json"
#define depTypesFile "./Dictionaries/depTypes.json"
#define categoryFile "./Dictionaries/categories.json"


map<string, int> Dictionaries::dep_type;
// запуск python скрипта на старте, который парсит сайт с валютами, он в файл - мы в рантайм
map<string, string> Dictionaries::currencies;
map<string, double> Dictionaries::exchange_rate;
// начального списка банков нет, при добавлении счетов будут добавляться и банки
map<string, int> Dictionaries::bank;
map<string, int> Dictionaries::categories;


//start init for cats, after first execution all standard and custom cats will be stored in user file
int Dictionaries::cat_init(User* current_user) {

	if(current_user->get_cats()->size() == 0) {
		fstream category;
		category.open(categoryFile);
		json cats;
		category >> cats;
		for (int i = 0; i < cats["counter"]; i++) {
			categories.insert(make_pair(cats[to_string(i)], i));
			current_user->get_cats()->insert(make_pair(cats[to_string(i)], i));
		}
		return 0;
	}

	map <string, int>::iterator it;
	if (current_user->get_cats()->size() != 0) {
		for (it = current_user->get_cats()->begin(); it != current_user->get_cats()->end(); it++) {
			categories.insert(make_pair((*it).first, (*it).second));
		}
	}
	return 0;
}

int Dictionaries::banksInit() {
	fstream bankJson;
	//first opening for checking if file exists, in other case it will create it
	bankJson.open(bankFile, ios::out);
	bankJson.close();
	bankJson.open(bankFile);


	if (bankJson.peek() != std::ifstream::traits_type::eof()) {
		json banks;
		bankJson >> banks;
		for (int i = 0; i < banks["counter"]; i++) {
			bank.insert(make_pair(banks[to_string(i)], i));
		}
	}
	bankJson.close();

	return 0;
}

int Dictionaries::save_banks() {
	fstream fs;
	//if file doesn't exists - create it, in other case - just open and close
	fs.open(bankFile, std::ios::out);
	fs.close();
	fs.open(bankFile);
	json bankJson;
	bankJson["counter"] = bank.size();
	map<string, int>::iterator it;
	for (it = bank.begin(); it != bank.end(); it++) {
		bankJson[to_string(it->second)] = it->first;
	}
	fs << bankJson;
	fs.close();

	return 0;
}

int Dictionaries::addBank(string bankName) {
	fstream bankJson;
	json banks;
	int bank_id;
	// if dictionary isn't initialized
	if (banks.size() == 0) {
		banksInit();
	}
	//search for bankName
	map<string, int>::iterator it = bank.find(bankName);
	//if found - return known id
	if (it != bank.end()) {
		bank_id = it->second;
	}
	//if not - add to map
	else {
		bank.insert(make_pair(bankName, bank.size()));
		bank_id = bank.size()-1;
	}
	//save them not to lose in case if we will have some incorrect behaviour
	save_banks();
	return bank_id;
}

// we don't need it, cause we can't update real exchange rate of currency not included in python script
int Dictionaries::addCurrency(string Currency, double value) {
	std::fstream currencyJson;
	currencyJson.open(currencyFile);
	json currencies;
	currencyJson >> currencies;
	if (currencies.empty()) {
		//currencies["$"] = 1;
	}
	currencies[Currency] = value;
	
	// need to check if this command override file, because it have to
	currencyJson << currencies;
	currencyJson.close();
	return 1;
}

double Dictionaries::get_rate(string ISO)
{
	return exchange_rate[ISO];
}

double Dictionaries::currency_select() {
	currencies_init();
	cout << "Currency select:" << endl;
	int answer;
	string currency;
	int counter = 0;
	map<string, string>::iterator it=currencies.begin();
	//select the currency from the list
	while (it != currencies.end()) {

		cout << to_string(counter+1) +") " + it->first + " - " + it->second << endl;
		counter++;
		it++;
		if (counter == 5 || it==currencies.end()) {
			cout << "Select one of the listed currencies, press 6 to move to the next page or '7' to exit selection menu:";
			cin.ignore(1, '\n');
			cin >> answer;
			if (answer == 7)
				return -1;
			else if (answer == 6) {
				counter = 0;
				continue;
			}
			else break;
		}
	}

	if (answer == 6) {
		cout << "\nNo more categories" << endl;
		return -1;
	}
	if (answer < 1 || answer > 5) {
		cout << "incorrect input, try one more time" << endl;
		return -1;
	}

	advance(it, -(counter - answer + 1));
	currency = it->first;

	//return it exchange rate to USD $$$$$$$
	return get_rate(currency);

}

int Dictionaries::currencies_init()
{
	if (currencies.size() != 0) {
		return 0;
	}
	system("python ./Transactions/exchange_rate.py");
	fstream rates;
	rates.open("./Transactions/exchange_rate.txt");
	string ISO;
	double rate;
	while (rates >> ISO) {
		rates >> rate;
		exchange_rate.insert(make_pair(ISO, rate));
	}
	rates.close();
	fstream ISOs;
	ISOs.open("./Transactions/currenciesISOnNames.txt");
	string ISO_name;
	string delim = ":";
	while (getline(ISOs, ISO_name)) {
		currencies.insert(make_pair(ISO_name.substr(0, ISO_name.find(delim)), ISO_name.substr(ISO_name.find(delim)+1, ISO_name.length() - 1)));
	}
	return 0;
}

void Dictionaries::depTypesInit(User* current_user) {
	//if deposit types user's dictionary already initialized
	if (current_user->get_types()->size()!=0)
	{
		return;
	}
	
	Dictionaries::dep_type.insert({ "Cash",0 });
	current_user->get_types()->insert({ "Cash",0 });

	Dictionaries::dep_type.insert({ "Card",1 });
	current_user->get_types()->insert({ "Card",1 });

	Dictionaries::dep_type.insert({ "Brokerage account",2 });
	current_user->get_types()->insert({ "Brokerage account",2 });

	Dictionaries::dep_type.insert({ "Bank deposit",3 });
	current_user->get_types()->insert({ "Bank deposit",3 });




	//std::fstream depTypesJson;
	//depTypesJson.open(depTypesFile, ios::out);
	//depTypesJson.close();
	//depTypesJson.open(depTypesFile);
	//json depTypes;
	//map<string, int>* types = current_user->get_types();
	//if (depTypesJson.peek() == std::ifstream::traits_type::eof()) {
	//depTypes["counter"] = 4;
	//
	//depTypes["0"] = "Cash";
	//Dictionaries::dep_type.insert({ "Cash",0 });
	//
	//depTypes["1"]="Card";
	//Dictionaries::dep_type.insert({"Card",1});
	//depTypes["2"]="Brokerage account";
	//Dictionaries::dep_type.insert({ "Brokerage account", 2 });
	//depTypes["3"]="Bank deposit";
	//Dictionaries::dep_type.insert({ "Bank deposit", 3 });
	//depTypesJson<<depTypes;
	//depTypesJson.close();
	//}
	//else {

	//	depTypesJson<<depTypes;
	//	for (int i = 0; i < depTypes["counter"]; i++) {
	//		types->insert(make_pair(depTypes[to_string(i)], i));
	//	}
	//}
	
}

int Dictionaries::add_depTypes(string name, User* current_user) {

	//init user's dep_types if they still haven't been 
	if (current_user->get_types()->size()==0) {
		depTypesInit(current_user);
	}
	map<string, int>* types = current_user->get_types();
	map<string, int>::iterator it = types->begin();
	cout << "\nThere are already " <<to_string(types->size()) << " types of deposits in the system : " << endl;
	
	int counter = 1;
	while (it != types->end()) {
		cout << to_string(counter++) + ") " + it->first << endl;
		it++;
	}


	cout << "Are you sure that you want to add one more? (Yes/No)"<<endl;
	string answer;
	cin >> answer;
	if (answer == "Yes") {
		types->insert(make_pair(name, types->size()));
		dep_type.insert(make_pair(name, dep_type.size()));
	}

	return 1;
}



map<string, double>* Dictionaries::getCurrencies() {

	std::fstream currencyJson;
	currencyJson.open(currencyFile);
	json currencies_list;
	currencyJson >> currencies_list;
	if (currencies_list.empty()) {
		return NULL;
	}
	
	//map<string, double> currenciesMap=currencies;

	currencyJson.close();
	//return &Dictionaries::currencies;
}

map<string, int>* Dictionaries::get_depTypes() {
	return &dep_type;
}

// select string value by int value for dep_type
string Dictionaries::getDepType(int type) {
	map<string, int>::iterator it;

	for (it = dep_type.begin(); it != dep_type.end(); it++) {
		if (it->second == type)
			return  it->first;
	}
	return "";
}

string Dictionaries::get_cat(int cat)
{
	map<string, int>::iterator it=categories.begin();
	while (it != categories.end()) {
		if (it->second == cat) {
			return it->first;
		}
	}
	return string();
}

int Dictionaries::cat_select(User* current_user) {

	cout << "Category select:" << endl;
		cat_init(current_user);
	int answer;
	string currency;
	int counter = 0;
	map<string, int>::iterator it = categories.begin();
	//select the category from the list
	while (it != categories.end()) {
		cout << to_string(counter + 1) + ") " + it->first << endl;
		counter++;
		it++;
		if (counter == 5 || it == categories.end()) {
			cout << "\nSelect one of the listed categories, press 6 to move to the next page or '7' to exit selection menu:";
			cin >> answer;
			if (answer == 7)
				return -1;
			else if (answer == 6) {
				counter = 0;
				continue;
			}
			else break;
		}
	}

	if (answer == 6) {
		cout << "\nNo more categories" << endl;
		return -1;
	}
	if (answer < 1 || answer > 5) {
		cout << "incorrect input, try one more time" << endl;
		return -1;
	}

	advance(it, -(counter - answer + 1));
	return it->second;

}

int Dictionaries::add_cat(User* current_user)
{
	current_user->get_cats();
	return 0;
}


