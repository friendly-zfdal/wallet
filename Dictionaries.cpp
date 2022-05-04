#include "Dictionaries.h"
#define bankFile "banks.json"
#define currencyFile "currencies.json"
#define depTypesFile "depTypes.json"
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
		currencies["$"] = 1;
	}
	currencies[Currency] = value;
	
	// need to check if this command override file, because it have to
	currencyJson << currencies;
	currencyJson.close();
	return 1;
}

int Dictionaries::add_depTypes(string name) {
	std::fstream depTypesJson;
	depTypesJson.open(currencyFile);
	json depTypes;
	depTypesJson >> depTypes;
	if (depTypes.empty()) {
		depTypes["counter"] = 4;
		depTypes["Cash"] = 0;
		depTypes["Card"] = 1;
		depTypes["Brokerage account"] = 2;
		depTypes["Bank deposit"] = 3;
	}
	map<string, int> typeMap = depTypes;
	map<string, int>::iterator it;
	cout << "\nThere are already " <<depTypes["counter"]<<" types of deposits in the system : " << endl;
	//skip "counter" value
	for (it = ++typeMap.begin(); it != typeMap.end(); it++) {
		cout << it->first << ";" << endl;
	}
	cout << "Are you sure that you want to add one more? (Yes/No)";
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
	return &banksMap;
}

map<string, double>* Dictionaries::getCurrencies() {

	std::fstream currencyJson;
	currencyJson.open(currencyFile);
	json currencies;
	currencyJson >> currencies;
	if (currencies.empty()) {
		return NULL;
	}
	
	map<string, double> currenciesMap=currencies;

	currencyJson.close();
	return &currenciesMap;
}

map<string, int>* Dictionaries::get_depTypes() {
	std::fstream depTypesJson;
	depTypesJson.open(currencyFile);
	json depTypes;
	depTypesJson >> depTypes;
	if (depTypes.empty()) {
		return NULL;
	}

	map<string, int> typeMap (++(depTypes.begin()), depTypes.end());

	return &typeMap;
}

string Dictionaries::getDepType(int type) {
	map<string, int>* types = get_depTypes();
	map<string, int>::iterator it;
	for (it = (*types).begin(); it != (*types).end(); it++) {
		if (it->second == type)
			return  it->first;
	}
	return NULL;
}

