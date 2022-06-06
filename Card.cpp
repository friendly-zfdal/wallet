#include "Card.h"



Card::Card(string card_id, string user_id, int bank_id, string end_date, double money_value, int dep_type, string alias) {
	this->card_id = card_id;
	this->bank_id = bank_id;
	this->account_id = user_id;
	this->end_date = end_date;
	this->money_value = money_value;
	// 0-cash, 1-card, 2-broker, 3- bank deposit
	this->dep_type = dep_type;
	this->alias = alias;
}

void Card::add(double value) {
	this->money_value = this->money_value + value;
}

void Card::dec(double value) {
	this->money_value = this->money_value - value;
}



double Card::get_value() {
	return this->money_value;
}

string Card::get_card_info() {
	return this->alias + " - " + to_string(this->money_value) + "$";
}
