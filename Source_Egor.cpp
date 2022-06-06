#include <iostream>
#include <fstream>
#include <string>
#include "Source_Egor.h"
#include "Wallet.h"
#include "Transaction.h"


using std::fstream;
using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::to_string;
using std::ios;

int choice_1 = 0;
int choice_2 = 3;

string selected_user = Wallet::current_user->get_login();

void Stock::allStock()
{
	fstream as("Stock/AllStocks.csv", std::ios::in);

	string index;
	string company_name;
	string shares;
	string share_price;

	while (as.peek() != EOF)
	{
		getline(as, index, ';');
		getline(as, company_name, ';');
		getline(as, shares, ';');
		getline(as, share_price, '\n');

		cout << index << ". Company: " << company_name << "; Shares: " << shares << "; Share Value: " << share_price << endl;
	}
	as.close();
}

void Stock::userShares()
{
	fstream us("Stock/UserShares.csv", std::ios::in);

	string index;
	string user;
	string company_name;
	string shares;
	string share_price;
	string total_value;
	int i = 0;

	while (us.peek() != EOF)
	{
		getline(us, index, ';');
		getline(us, user, ';');
		getline(us, company_name, ';');
		getline(us, shares, ';');
		getline(us, share_price, ';');
		getline(us, total_value, '\n');

		if (selected_user == user)
		{
			i++;
			index = to_string(i);
			cout << index << ". Company: " << company_name << "; Shares: " << shares << "; Share Value: " << share_price << "; Total Value: " << total_value << endl;
		}
	}
	us.close();
}

void Stock::totalValue()
{
	fstream us("Stock/UserShares.csv", std::ios::in);

	string index;
	string user;
	string company_name;
	string shares;
	string share_price;
	string total_value;
	double total_value_1;
	double total_value_2 = 0;


	while (us.peek() != EOF)
	{
		getline(us, index, ';');
		getline(us, user, ';');
		getline(us, company_name, ';');
		getline(us, shares, ';');
		getline(us, share_price, ';');
		getline(us, total_value, '\n');

		if (selected_user == user)
		{
			total_value_1 = stol(shares) * stod(share_price);
			total_value_2 += total_value_1;
		}
	}
	us.close();
	cout << "Total Value: " << total_value_2 << endl;
}

