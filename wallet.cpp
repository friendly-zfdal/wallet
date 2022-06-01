#include "Wallet.h"
User* Wallet::current_user;
int Wallet::user_counter;

Wallet::Wallet() {
	ifstream settings_f;
	settings_f.open(settings_file);
	if (settings_f.peek() == std::ifstream::traits_type::eof()) {
		Wallet::user_counter = 0;
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





//int main(){
//    Dictionaries::cat_init();
//
//
//
//
//
//
//
////myW.diplay()
//    return 0;
//} 