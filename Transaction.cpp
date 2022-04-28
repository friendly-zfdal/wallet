#include "Transaction.h"

Transaction::Transaction(string date, int type, double amount, string category, string comment) {
	
	//cout<<"Categories: 1...n"<<endl;
	//cin >> category;
	
	this->date = date;
	this->type = type;
	this->amount = amount;
	this->category = category;
	this->comment=comment;
}