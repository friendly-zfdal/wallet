#pragma once
#include <string>
#include <iostream>

using namespace std;

class Transaction
{
	string date;
	// 1 - add, 2 - dec
	int type;
	double amount;
	string category;
	string comment;
public:
	Transaction(string date, int type, double amount, string category, string comment);
	~Transaction();
	int CardAddTrans(int card_id, double value);
	int CardDecTrans(int card_id, double value);
	int StockAddTrans();
	int StockDecTrans();
	


};

