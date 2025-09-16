#include <iostream>
#include <string>
#include <ctime>
#include <vector>
using namespace std;

class Transaction {
    int amount;
    string type;
    string date;
    string getCurrentTime() const{
        time_t now = time(0);
        char buf[80];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return string(buf);
    }
    public:
    Transaction(int amt, string t) : amount(amt), type(t), date(getCurrentTime()) {}
    int getAmount() const{
        return amount;
    }
    string getType() const{
        return type;
    }
    string getDate() const{
        return date;
    }
    friend ostream& operator<<(ostream& os, const Transaction& t) {
        os << "[" << t.date << "] " << t.type << " : " << t.amount;
        return os;
    }
};

class Account {
    protected:
    string accountNumber;
    int balance;
    string ownerName;
    vector<Transaction> history;
    public:
    Account(string accNum, string owner) : accountNumber(accNum), ownerName(owner), balance(0) {}
    Account& operator+=(int amount){
        if(amount < 0){
            cout << "Must be positive" << endl;
        } else {
            balance += amount;
            history.push_back(Transaction(amount, "Deposit"));
            cout << "Deposited: " << amount << ", Current balance: " << balance << endl;
        }
        return *this;
    }
    Account& operator-=(int amount){
        if(amount > balance){
            cout << "Insufficient funds." << endl;
        } else {
            balance -= amount;
            history.push_back(Transaction(amount, "Withdrawal")); 
            cout << "Withdrawn: " << amount << ", Current balance: " << balance << endl;
        }
        return *this;
    }
    virtual void Withdrawal(int amount) {
        if(amount > balance){
            cout << "Insufficient funds." << endl;
        } else {
            balance -= amount;
            history.push_back(Transaction(amount, "Withdrawal"));
            cout << "Withdrawn: " << amount << ", Balance: " << balance << endl;
        }
    }
    string getAccountNumber() const {
        return accountNumber;
    }
    int getBalance() const {
        return balance;
    }
    string getOwnerName() const {
        return ownerName;
    }
    void printHistory() const {
        cout << accountNumber << " transaction history:" << endl;
        for (const auto& transaction : history) {
            cout << transaction.getType() << ": " << transaction.getAmount() << endl;
        }
    }
};

class Customer {
    string name;
    string id;
    vector<Account*> accounts; 
    public:
    Customer(string cname, string cid) : name(cname), id(cid) {} 
    
    void addAccount(Account* acc){ 
        accounts.push_back(acc);
    }
    
    void printAccounts() const{
        cout << "Customer: " << name << " ( " << id << " )\n";
        for(const auto &acc : accounts){
            cout << " - Account " << acc->getAccountNumber() << " owned by " 
                 << acc->getOwnerName() << " | Balance: " << acc->getBalance() << endl;
        }
        cout << "Total Balance: " << getTotalBalance() << endl; 
    }
    
    int getTotalBalance() const {
        int total = 0;
        for (const auto& acc : accounts) {
            total += acc->getBalance();
        }
        return total;
    }
    
    void displayInfo(){
        cout << "Customer: " << name << ", ID: " << id << endl;
        if(accounts.empty()){
            cout << "No accounts." << endl;
        } else {
            cout << "Accounts owned: " << endl;
            for(const auto& acc : accounts){
                cout << "Account: " << acc->getAccountNumber() << ", Owner: " 
                     << acc->getOwnerName() << ", Balance: " << acc->getBalance() << endl;
            }
            cout << "Total Balance: " << getTotalBalance() << endl;
        }
    }
};

class SpecializedAccounts: public Account{
    double interestRate;
    int withdrawalCount;
    int withdrawalLimit;
    
    public:
    SpecializedAccounts(string accNum, string owner, double rate, int limit = 10): 
        Account(accNum, owner), interestRate(rate), withdrawalCount(0), withdrawalLimit(limit) {}
    void countInterest(){
        int interest = static_cast<int>(balance * interestRate);
        balance += interest;
        cout << interest << " of interest applied. Balance: " << balance << endl;
    }
    virtual void Withdrawal(int amount) override {
        if(withdrawalCount >= withdrawalLimit){
            cout << "Withdrawal limit reached." << endl;
        } else if (amount > balance) {
            cout << "Insufficient funds." << endl; 
        } else {
            balance -= amount;
            withdrawalCount++;
            cout << "Withdrawn: " << amount << ", Balance: " << balance << endl;
        }
    }
};

int main(){
    //Customers
    Customer cus1("Hugo Vlad", "C1001");
    Customer cus2("Vivian Banshee", "C1002");
    //Normal accounts
    Account hugo1("HUGO1","Hugo");
    Account hugo2("HUGO2","Hugo");
    Account vivian1("VIVIAN1","Vivian");
    //Saving accounts
    SpecializedAccounts hugoSavings("HUGOSP1","Hugo",0.2, 5);
    //Assign accounts
    cus1.addAccount(&hugo1);
    cus1.addAccount(&hugo2);
    cus1.addAccount(&hugoSavings);
    cus2.addAccount(&vivian1);
    //Displays info
    cus1.displayInfo();
    cus2.displayInfo();
    //Deposits and withdrawals
    hugo1 += 300;
    hugo2 += 200;
    hugo1 -= 100;
    hugoSavings +=400;
    hugoSavings.printHistory(); //prints history
    hugoSavings.countInterest();
    cout<<"Final Balance:" <<hugo1.getBalance()<<endl;
    cout<<"Final Balance:" <<hugo2.getBalance()<<endl;
    cout<<"Final Balance:" <<hugoSavings.getBalance()<<endl;
    vivian1 += 400;
    vivian1 -= 50;
    vivian1.printHistory();
    cout<<"Final Balance:" <<vivian1.getBalance()<<endl;
    cus1.displayInfo();
    cus2.displayInfo();
    return 0;
};