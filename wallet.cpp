#include <iostream>
#include <fstream>
#include <string>
#include "Card.h"
#include "User.h"
#include "Transaction.h"







int main(){
    User* client = new User("1234", "Trash", "12.04.2002");
    client->add_account("124354235", 1233, "12.04.1999", 12, 0);
    client->add_account("124354236", 1233, "12.04.1999", 12, 1);
    client->add_account("124354237", 1233, "12.04.1999", 12, 2);
    client->add_account("124354238", 1233, "12.04.1999", 12, 3);
    client->save_user();
    User* same_client = User::load_user("user.json");
    Transaction* test = new Transaction(same_client);
    Transaction* test1 = new Transaction(same_client);








//myW.diplay()
    return 0;
} 