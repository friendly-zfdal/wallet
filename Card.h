#pragma once
#include <string>

using namespace std;

class Card {
public:
	string card_id;
	string account_id;
	int bank_id;
	double money_value;
	string end_date;
	int dep_type;

public:
	Card(string card_id, string user_id, int bank_id, string end_date, double money_value, int dep_type);
	~Card();
	string get_card_info();
	double get_value();
	void add(double value);
	void dec(double value);

};