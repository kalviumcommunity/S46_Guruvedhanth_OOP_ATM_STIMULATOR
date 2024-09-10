#include <iostream>
#include <string>

using namespace std;

class BankAccount {
private:
    string accountNumber;
    double balance;

public:
    BankAccount(string accNum, double bal) : accountNumber(accNum), balance(bal) {}

    BankAccount& deposit(double amount) {
        this->balance += amount;
        cout << "Deposited: " << amount << ". New balance: " << this->balance << endl;
        return *this;
    }

    BankAccount& withdraw(double amount) {
        if (amount > this->balance) {
            cout << "Insufficient balance!" << endl;
        } else {
            this->balance -= amount;
            cout << "Withdrew: " << amount << ". Remaining balance: " << this->balance << endl;
        }
        return *this;
    }

    double getBalance() const {
        return this->balance;
    }
};

class ATM {
private:
    BankAccount* account;

public:
    ATM(BankAccount* acc) : account(acc) {}

    void addAmount(double amount) {
        account->deposit(amount);
    }

    void withdrawAmount(double amount) {
        account->withdraw(amount);
    }
};

int main() {
    BankAccount myAccount("12345678", 500.0);

    ATM myATM(&myAccount);

    myATM.addAmount(200.0);
    myATM.withdrawAmount(100.0);

    return 0;
}
