#include <iostream>
#include <string>
#include <stdexcept>
#include <memory>
#include <vector>

// Separate logging responsibility
class Logger {
public:
    static void log(const std::string& message) {
        std::cout << message << std::endl;
    }

    static void logError(const std::string& message) {
        std::cerr << "ERROR: " << message << std::endl;
    }
};

// Separate interest calculation responsibility
class InterestCalculator {
public:
    static double calculateSimpleInterest(double principal, double rate, double time) {
        return principal * (rate / 100) * time;
    }
};

// Separate validation responsibility
class AccountValidator {
public:
    static bool isValidAmount(double amount) {
        return amount > 0;
    }

    static bool hasEnoughBalance(double balance, double amount, double overdraftLimit = 0) {
        return amount <= (balance + overdraftLimit);
    }
};

// Base class with minimal responsibility
class Account {
protected:
    std::string accountNumber;
    double balance;

public:
    Account(const std::string& accNum, double initialBalance) 
        : accountNumber(accNum), balance(initialBalance) {
        Logger::log("Account created: " + accountNumber);
    }

    virtual ~Account() {
        Logger::log("Account destroyed: " + accountNumber);
    }

    std::string getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    virtual void deposit(double amount) = 0;
    virtual void withdraw(double amount) = 0;
};

// Savings Account with focused responsibility
class SavingsAccount : public Account {
private:
    double interestRate;

public:
    SavingsAccount(const std::string& accNum, double initialBalance, double rate) 
        : Account(accNum, initialBalance), interestRate(rate) {
        Logger::log("Savings Account created: " + accountNumber);
    }

    void deposit(double amount) override {
        if (!AccountValidator::isValidAmount(amount)) {
            Logger::logError("Invalid deposit amount");
            return;
        }
        balance += amount;
        Logger::log("Deposited: " + std::to_string(amount) + 
                    ". New balance: " + std::to_string(balance));
    }

    void withdraw(double amount) override {
        if (!AccountValidator::isValidAmount(amount)) {
            Logger::logError("Invalid withdrawal amount");
            return;
        }
        if (!AccountValidator::hasEnoughBalance(balance, amount)) {
            Logger::logError("Insufficient balance");
            return;
        }
        balance -= amount;
        Logger::log("Withdrew: " + std::to_string(amount) + 
                    ". New balance: " + std::to_string(balance));
    }

    void applyInterest() {
        double interest = InterestCalculator::calculateSimpleInterest(balance, interestRate, 1);
        balance += interest;
        Logger::log("Interest applied. New balance: " + std::to_string(balance));
    }
};

// Current Account with focused responsibility
class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& accNum, double initialBalance, double limit) 
        : Account(accNum, initialBalance), overdraftLimit(limit) {
        Logger::log("Current Account created: " + accountNumber);
    }

    void deposit(double amount) override {
        if (!AccountValidator::isValidAmount(amount)) {
            Logger::logError("Invalid deposit amount");
            return;
        }
        balance += amount;
        Logger::log("Deposited: " + std::to_string(amount) + 
                    ". New balance: " + std::to_string(balance));
    }

    void withdraw(double amount) override {
        if (!AccountValidator::isValidAmount(amount)) {
            Logger::logError("Invalid withdrawal amount");
            return;
        }
        if (!AccountValidator::hasEnoughBalance(balance, amount, overdraftLimit)) {
            Logger::logError("Withdrawal exceeds overdraft limit");
            return;
        }
        balance -= amount;
        Logger::log("Withdrew: " + std::to_string(amount) + 
                    ". New balance: " + std::to_string(balance));
    }
};

// ATM with focused responsibility of account interaction
class ATM {
private:
    Account* account;

public:
    ATM(Account* acc) : account(acc) {}

    void deposit(double amount) {
        account->deposit(amount);
    }

    void withdraw(double amount) {
        account->withdraw(amount);
    }

    // Specific behavior handling
    void applySpecificBehavior() {
        SavingsAccount* savings = dynamic_cast<SavingsAccount*>(account);
        if (savings) {
            savings->applyInterest();
        }
    }
};

// Banking system manager with responsibility of account management
class BankingSystem {
private:
    std::vector<std::unique_ptr<Account>> accounts;

public:
    void addAccount(std::unique_ptr<Account> account) {
        accounts.push_back(std::move(account));
    }

    Account* findAccount(const std::string& accountNumber) {
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                return account.get();
            }
        }
        return nullptr;
    }
};

int main() {
    // Create a banking system
    BankingSystem bank;

    // Create accounts
    auto savings = std::make_unique<SavingsAccount>("12345678", 1000.0, 5.0);
    auto current = std::make_unique<CurrentAccount>("87654321", 500.0, 200.0);

    // Add accounts to banking system
    bank.addAccount(std::move(savings));
    bank.addAccount(std::move(current));

    // Find the accounts
    SavingsAccount* savingsAccount = dynamic_cast<SavingsAccount*>(bank.findAccount("12345678"));
    CurrentAccount* currentAccount = dynamic_cast<CurrentAccount*>(bank.findAccount("87654321"));

    // Create ATMs for accounts
    ATM savingsATM(savingsAccount);
    ATM currentATM(currentAccount);

    // Perform operations
    savingsATM.deposit(200.0);
    savingsATM.applySpecificBehavior();

    currentATM.withdraw(600.0);

    return 0;
}