#pragma once
#include <string>
#include <iostream>

using namespace std;

class Card {
public:
	///
	///?
	string alias;
	string card_id;
	//user id
	string account_id;
	int bank_id;
	double money_value;
	string end_date;
	int dep_type;

public:
	//относительно user-a конструктор надо сделать
	Card(string card_id, string user_id, int bank_id, string end_date, double money_value, int dep_type, string alias);
	~Card();
	string get_card_info();
	double get_value();
	void add(double value);
	void dec(double value);

};