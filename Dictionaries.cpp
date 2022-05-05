#include "Dictionaries.h"
#define bankFile "banks.json"
#define currencyFile "currencies.json"
#define depTypesFile "depTypes.json"

map<string, int> Dictionaries::dep_type;
map<string, double> Dictionaries::currencies;
map<string, int> Dictionaries::bank;

int Dictionaries::addBank(string bankName) {
	std::fstream bankJson;
	bankJson.open(bankFile);
	json banks;
	bankJson >> banks;
	if (banks.empty()) {
		banks["counter"] = 0;
	}
	banks["bank_list"][bankName] = banks["counter"] + 1;
	banks["counter"] += 1;
	// need to check if this command override file, because it have to
	bankJson << banks;
	bankJson.close();
	return 1;
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

