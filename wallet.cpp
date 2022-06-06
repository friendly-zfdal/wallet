#include "Wallet.h"
User* Wallet::current_user;
int Wallet::user_counter;

string Wallet::last_login;

Wallet::Wallet() {

	//need to add to menu last user login

	ifstream settings_f;
	settings_f.open(settings_file);
	if (settings_f.peek() == std::ifstream::traits_type::eof()) {
		Wallet::user_counter = 0;
	}
	else {
		json settings;
		settings_f >> settings;
		Wallet::user_counter = settings["user_counter"];
		Wallet::last_login = settings["last_login"];
	}
}

int Wallet::save_wallet() {
	json settings;
	settings["last_login"] = current_user->get_login();
	settings["user_counter"] = user_counter;
	ofstream settings_f;
	settings_f.open(settings_file);
	settings_f << settings;
	settings_f.close();
	return 0;
}





