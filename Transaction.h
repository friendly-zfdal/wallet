#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <ctime>
#include "json.hpp"
#include "Dictionaries.h"
#include "Authorization.h"

using json = nlohmann::json;


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
	static vector<Transaction*> transactions;
	
public:
	Transaction(User* cur);
	Transaction(string date, int type, double amount, string category, string comment, string output_acc, string output_acc_type, string input_acc, string input_acc_type);

	//type: 1 - buy, 2-sell
	int static Transaction_for_Stock(int type,double amount, string comment);
	int static load_trans(User* cur, string pin);
	void static save_trans(User* cur);
	string get_date();
	string static get_time();
	void static trans_edit(User* cur);
	void static trans_delete(User* cur);
	static Transaction* trans_select(User* cur);
	static bool compTrans(Transaction* a, Transaction* b);
	static void data_gather(int months);


	


};

