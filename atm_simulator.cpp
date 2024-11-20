#include <iostream>
#include <string>

using namespace std;

// Base class: BankAccount
class BankAccount {
protected:
    string accountNumber;  // Protected member to allow access in derived classes
    double balance;        // Protected member for derived classes
    static int accountCount;  // Static member to keep track of total accounts

public:
    // Default constructor
    BankAccount() : accountNumber("00000000"), balance(0.0) {
        cout << "Default constructor called for account: " << accountNumber << endl;
        accountCount++;
    }

    // Parameterized constructor
    BankAccount(string accNum, double bal) : accountNumber(accNum), balance(bal) {
        cout << "Parameterized constructor called for account: " << accountNumber << endl;
        accountCount++;
    }

    // Virtual destructor for proper cleanup in inheritance
    virtual ~BankAccount() {
        cout << "Destructor called for account: " << accountNumber << endl;
        accountCount--;
    }

    // Public getter for balance
    virtual double getBalance() const {
        return this->balance;
    }

    // Public deposit method
    virtual void deposit(double amount) {
        if (amount > 0) {
            balance += amount;
            cout << "Deposited: " << amount << ". New balance: " << balance << endl;
        } else {
            cout << "Deposit amount must be positive!" << endl;
        }
    }

    // Public withdraw method (can be overridden)
    virtual void withdraw(double amount) {
        if (amount > balance) {
            cout << "Insufficient balance!" << endl;
        } else {
            balance -= amount;
            cout << "Withdrew: " << amount << ". Remaining balance: " << balance << endl;
        }
    }

    // Static method to get total accounts
    static int getTotalAccounts() {
        return accountCount;
    }
};

// Initialize static member
int BankAccount::accountCount = 0;

// Derived class: SavingsAccount
class SavingsAccount : public BankAccount {
private:
    double interestRate;  // Interest rate for savings account

public:
    // Constructor for SavingsAccount
    SavingsAccount(string accNum, double bal, double rate) 
        : BankAccount(accNum, bal), interestRate(rate) {
        cout << "SavingsAccount created for account: " << accountNumber << endl;
    }

    // Method to apply interest
    void applyInterest() {
        double interest = balance * (interestRate / 100);
        balance += interest;
        cout << "Interest of " << interestRate << "% applied. New balance: " << balance << endl;
    }

    // Destructor
    ~SavingsAccount() {
        cout << "Destructor called for SavingsAccount: " << accountNumber << endl;
    }
};

// Derived class: CurrentAccount
class CurrentAccount : public BankAccount {
private:
    double overdraftLimit;  // Overdraft limit for current account

public:
    // Constructor for CurrentAccount
    CurrentAccount(string accNum, double bal, double limit) 
        : BankAccount(accNum, bal), overdraftLimit(limit) {
        cout << "CurrentAccount created for account: " << accountNumber << endl;
    }

    // Override withdraw to include overdraft functionality
    void withdraw(double amount) override {
        if (amount > balance + overdraftLimit) {
            cout << "Withdrawal exceeds overdraft limit!" << endl;
        } else {
            balance -= amount;
            cout << "Withdrew: " << amount << ". Remaining balance: " << balance << endl;
        }
    }

    // Destructor
    ~CurrentAccount() {
        cout << "Destructor called for CurrentAccount: " << accountNumber << endl;
    }
};

// ATM class demonstrates interaction with BankAccount using abstraction
class ATM {
private:
    BankAccount* account;  // Pointer to a BankAccount object

public:
    // Constructor
    ATM(BankAccount* acc) : account(acc) {}

    // Deposit money
    void addAmount(double amount) {
        account->deposit(amount);
    }

    // Withdraw money
    void withdrawAmount(double amount) {
        account->withdraw(amount);
    }

    // Apply specific behaviors (if applicable)
    void applySpecificBehavior() {
        // Try dynamic_cast to check account type and apply specific behavior
        SavingsAccount* savings = dynamic_cast<SavingsAccount*>(account);
        if (savings) {
            savings->applyInterest();
        }
    }
};

int main() {
    // Create a SavingsAccount object
    SavingsAccount* savings = new SavingsAccount("12345678", 1000.0, 5.0);

    // Create a CurrentAccount object
    CurrentAccount* current = new CurrentAccount("87654321", 500.0, 200.0);

    // Display the total number of accounts created
    cout << "Total accounts created: " << BankAccount::getTotalAccounts() << endl;

    // Create ATM objects for each account
    ATM* savingsATM = new ATM(savings);
    savingsATM->addAmount(200.0);  // Deposit to savings account
    savingsATM->applySpecificBehavior();  // Apply interest to savings account

    ATM* currentATM = new ATM(current);
    currentATM->withdrawAmount(600.0);  // Withdraw using overdraft

    // Clean up memory
    delete savingsATM;
    delete currentATM;
    delete savings;
    delete current;

    // Display the total number of accounts remaining
    cout << "Total accounts remaining: " << BankAccount::getTotalAccounts() << endl;

    return 0;
}
