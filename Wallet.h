class Wallet {
 public:
   double accountCash;
   double accountCard;
  // double accountBank;
   double accountStock;
public:

Wallet(){accountCash=1000;}
~Wallet(){save();}

void add( double sum );
void add( char s[] ); 
void dec( double sum );
bool save();

//void display(Wallet a){std::cout<<a.accountCash<<std::endl;}
};