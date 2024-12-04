#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>

// Abstract base class for bank accounts
class BankAccount {
protected:
    std::string accountNumber;
    double balance;

public:
    BankAccount(std::string accNum, double initialBalance) 
        : accountNumber(accNum), balance(initialBalance) {}

    virtual ~BankAccount() = default;

    // Pure virtual methods to enforce contract
    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
    virtual double getBalance() const = 0;
    virtual std::string getAccountType() const = 0;
};

// Regular Savings Account
class SavingsAccount : public BankAccount {
private:
    double minimumBalance;

public:
    SavingsAccount(std::string accNum, double initialBalance, double minBalance = 100.0)
        : BankAccount(accNum, initialBalance), minimumBalance(minBalance) {}

    void deposit(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("Deposit amount must be positive");
        }
        balance += amount;
    }

    void withdraw(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("Withdrawal amount must be positive");
        }
        if (balance - amount < minimumBalance) {
            throw std::runtime_error("Cannot withdraw below minimum balance");
        }
        balance -= amount;
    }

    double getBalance() const override {
        return balance;
    }

    std::string getAccountType() const override {
        return "Savings Account";
    }
};

// Checking Account with overdraft protection
class CheckingAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CheckingAccount(std::string accNum, double initialBalance, double limit = 500.0)
        : BankAccount(accNum, initialBalance), overdraftLimit(limit) {}

    void deposit(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("Deposit amount must be positive");
        }
        balance += amount;
    }

    void withdraw(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("Withdrawal amount must be positive");
        }
        if (balance + overdraftLimit < amount) {
            throw std::runtime_error("Insufficient funds including overdraft");
        }
        balance -= amount;
    }

    double getBalance() const override {
        return balance;
    }

    std::string getAccountType() const override {
        return "Checking Account";
    }
};

// ATM class that works with any BankAccount
class ATM {
private:
    BankAccount* account;

public:
    ATM(BankAccount* acc) : account(acc) {}

    void performDeposit(double amount) {
        try {
            account->deposit(amount);
            std::cout << "Deposited: " << amount 
                      << ". New balance: " << account->getBalance() 
                      << " (" << account->getAccountType() << ")" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Deposit failed: " << e.what() << std::endl;
        }
    }

    void performWithdrawal(double amount) {
        try {
            account->withdraw(amount);
            std::cout << "Withdrew: " << amount 
                      << ". New balance: " << account->getBalance() 
                      << " (" << account->getAccountType() << ")" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Withdrawal failed: " << e.what() << std::endl;
        }
    }
};

int main() {
    try {
        // Create different types of accounts
        std::unique_ptr<BankAccount> savingsAccount = 
            std::make_unique<SavingsAccount>("SA001", 1000.0, 100.0);
        std::unique_ptr<BankAccount> checkingAccount = 
            std::make_unique<CheckingAccount>("CA001", 500.0, 200.0);

        // ATM operations with both account types
        ATM savingsATM(savingsAccount.get());
        ATM checkingATM(checkingAccount.get());

        // Demonstrate LSP: Both accounts can be used interchangeably through the base class interface
        savingsATM.performDeposit(200.0);
        savingsATM.performWithdrawal(50.0);

        checkingATM.performDeposit(300.0);
        checkingATM.performWithdrawal(700.0);  // Uses overdraft

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}