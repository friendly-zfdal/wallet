#include "Card.h"


Card::Card(string card_id, User* user, int bank_id, string end_date, double money_value) {
	this->card_id = card_id;
	this->bank_id = bank_id;
	this->account_id = user->get_id();
	this->end_date = end_date;
	this->money_value = money_value;
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

string get_card_info() {
	
	return "Card Info";
}
