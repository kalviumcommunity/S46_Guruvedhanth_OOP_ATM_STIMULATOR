#include <iostream>
#include <string>

using namespace std;

class BankAccount {
private:
    string accountNumber;
    double balance;
    static int accountCount;  // Static data member to keep track of account count

public:
    BankAccount(string accNum, double bal) : accountNumber(accNum), balance(bal) {
        accountCount++;  // Increment the count whenever a new account is created
    }

    ~BankAccount() {
        accountCount--;  // Decrement the count when an account is deleted
    }

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

    // Static member function to get the total number of accounts
    static int getTotalAccounts() {
        return accountCount;
    }
};

// Initialize the static member
int BankAccount::accountCount = 0;

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
    // Dynamically allocate BankAccount objects
    BankAccount* account1 = new BankAccount("12345678", 500.0);
    BankAccount* account2 = new BankAccount("87654321", 1000.0);
    BankAccount* account3 = new BankAccount("11223344", 750.0);

    // Display total accounts using the static member function
    cout << "Total accounts created: " << BankAccount::getTotalAccounts() << endl;

    // Dynamically allocate ATM objects
    ATM* myATM = new ATM(account1);
    myATM->addAmount(200.0);
    myATM->withdrawAmount(100.0);

    ATM* anotherATM = new ATM(account2);
    anotherATM->addAmount(300.0);
    anotherATM->withdrawAmount(150.0);

    // Clean up dynamically allocated memory
    delete myATM;
    delete anotherATM;
    delete account1;
    delete account2;
    delete account3;

    // Display total accounts after deletion
    cout << "Total accounts remaining: " << BankAccount::getTotalAccounts() << endl;

    return 0;
}