void Stock::buy()
{
	vector<Card*> test = *Wallet::current_user->get_accounts();
	vector<Card*>::iterator it = test.begin();
	while (it != test.end())
	{
		if ((*it)->dep_type == 2)
			break;
	}
	double amount = (*it)->get_value();
	string selected_index;
	long purchased_shares;
	double purchased_share_price;
	string user;
	string selected_company_name;
	double total_value;
	string comment;
	int i = 0;
	long* purchased_shares_pointer = NULL;
	double* purchase_share_price_pointer = NULL;

	allStock();

	cout << "Enter the number of requirement company: ";
	cin >> selected_index;

	fstream as("Stock/AllStocks.csv", ios::in);
	fstream ns("Stock/NewStocks.csv", ios::out);
	fstream us("Stock/UserShares.csv", ios::app);
	fstream nus("Stock/NewUserShares.csv", ios::out);

	string index;
	string company_name;
	string shares;
	string share_price;
	string total_value_1;

	while (as.peek() != EOF)
	{
		getline(as, index, ';');
		getline(as, company_name, ';');
		getline(as, shares, ';');
		getline(as, share_price, '\n');

		if (selected_index == index)
		{
			cout << index << ". Company: " << company_name << "; Shares: " << shares << "; Share Value: " << share_price << endl;
			cout << "Enter the number of shares: ";
			cin >> purchased_shares;
			if (cin.fail())
			{
				cout << "Incorrect input" << endl;
				cin.clear();
				while (cin.get() != '\n');
			}
			else
			{
				purchased_shares_pointer = &purchased_shares;
				if (purchased_shares > stol(shares))
				{
					cout << "Sorry, the number of shares exceeds the current value" << endl;
				}
				else if (purchased_shares < 0)
				{
					cout << "Please, enter a valid number" << endl;
				}
				else
				{
					cout << "Write the share price: ";
					cin >> purchased_share_price;
					if (cin.fail())
					{
						cout << "Incorrect input" << endl;
						cin.clear();
						while (cin.get() != '\n');
					}
					else
					{
						purchase_share_price_pointer = &purchased_share_price;
						if (purchased_share_price < stol(share_price))
						{
							cout << "Sorry, share price cannot be less than the current" << endl;
						}
						else
						{
							total_value = purchased_shares * purchased_share_price;
							if (amount >= total_value)
							{
								long shares_1 = stol(shares);
								shares_1 -= purchased_shares;
								shares = to_string(shares_1);
								amount -= total_value;
								us << i << ';' << selected_user << ';' << company_name << ';' << purchased_shares << ';' << purchased_share_price << ';' << total_value << '\n';
								comment = company_name + "; Shares: " + to_string(purchased_shares) + "; Share Price:" + to_string(purchased_share_price);
								Transaction::Transaction_for_Stock(1, total_value, comment);
								selected_company_name = company_name;
							}
							else
							{
								cout << "Sorry, not enough money" << endl;
							}
						}
					}
				}
			}
			ns << index << ';' << company_name << ';' << shares << ';' << share_price << '\n';
		}
		else
		{
			ns << index << ';' << company_name << ';' << shares << ';' << share_price << '\n';
		}
	}
	ns.close();
	as.close();
	us.close();
	nus.close();

	us.open("Stock/UserShares.csv", ios::in);
	nus.open("Stock/NewUserShares.csv", ios::out);

	while (us.peek() != EOF)
	{
		getline(us, index, ';');
		getline(us, user, ';');
		getline(us, company_name, ';');
		getline(us, shares, ';');
		getline(us, share_price, ';');
		getline(us, total_value_1, '\n');
		if (selected_user == user)
		{
			i++;
			nus << i << ";" << selected_user << ';' << company_name << ';' << shares << ';' << share_price << ';' << total_value_1 << '\n';
		}
		else
		{
			nus << index << ";" << user << ';' << company_name << ';' << shares << ';' << share_price << ';' << total_value_1 << '\n';
		}
	}
	us.close();
	nus.close();

	us.open("Stock/UserShares.csv", ios::out);
	nus.open("Stock/NewUserShares.csv", ios::in);

	while (nus.peek() != EOF)
	{
		getline(nus, index, ';');
		getline(nus, user, ';');
		getline(nus, company_name, ';');
		getline(nus, shares, ';');
		getline(nus, share_price, ';');
		getline(nus, total_value_1, '\n');

		us << index << ";" << user << ';' << company_name << ';' << shares << ';' << share_price << ';' << total_value_1 << '\n';
	}
	us.close();
	nus.close();
	remove("Stock/NewUserShares.csv");

	as.open("Stock/AllStocks.csv", ios::out);
	ns.open("Stock/NewStocks.csv", ios::in);

	while (ns.peek() != EOF)
	{
		getline(ns, index, ';');
		getline(ns, company_name, ';');
		getline(ns, shares, ';');
		getline(ns, share_price, '\n');
		as << index << ';' << company_name << ';' << shares << ';' << share_price << '\n';
	}
	ns.close();
	as.close();
	remove("Stock/NewStocks.csv");
}

