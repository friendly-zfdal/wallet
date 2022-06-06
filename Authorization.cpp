#include "Authorization.h"
#define USERS_DIR "./Users"
#define TRANS_DIR "./Transactions"


string Authorization::loginToASCII(string login) {
    string login_ASCII;
    for (int i = 0; i < login.length(); i++) {
        login_ASCII += to_string((int)login[i]);
    }
    return login_ASCII;
}

string Authorization::loginFromASCII(string login_ASCII) {
    string login;
    int i = 0;
    while (i + 1 < login_ASCII.length()) {
       login += (char)((login_ASCII[i] - 48) * 10 + login_ASCII[i + 1] - 48);
       i += 2;
    }

    return login;
}
//should return 1 if success
int Authorization::signIn(string login, string pin)
{
    
    vector<string> files;
    string filename="";
    //load user
    for (const auto& file : filesystem::directory_iterator(USERS_DIR))
        if(file.is_regular_file())
            files.push_back(file.path().string());
    vector<string>::iterator it;
    for (it = files.begin(); it != files.end(); it++) {
        if (*it == ("./Users\\" + loginToASCII(login) + ".json")) {
            filename = *it;
            break;
        }
    }
    if (filename.length() > 0) {
        Wallet::current_user = User::load_user(filename, pin);
    }
    //load user's transactions if user exists and has provided correct pin
    if (Wallet::current_user != NULL) {
        Dictionaries::cat_init(Wallet::current_user);
        Transaction::load_trans(Wallet::current_user, pin);
    }
    else return 0;
    //Encryption::pin_encrypt(&(Wallet::current_user->pin));
    filename = "";
    files.clear();

    

    return 1;
}

//should return 1 if success
int Authorization::signUp(string login, string id, string birth_date, string name, string pin)
{
    //pin will be encrypted in constructor
    User* user = new User(id, name, birth_date, pin, login);
    Wallet::current_user = user;
    //there we'll use pin entered by user in sign up form
    user->save_user(loginToASCII(user->get_login()));
    Dictionaries::cat_init(Wallet::current_user);
    Dictionaries::depTypesInit(Wallet::current_user);
    //banks init
    
    return 1;
}
