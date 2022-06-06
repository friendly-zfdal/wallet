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
	static map<string, int> dep_type;
	static map<string, int> categories;
	static map<string, string> currencies;
	static map<string, double> exchange_rate;


public:
	static double currency_select();
	static int currencies_init();
	static double get_rate(string ISO);
	static int addCurrency(string, double);
	static map<string, double>* getCurrencies();

	static void depTypesInit(User*);
	static int add_depTypes(string, User*);
	static map<string, int>* get_depTypes();
	static string getDepType(int type);

	static int cat_init(User*);
	static string get_cat(int);
	static int cat_select(User*);
	static int add_cat(User*);
	
	static int banksInit();
	static int addBank(string);
	static int save_banks();

};

