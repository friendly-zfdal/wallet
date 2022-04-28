#pragma once
#include <string>
#include "User.h"

using namespace std;

class Card {
public:
	string card_id;
	string account_id;
	int bank_id;
	double money_value;
	string end_date;

public:
	Card(string card_id, User* user, int bank_id, string end_date, double money_value);
	~Card();
	string get_card_info();
	double get_value();
	void add(double value);
	void dec(double value);

};