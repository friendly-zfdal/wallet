#define _CRT_SECURE_NO_WARNINGS
#include "Source_Egor.h"



using std::fstream;
using std::ifstream;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;
using std::cin;
using std::to_string;
using std::ios;

int choice_1 = 0;
int choice_2 = 3;
string selected_user = "James";

void Stock::allStock()
{
	Wallet::current_user->get_login();
	vector<Card*> test=*Wallet::current_user->get_accounts();
	vector<Card*>::iterator it = test.begin();
	while(it != test.end()) {
		if ((*it)->dep_type == 2)
			break;
	}
	double amount = (*it)->get_value();

	fstream as("AllStocks.csv", std::ios::in);

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
	fstream us("UserShares.csv", std::ios::in);

	string index;
	string user;
	string company_name;
	string shares;
	string share_price;
	string total_value;
	int i = 1;

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
			cout << index << ". Company: " << company_name << "; Shares: " << shares << "; Share Value: " << share_price << endl;
		}
	}
	us.close();
}

void Stock::totalValue()
{
	fstream us("UserShares.csv", std::ios::in);

	string user;
	string company_name;
	string shares;
	string share_price;
	string total_value;
	double total_value_1;

	while (us.peek() != EOF)
	{
		getline(us, user, ';');
		getline(us, company_name, ';');
		getline(us, shares, ';');
		getline(us, share_price, ';');
		getline(us, total_value, '\n');

		if (selected_user == user)
		{
			total_value_1 = stol(shares) * stod(share_price);
			cout << "Total Value: " << total_value_1 << endl;
		}
	}
	us.close();
}

void Stock::buy()
{
	string selected_index;
	long purchased_shares;
	double purchased_share_price;
	string user;
	string selected_company_name;
	double total_value;
	int i = 1;

	string type = "Buy";

	allStock();

	cout << "Enter the number of requirement company: ";
	cin >> selected_index;

	fstream as("AllStocks.csv", ios::in);
	fstream ns("NewStocks.csv", ios::out);
	fstream tr("Transaction.csv", ios::app);
	fstream us("UserShares.csv", ios::app);
	fstream nus("NewUserShares.csv", ios::out);

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
				long shares_1 = stol(shares);
				shares_1 -= purchased_shares;
				shares = to_string(shares_1);
			}
			cout << "Write the share price: ";
			cin >> purchased_share_price;
			if (purchased_share_price < stol(share_price))
			{
				cout << "Sorry, share price cannot be less than the current" << endl;
			}
			else
			{
				//Изменение цены
			}
			total_value = purchased_shares * purchased_share_price;
			ns << index << ';' << company_name << ';' << shares << ';' << share_price << '\n';
			tr << selected_user << ';' << company_name << ';' << purchased_shares << ';' << purchased_share_price << ';' << type << ';' << total_value << '\n';
			us << i << ';' << selected_user << ';' << company_name << ';' << purchased_shares << ';' << purchased_share_price << ';' << total_value << '\n';
			selected_company_name = company_name;

		}
		else
		{
			ns << index << ';' << company_name << ';' << shares << ';' << share_price << '\n';
		}
	}
	ns.close();
	as.close();
	tr.close();
	us.close();
	nus.close();

	as.open("AllStocks.csv", ios::out);
	ns.open("NewStocks.csv", ios::in);

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
	remove("NewStocks.csv");

	us.open("UserShares.csv", ios::in);
	nus.open("NewUserShares.csv", ios::out);

	long user_shares = 0;

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
			if (selected_company_name == company_name)
			{
				long shares_2 = stol(shares);
				user_shares += shares_2; //Количество акций пользователя
				total_value_1 = user_shares * stod(share_price);//ОБщая сумма покупки
				nus << i << ';' << user << ';' << company_name << ';' << user_shares << ';' << share_price << ';' << total_value_1 << '\n';
				i++;
			}
		}
	}
	us.close();
	nus.close();

	us.open("UserShares.csv", ios::out);
	nus.open("NewUserShares.csv", ios::in);

	while (nus.peek() != EOF)
	{
		getline(nus, index, ';');
		getline(nus, user, ';');
		getline(nus, company_name, ';');
		getline(nus, shares, ';');
		getline(nus, share_price, ';');
		getline(nus, total_value_1, '\n');
		total_value = stol(shares) * stod(share_price);
		us << index << ';' << user << ';' << company_name << ';' << shares << ';' << share_price << ';' << total_value << '\n';
	}
	nus.close();
	us.close();
	remove("NewUserShares.csv");
}

void Stock::sell()
{
	string selected_index;
	long tradable_shares;
	double tradable_share_price;
	string selected_company_name;
	double avg_share_price;


	string type = "Sell";

	userShares();

	cout << "Enter the number of requirement company: ";
	cin >> selected_index;

	fstream as("AllStocks.csv", ios::in);
	fstream ns("NewStocks.csv", ios::out);
	fstream tr("Transaction.csv", ios::app);
	fstream us("UserShares.csv", ios::in);
	fstream nus("NewUserShares.csv", ios::out);

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
				if (tradable_share_price < 0)
				{
					cout << "Please, enter a valid number" << endl;
				}
				double profit = (tradable_share_price - stod(share_price)) * tradable_shares; //Выгода от проодажи
				avg_share_price = (stod(share_price) + tradable_share_price) / 2; //Усредненная цена за акцию
				double total_value_1 = stol(shares) * avg_share_price + profit; //Общая сумма всех акций пользователя + выгода с продажи
				double total_value_2 = tradable_shares * tradable_share_price; //ОБщая сумма продажи
				tr << selected_user << ';' << company_name << ';' << tradable_shares << ';' << tradable_share_price << ';' << type << ';' << total_value_2 << '\n';
				nus << index << ';' << selected_user << ';' << company_name << ';' << shares << ';' << avg_share_price << ';' << total_value_1 << '\n';
				selected_company_name = company_name;
			}
			else
			{
				nus << index << ';' << selected_user << ';' << company_name << ';' << shares << ';' << share_price << ';' << total_value << '\n';
			}
		}
		else
		{
			nus << index << ';' << selected_user << ';' << company_name << ';' << shares << ';' << share_price << ';' << total_value << '\n';
		}
	}
	ns.close();
	as.close();
	tr.close();
	us.close();
	nus.close();

	as.open("AllStocks.csv", ios::in);
	ns.open("NewStocks.csv", ios::out);

	while (as.peek() != EOF)
	{
		getline(as, index, ';');
		getline(as, company_name, ';');
		getline(as, shares, ';');
		getline(as, share_price, '\n');
		if (selected_company_name == company_name)
		{
			shares += tradable_shares;
			ns << index << ';' << company_name << ';' << shares << ';' << share_price << '\n';
		}
		else
		{
			ns << index << ';' << company_name << ';' << shares << ';' << share_price << '\n';
		}
	}
	as.close();
	ns.close();

	as.open("AllStocks.csv", ios::out);
	ns.open("NewStocks.csv", ios::in);

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
}

void Stock::mainMenu(void)
{
	cout << "Stocks" << endl;
	cout << "1. Show your share list" << endl;
	cout << "2. Buy shares" << endl;
	cout << "3. Sell Shares" << endl;
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

int Stock::main()
{
	menu();
	return 0;
}