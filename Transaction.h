#pragma once
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include "json.hpp"
#include "User.h"
#include "Dictionaries.h"
#include "Card.h"
using json = nlohmann::json;
#define userTrans "UserTransactions.json"

using namespace std;

class Transaction
{
	string date;
	// 1 - add, 2 - dec
	int type;
	double amount;
	string category;
	string comment;
	string output_acc;
	string output_acc_type;
	string input_acc;
	string input_acc_type;
public:
	Transaction(User*);
	~Transaction();

	


};

