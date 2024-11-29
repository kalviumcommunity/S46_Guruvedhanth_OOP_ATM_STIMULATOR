#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <stdexcept>
#include <cmath>  // Added for pow() function

// Abstract base class for transaction fees
class TransactionFeeStrategy {
public:
    virtual ~TransactionFeeStrategy() = default;
    virtual double calculateFee(double amount) const = 0;
};

// Concrete fee strategies
class NoFeeStrategy : public TransactionFeeStrategy {
public:
    double calculateFee(double amount) const override {
        return 0.0;
    }
};

class PercentageFeeStrategy : public TransactionFeeStrategy {
private:
    double feeRate;

public:
    explicit PercentageFeeStrategy(double rate) : feeRate(rate) {}

    double calculateFee(double amount) const override {
        return amount * (feeRate / 100.0);
    }
};

class FlatFeeStrategy : public TransactionFeeStrategy {
private:
    double flatFee;

public:
    explicit FlatFeeStrategy(double fee) : flatFee(fee) {}

    double calculateFee(double amount) const override {
        return flatFee;
    }
};

// Abstract base class for interest calculation
class InterestCalculationStrategy {
public:
    virtual ~InterestCalculationStrategy() = default;
    virtual double calculateInterest(double balance, double rate) const = 0;
};

// Concrete interest calculation strategies
class SimpleInterestStrategy : public InterestCalculationStrategy {
public:
    double calculateInterest(double balance, double rate) const override {
        return balance * (rate / 100.0);
    }
};

class CompoundInterestStrategy : public InterestCalculationStrategy {
private:
    int compoundFrequency;

public:
    explicit CompoundInterestStrategy(int frequency = 12) 
        : compoundFrequency(frequency) {}

    double calculateInterest(double balance, double rate) const override {
        // Use std::pow from <cmath>
        double periodRate = rate / (100.0 * compoundFrequency);
        return balance * (std::pow(1 + periodRate, compoundFrequency) - 1);
    }
};

// Forward declaration
class Account;

// Abstract base class for account
class Account {
protected:
    std::string accountNumber;
    double balance;
    std::unique_ptr<TransactionFeeStrategy> feeStrategy;

public:
    Account(const std::string& accNum, double initialBalance, 
            std::unique_ptr<TransactionFeeStrategy> feeStrat = std::make_unique<NoFeeStrategy>())
        : accountNumber(accNum), balance(initialBalance), 
          feeStrategy(std::move(feeStrat)) {}

    virtual ~Account() = default;

    // Virtual methods for core account operations
    virtual void deposit(double amount) {
        if (amount <= 0) {
            throw std::invalid_argument("Deposit amount must be positive");
        }
        double fee = feeStrategy->calculateFee(amount);
        if (fee > amount) {
            throw std::runtime_error("Fee exceeds deposit amount");
        }
        balance += (amount - fee);
        std::cout << "Deposited: " << amount 
                  << ", Fee: " << fee 
                  << ", New Balance: " << balance << std::endl;
    }

    virtual void withdraw(double amount) {
        if (amount <= 0) {
            throw std::invalid_argument("Withdrawal amount must be positive");
        }
        double fee = feeStrategy->calculateFee(amount);
        if (amount + fee > balance) {
            throw std::runtime_error("Insufficient funds");
        }
        balance -= (amount + fee);
        std::cout << "Withdrew: " << amount 
                  << ", Fee: " << fee 
                  << ", New Balance: " << balance << std::endl;
    }

    // Getters
    virtual std::string getAccountType() const = 0;
    double getBalance() const { return balance; }
    std::string getAccountNumber() const { return accountNumber; }
};

