#include <iostream>
#include <fstream>
#include <string>
#include "Wallet.h"
#include "Card.h"
#include "User.h"


#define FILENAME "C:\\wallet.txt"

 

void Wallet::add(double sum){

    accountCash +=sum;
    std::cout<<accountCash<<std::endl;
}

void Wallet::add(char s[]){

    std::cout<<s<<std::endl;
}

void Wallet::dec(double sum){

    accountCash -= sum;
    std::cout<<accountCash<<std::endl;
}

bool Wallet::save () {

    std::ofstream out;         // поток для записи
    out.open(FILENAME); // окрываем файл для записи
    if (out.is_open())
    {
        out << accountCash << std::endl;
        return 1;
    }

   return 0;  
}



int main(){
   // Wallet myW;
   // myW.add(100);
   // myW.dec(400);
   //  myW.add("hello");
   // myW.save();
   //int f;
   //std::cin>>f;


   User* client = new User("1234", "Pavel Gogol", "12.12.2004");
   client->add_account("21312324565", 0);
   client->add_account("21312324564", 1);
   client->add_account("21312324567", 1);
   client->add_account("21312324568", 2);
   client->add_account("21312324569", 3);




   client->save_account();

   
   Card* sber = new Card("12314", client, 12, "12.12.2008", 2000);


//myW.diplay()
    return 0;
} 