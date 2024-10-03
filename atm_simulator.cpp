#include <iostream>
#include <string>

using namespace std;

// BankAccount class demonstrating abstraction, encapsulation, constructors, and destructor
class BankAccount {
private:
    string accountNumber;  // Private member to store account number
    double balance;        // Private member to store account balance
    static int accountCount;  // Static private member to keep track of total accounts

public:
    // Default constructor
    BankAccount() : accountNumber("00000000"), balance(0.0) {
        cout << "Default constructor called for account: " << accountNumber << endl;
        accountCount++;  // Increment the count when a new account is created
    }

    // Parameterized constructor to initialize account with an account number and balance
    BankAccount(string accNum, double bal) : accountNumber(accNum), balance(bal) {
        cout << "Parameterized constructor called for account: " << accountNumber << endl;
        accountCount++;  // Increment the count when a new account is created
    }

    // Public destructor
    ~BankAccount() {
        cout << "Destructor called for account: " << accountNumber << endl;
        accountCount--;  // Decrement the count when an account is deleted
    }

    // Public accessor (getter) to retrieve the account balance
    double getBalance() const {
        return this->balance;
    }

    // Public mutator (setter) to deposit amount into the account
    void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: " << amount << ". New balance: " << balance << endl;
        } else {
            cout << "Deposit amount must be positive!" << endl;
        }
    }

    // Public mutator (setter) to withdraw amount from the account
    void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance!" << endl;
        } else {
            balance -= amount;
            cout << "Withdrew: " << amount << ". Remaining balance: " << balance << endl;
        }
    }

    // Static public method to retrieve the total number of accounts
    static int getTotalAccounts() {
        return accountCount;
    }
};

// Initialize the static member outside the class definition
int BankAccount::accountCount = 0;

// ATM class demonstrates interaction with BankAccount using abstraction
class ATM {
private:
    BankAccount* account;  // Private member to hold the bank account being used

public:
    // Public constructor to associate an ATM with a bank account
    ATM(BankAccount* acc) : account(acc) {}

    // Public method to deposit money into the associated account
    void addAmount(double amount) {
        account->deposit(amount);  // Accessing BankAccount through abstraction
    }

    // Public method to withdraw money from the associated account
    void withdrawAmount(double amount) {
        account->withdraw(amount);  // Accessing BankAccount through abstraction
    }
};

int main() {
    // Demonstrating the use of the default constructor
    BankAccount* account1 = new BankAccount();  // Default constructor will be called

    // Demonstrating the use of the parameterized constructor
    BankAccount* account2 = new BankAccount("87654321", 1000.0);  // Parameterized constructor

    // Display the total number of bank accounts created
    cout << "Total accounts created: " << BankAccount::getTotalAccounts() << endl;

    // Create ATM objects for each bank account
    ATM* myATM = new ATM(account1);
    myATM->addAmount(200.0);      // Deposit using the ATM
    myATM->withdrawAmount(100.0); // Withdraw using the ATM

    ATM* anotherATM = new ATM(account2);
    anotherATM->addAmount(300.0);     // Deposit using the ATM
    anotherATM->withdrawAmount(150.0); // Withdraw using the ATM

    // Clean up dynamically allocated memory
    delete myATM;
    delete anotherATM;
    delete account1;  // Destructor will be called here for account1
    delete account2;  // Destructor will be called here for account2

    // Display the total number of bank accounts after deletion
    cout << "Total accounts remaining: " << BankAccount::getTotalAccounts() << endl;

    return 0;
}