void Stock::sell()
{
	vector<Card*> test = *Wallet::current_user->get_accounts();
	vector<Card*>::iterator it = test.begin();
	while (it != test.end())
	{
		if ((*it)->dep_type == 2)
			break;
	}
	double amount = (*it)->get_value();
	string selected_index;
	long tradable_shares;
	double tradable_share_price;
	string selected_company_name;
	double avg_share_price;
	string comment;
	long* tradable_shares_pointer = NULL;
	double* avg_share_price_pointer = NULL;

	string type = "Sell";

	userShares();

	cout << "Enter the number of requirement company: ";
	cin >> selected_index;

	fstream as("Stock/AllStocks.csv", ios::in);
	fstream ns("Stock/NewStocks.csv", ios::out);
	fstream us("Stock/UserShares.csv", ios::in);
	fstream nus("Stock/NewUserShares.csv", ios::out);

	string index;
	string user;
	string company_name;
	string shares;
	string share_price;
	string total_value;

	while (us.peek() != EOF)
	{
		getline(us, index, ';');
		getline(us, user, ';');
		getline(us, company_name, ';');
		getline(us, shares, ';');
		getline(us, share_price, ';');
		getline(us, total_value, '\n');
		if (selected_user == user)
		{
			if (selected_index == index)
			{
				cout << index << ". Company: " << company_name << "; Shares: " << shares << "; Share Value: " << share_price << endl;
				cout << "Enter the number of shares: ";
				cin >> tradable_shares;
				if (cin.fail())
				{
					cout << "Incorrect input" << endl;
					cin.clear();
					while (cin.get() != '\n');
				}
				else
				{
					tradable_shares_pointer = &tradable_shares;
					if (tradable_shares > stol(shares))
					{
						cout << "Sorry, the number of shares exceeds the current value" << endl;
					}
					else if (tradable_shares < 0)
					{
						cout << "Please, enter a valid number" << endl;
					}
					else
					{
						long shares_1 = stol(shares);
						shares_1 -= tradable_shares;
						shares = to_string(shares_1);
					}
					cout << "Write the share price: ";
					cin >> tradable_share_price;
					if (cin.fail())
					{
						cout << "Incorrect input" << endl;
						cin.clear();
						while (cin.get() != '\n');
					}
					else
					{
						if (tradable_share_price < 0)
						{
							cout << "Please, enter a valid number" << endl;
						}
						double profit = (tradable_share_price - stod(share_price)) * tradable_shares;
						avg_share_price = (stod(share_price) + tradable_share_price) / 2;
						avg_share_price_pointer = &avg_share_price;
						double total_value_1 = stol(shares) * avg_share_price + profit;
						double total_value_2 = tradable_shares * tradable_share_price;
						amount += total_value_1;
						comment = company_name + "; Shares: " + to_string(tradable_shares) + "; Share Price:" + to_string(tradable_share_price);
						Transaction::Transaction_for_Stock(2, total_value_2, comment);
						nus << index << ';' << selected_user << ';' << company_name << ';' << shares << ';' << avg_share_price << ';' << total_value_1 << '\n';
						selected_company_name = company_name;
					}
				}
				nus << index << ';' << selected_user << ';' << company_name << ';' << shares << ';' << share_price << ';' << total_value << '\n';
			}
			else
			{
				if (selected_company_name == company_name)
				{
					double total_value_3 = stol(shares) * avg_share_price;
					nus << index << ';' << selected_user << ';' << selected_company_name << ';' << shares << ';' << avg_share_price << ';' << total_value_3 << '\n';
				}
				else
				{
					nus << index << ';' << selected_user << ';' << company_name << ';' << shares << ';' << share_price << ';' << total_value << '\n';
				}
			}
		}
		else
		{
			if (selected_company_name == company_name)
			{
				double total_value_3 = stol(shares) * avg_share_price;
				nus << index << ';' << user << ';' << selected_company_name << ';' << shares << ';' << avg_share_price << ';' << total_value_3 << '\n';
			}
			else
			{
				nus << index << ';' << user << ';' << company_name << ';' << shares << ';' << share_price << ';' << total_value << '\n';
			}
		}
	}
	ns.close();
	as.close();
	us.close();
	nus.close();

	as.open("Stock/AllStocks.csv", ios::in);
	ns.open("Stock/NewStocks.csv", ios::out);

	while (as.peek() != EOF)
	{
		getline(as, index, ';');
		getline(as, company_name, ';');
		getline(as, shares, ';');
		getline(as, share_price, '\n');
		if (selected_company_name == company_name)
		{
			tradable_shares = *tradable_shares_pointer;
			avg_share_price = *avg_share_price_pointer;
			long shares_1 = stol(shares);
			shares_1 += tradable_shares;
			shares = to_string(shares_1);
			ns << index << ';' << company_name << ';' << shares << ';' << avg_share_price << '\n';
		}
		else
		{
			ns << index << ';' << company_name << ';' << shares << ';' << share_price << '\n';
		}
	}
	as.close();
	ns.close();

	as.open("Stock/AllStocks.csv", ios::out);
	ns.open("Stock/NewStocks.csv", ios::in);

	while (ns.peek() != EOF)
	{
		getline(ns, index, ';');
		getline(ns, company_name, ';');
		getline(ns, shares, ';');
		getline(ns, share_price, '\n');
		as << index << ';' << company_name << ';' << shares << ';' << share_price << '\n';
	}
	ns.close();
	as.close();
	remove("Stock/NewStocks.csv");

	us.open("Stock/UserShares.csv", ios::out);
	nus.open("Stock/NewUserShares.csv", ios::in);

	while (nus.peek() != EOF)
	{
		getline(nus, index, ';');
		getline(nus, user, ';');
		getline(nus, company_name, ';');
		getline(nus, shares, ';');
		getline(nus, share_price, ';');
		getline(nus, total_value, '\n');
		double total_value_1 = stol(shares) * stod(share_price);
		if (shares != "0")
		{
			us << index << ';' << user << ';' << company_name << ';' << shares << ';' << share_price << ';' << total_value_1 << '\n';
		}
		else
		{
			us << "0" << ';' << "0" << ';' << "0" << ';' << "0" << ';' << "0" << ';' << "0" << '\n';
		}
	}
	nus.close();
	us.close();
	remove("Stock/NewUserShares.csv");
}

void Stock::mainMenu(void)
{
	totalValue();
	cout << "Stocks" << endl;
	cout << "1. Show your share list" << endl;
	cout << "2. Buy shares" << endl;
	cout << "3. Sell shares" << endl;
	cout << ">>";
	cin >> choice_1;
}

void Stock::menu()
{
	do
	{
		choice_2 = 0;
		mainMenu();
		switch (choice_1)
		{
		case 1:
			userShares();
			break;
		case 2:
			buy();
			break;
		case 3:
			sell();
			break;
		default:
			break;
		}
	} while (choice_1 != 4);
}

int main()
{
	Stock obj1;
	obj1.menu();
}