// Concrete Account Types
class SavingsAccount : public Account {
private:
    double interestRate;
    std::unique_ptr<InterestCalculationStrategy> interestStrategy;

public:
    SavingsAccount(const std::string& accNum, double initialBalance, 
                   double rate, 
                   std::unique_ptr<TransactionFeeStrategy> feeStrat = std::make_unique<NoFeeStrategy>(),
                   std::unique_ptr<InterestCalculationStrategy> intStrat = std::make_unique<SimpleInterestStrategy>())
        : Account(accNum, initialBalance, std::move(feeStrat)), 
          interestRate(rate), 
          interestStrategy(std::move(intStrat)) {}

    void applyInterest() {
        double interest = interestStrategy->calculateInterest(balance, interestRate);
        balance += interest;
        std::cout << "Interest Applied: " << interest 
                  << ", New Balance: " << balance << std::endl;
    }

    std::string getAccountType() const override {
        return "Savings";
    }
};

class CurrentAccount : public Account {
private:
    double overdraftLimit;

public:
    CurrentAccount(const std::string& accNum, double initialBalance, 
                   double limit, 
                   std::unique_ptr<TransactionFeeStrategy> feeStrat = std::make_unique<NoFeeStrategy>())
        : Account(accNum, initialBalance, std::move(feeStrat)), 
          overdraftLimit(limit) {}

    void withdraw(double amount) override {
        if (amount <= 0) {
            throw std::invalid_argument("Withdrawal amount must be positive");
        }
        double fee = feeStrategy->calculateFee(amount);
        if (amount + fee > balance + overdraftLimit) {
            throw std::runtime_error("Exceeds overdraft limit");
        }
        balance -= (amount + fee);
        std::cout << "Withdrew: " << amount 
                  << ", Fee: " << fee 
                  << ", New Balance: " << balance << std::endl;
    }

    std::string getAccountType() const override {
        return "Current";
    }
};

// Banking system to manage accounts
class BankingSystem {
private:
    std::vector<std::unique_ptr<Account>> accounts;

public:
    void addAccount(std::unique_ptr<Account> account) {
        accounts.push_back(std::move(account));
    }

    // New method to find account by number
    Account* getAccountByNumber(const std::string& accountNumber) {
        for (auto& account : accounts) {
            if (account->getAccountNumber() == accountNumber) {
                return account.get();
            }
        }
        return nullptr;
    }

    // Extensible reporting method
    void generateReport() {
        std::cout << "--- Banking System Report ---" << std::endl;
        for (const auto& account : accounts) {
            std::cout << "Account Number: " << account->getAccountNumber() 
                      << ", Type: " << account->getAccountType()
                      << ", Balance: " << account->getBalance() << std::endl;
        }
    }
};

int main() {
    BankingSystem bank;

    // Create accounts with different fee and interest strategies
    auto savingsAccount1 = std::make_unique<SavingsAccount>(
        "12345", 1000.0, 5.0, 
        std::make_unique<PercentageFeeStrategy>(0.5),  // 0.5% transaction fee
        std::make_unique<SimpleInterestStrategy>()
    );

    auto savingsAccount2 = std::make_unique<SavingsAccount>(
        "67890", 2000.0, 4.5, 
        std::make_unique<FlatFeeStrategy>(2.0),  // $2 flat fee
        std::make_unique<CompoundInterestStrategy>()
    );

    auto currentAccount = std::make_unique<CurrentAccount>(
        "54321", 500.0, 200.0, 
        std::make_unique<PercentageFeeStrategy>(1.0)  // 1% transaction fee
    );

    // Add accounts to banking system
    bank.addAccount(std::move(savingsAccount1));
    bank.addAccount(std::move(savingsAccount2));
    bank.addAccount(std::move(currentAccount));

    // Demonstrate operations
    try {
        // Perform some transactions
        bank.generateReport();

        // Deposit and apply interest
        SavingsAccount* savings = dynamic_cast<SavingsAccount*>(
            bank.getAccountByNumber("12345")
        );
        if (savings) {
            savings->deposit(100.0);
            savings->applyInterest();
        }

        // Demonstrate extensibility
        bank.generateReport();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
}