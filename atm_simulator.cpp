#include <iostream>
#include <string>

using namespace std;

class BankAccount {
private:
    string accountNumber;
    double balance;

public:
    BankAccount(string accNum, double bal) : accountNumber(accNum), balance(bal) {}

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount << ". New balance: " << balance << endl;
    }

    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance!" << endl;
        } else {
            balance -= amount;
            cout << "Withdrew: " << amount << ". Remaining balance: " << balance << endl;
        }
    }

    double getBalance() const {
        return balance;
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
