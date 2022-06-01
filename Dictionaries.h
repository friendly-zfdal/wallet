#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "User.h"

using json = nlohmann::json;

using namespace std;

// class for making, save and load dictionaries with static data
class Dictionaries
{
private:
	static map<string, int> bank;
	// $ equals to 1, all other currencies = x * 1$ (current exchange rate against the dollar)
	static map<string, double> currencies;
	static map<string, int> dep_type;
	static map<string, int> categories;
public:
	static void depTypesInit();
	static int cat_init(User*);
	int banksInit();
	static int addBank(string);
	static int addCurrency(string, double);
	static int add_depTypes(string);
	static map<string, int>* getBanks();
	static map<string, double>* getCurrencies();
	static map<string, int>* get_depTypes();
	static string getDepType(int type);
	static string get_cat(int);



};

