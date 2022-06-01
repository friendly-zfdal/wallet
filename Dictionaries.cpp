#include "Dictionaries.h"
#define bankFile "./Dictionaries/banks.json"
#define currencyFile "./Dictionaries/currencies.json"
#define depTypesFile "./Dictionaries/depTypes.json"
#define categoryFile "./Dictionaries/categories.json"


map<string, int> Dictionaries::dep_type;
// запуск python скрипта на старте, который парсит сайт с валютами, он в файл - мы в рантайм
map<string, double> Dictionaries::currencies;
// начального списка банков нет, при добавлении счетов будут добавляться и банки
map<string, int> Dictionaries::bank;
map<string, int> Dictionaries::categories;

int Dictionaries::cat_init(User* current_user) {
	fstream category;
	category.open(categoryFile);
	json cats;
	category >> cats;
	for (int i = 0; i < cats["counter"]; i++) {
		categories.insert(make_pair(cats[to_string(i)], i));
	}
	map <string, int>::iterator it;
	if (current_user->get_cats().size() != 0) {
		for (it = current_user->get_cats().begin(); it != current_user->get_cats().end(); it++) {
			categories.insert(make_pair((*it).first, (*it).second));
		}
	}
	return 0;
}

int Dictionaries::banksInit() {
	fstream bankJson;
	bankJson.open(bankFile);
	if (bankJson.peek()== std::ifstream::traits_type::eof()){}
	return 0;
}

/// in json: id - bankname; in map: bankname - id
int Dictionaries::addBank(string bankName) {
	fstream bankJson;
	json banks;
	// if dictionary isn't initialized
	if (bank.size() == 0) {
		bankJson.open(bankFile);
		//if file with banks list is empty
		if (bankJson.peek() == ifstream::traits_type::eof()) {
			bank.insert(make_pair(bankName, 0));
			banks["counter"] = 1;
			banks["0"] = bankName;
		}
		// if file with banks list isn't empty
		else {
			bankJson >> banks;
			for (int i = 0; i < banks["counter"]; i++) {
				bank.insert(make_pair(banks[to_string(i)],i));
			}
			bank.insert(make_pair(bankName, banks["counter"]-1));
			banks["counter"] += 1;
		}
	}
	//write to file
	bankJson << banks;
	bankJson.close();
	return 0;
}

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

void Dictionaries::depTypesInit() {
	//if deposit types dictionary already initialized
	if (!(Dictionaries::dep_type.empty()))
	{
		return;
	}

	std::fstream depTypesJson;
	depTypesJson.open(depTypesFile);
	json depTypes;
	if (depTypesJson.peek() == std::ifstream::traits_type::eof()) {
	depTypes["counter"] = 4;
	
	depTypes["0"] = "Cash";
	Dictionaries::dep_type.insert({ "Cash",0 });
	depTypes["1"]="Card";
	Dictionaries::dep_type.insert({"Card",1});
	depTypes["2"]="Brokerage account";
	Dictionaries::dep_type.insert({ "Brokerage account", 2 });
	depTypes["3"]="Bank deposit";
	Dictionaries::dep_type.insert({ "Bank deposit", 3 });
	depTypesJson<<depTypes;
	depTypesJson.close();
	}
	else {

		depTypesJson<<depTypes;
		for (int i = 0; i < depTypes["counter"]; i++) {
			Dictionaries::dep_type.insert(make_pair(depTypes[to_string(i)], i));
		}
	}
	
}

int Dictionaries::add_depTypes(string name) {
	std::fstream depTypesJson;
	depTypesJson.open(depTypesFile);

	json depTypes;
	if (depTypesJson.peek() == std::ifstream::traits_type::eof()) {
		depTypesInit();
	}
	depTypesJson >> depTypes;

	map<string, int> typeMap = depTypes;
	map<string, int>::iterator it;
	cout << "\nThere are already " <<depTypes["counter"]<<" types of deposits in the system : " << endl;
	//skip "counter" value
	for (it = ++typeMap.begin(); it != typeMap.end(); it++) {
		cout << it->first << ";" << endl;
	}
	cout << "Are you sure that you want to add one more? (Yes/No)"<<endl;
	string answer;
	cin >> answer;
	if (answer == "Yes") {
		depTypes[name] = depTypes["counter"];
		depTypes["counter"] += 1;
	}

	// need to check if this command override file, because it have to
	depTypesJson << depTypes;
	depTypesJson.close();

	return 1;
}

map<string, int>* Dictionaries::getBanks() {
	map<string, int> banksMap;
	std::fstream bankJson;
	bankJson.open(bankFile);
	json banks;
	bankJson >> banks;
	if (banks.empty()) {
		return NULL;
	}

	banksMap = banks["bank_list"];
	bankJson.close();
	//
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
	Dictionaries::depTypesInit();
	return &dep_type;

}

string Dictionaries::getDepType(int type) {
	Dictionaries::depTypesInit();
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

