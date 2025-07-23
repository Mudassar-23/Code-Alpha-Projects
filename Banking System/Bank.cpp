///////////////////////// Banking System  ///////////////////////
#include <iostream>
#include <string>

using namespace std;
// Forward declaration
class Account;

class Customer {
private:
    string name;
    string address;
    string phone;
    int pin;

public:
    Customer()
        : name("AlEXa"), address("14 House"), phone("1023456"), pin(1234) {}

    const string& getName() const { return name; }
    const string& getAddress() const { return address; }
    const string& getPhone() const { return phone; }
    int getPin() const { return pin; }

    friend istream& operator>>(istream& in, Customer& customer) {
        cout << "Enter customer name: ";
        in.ignore();
        getline(in, customer.name);
        cout << "Enter customer address: ";
        getline(in, customer.address);
        cout << "Enter customer phone: ";
        getline(in, customer.phone);
        cout << "Enter customer pin: ";
        in >> customer.pin;
        return in;
    }

    friend ostream& operator<<(ostream& out, const Customer& customer) {
        out << "Customer Name: " << customer.name << endl;
        out << "Customer Address: " << customer.address << endl;
        out << "Customer Phone: " << customer.phone << endl;
        return out;
    }
};

class Transaction {
private:
    string type;
    double amount;

public:
    Transaction(const string& type = "", double amount = 0.0)
        : type(type), amount(amount) {}

    void display() const {
        cout << "Transaction Type: " << type << endl;
        cout << "Amount: " << amount << endl;
    }
};

class Account {
private:
    int accountNumber;
    Customer customer;
    double balance;
    Transaction* transactions[10]; // Assuming maximum 10 transactions per account
    int transactionCount;

public:
    Account(int accountNumber = 0, const Customer& customer = Customer())
        : accountNumber(accountNumber), customer(customer), balance(0.0), transactionCount(0) {}

    int getAccountNumber() const { return accountNumber; }
    const Customer& getCustomer() const { return customer; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        balance += amount;
        Transaction* transaction = new Transaction("Deposit", amount);
        transactions[transactionCount++] = transaction;
    }

    void withdraw(double amount) {
        if (balance >= amount) {
            balance -= amount;
            Transaction* transaction = new Transaction("Withdrawal", amount);
            transactions[transactionCount++] = transaction;
        }
        else {
            cout << "Insufficient balance." << endl;
        }
    }

    void transfer(Account& toAccount, double amount) {
        if (balance >= amount) {
            balance -= amount;
            Transaction* transactionOut = new Transaction("Transfer to account " + to_string(toAccount.getAccountNumber()), amount);
            transactions[transactionCount++] = transactionOut;

            toAccount.deposit(amount);
            Transaction* transactionIn = new Transaction("Transfer from account " + to_string(accountNumber), amount);
            toAccount.recordTransaction(transactionIn);
        }
        else {
            cout << "Insufficient balance." << endl;
        }
    }

    void recordTransaction(Transaction* transaction) {
        if (transactionCount < 10) {
            transactions[transactionCount++] = transaction;
        }
        else {
            cout << "Transaction limit exceeded." << endl;
        }
    }

    void displayRecentTransactions(int count = 5) const {
        cout << "Recent Transactions for Account " << accountNumber << ":" << endl;
        int start = max(0, transactionCount - count);
        for (int i = start; i < transactionCount; ++i) {
            transactions[i]->display();
        }
    }

    ~Account() {
        for (int i = 0; i < transactionCount; ++i) {
            delete transactions[i];
        }
    }
};

class Bank {
private:
    Customer customers[10]; // Assuming maximum 10 customers
    Account* accounts[10]; // Assuming maximum 10 accounts
    int nextAccountNumber;
    int customerCount;
    int accountCount;
    int managerPin;

public:
    Bank() : nextAccountNumber(1), customerCount(0), accountCount(0), managerPin(1234) {}

    void addCustomer() {
        if (customerCount < 10) {
            Customer newCustomer;
            cin >> newCustomer;
            customers[customerCount++] = newCustomer;
            cout << "Customer added successfully." << endl;
        }
        else {
            cout << "Customer limit exceeded." << endl;
        }
    }

    void openAccount() {
        if (accountCount < 10) {
            int customerIndex;
            cout << "Enter customer index (0 to " << customerCount - 1 << "): ";
            cin >> customerIndex;
            if (customerIndex >= 0 && customerIndex < customerCount) {
                accounts[accountCount++] = new Account(nextAccountNumber, customers[customerIndex]);
                cout << "Account opened successfully. Account number: " << nextAccountNumber << endl;
                nextAccountNumber++;
            }
            else {
                cout << "Invalid customer index." << endl;
            }
        }
        else {
            cout << "Account limit exceeded." << endl;
        }
    }

    void deleteAccount() {
        int accountNumber;
        cout << "Enter account number to delete: ";
        cin >> accountNumber;
        for (int i = 0; i < accountCount; ++i) {
            if (accounts[i]->getAccountNumber() == accountNumber) {
                delete accounts[i];
                for (int j = i; j < accountCount - 1; ++j) {
                    accounts[j] = accounts[j + 1];
                }
                accountCount--;
                cout << "Account deleted successfully." << endl;
                return;
            }
        }
        cout << "Account not found." << endl;
    }

    void performTransaction(int accountNumber, char type, double amount, int targetAccountNumber = 0) {
        bool accountFound = false;
        for (int i = 0; i < accountCount; ++i) {
            if (accounts[i]->getAccountNumber() == accountNumber) {
                accountFound = true;
                switch (type) {
                case 'd':
                    accounts[i]->deposit(amount);
                    cout << "Deposit of " << amount << " successful." << endl;
                    break;
                case 'w':
                    accounts[i]->withdraw(amount);
                    cout << "Withdrawal of " << amount << " successful." << endl;
                    break;
                case 't':
                    if (targetAccountNumber != 0) {
                        for (int j = 0; j < accountCount; ++j) {
                            if (accounts[j]->getAccountNumber() == targetAccountNumber) {
                                accounts[i]->transfer(*accounts[j], amount);
                                cout << "Transfer of " << amount << " to account " << targetAccountNumber << " successful." << endl;
                                break;
                            }
                        }
                    }
                    else {
                        cout << "Invalid target account number." << endl;
                    }
                    break;
                default:
                    cout << "Invalid transaction type." << endl;
                }
                return;
            }
        }
        if (!accountFound) {
            cout << "Account not found." << endl;
        }
    }


    void displayCustomerAccounts(int customerIndex) const {
        if (customerIndex >= 0 && customerIndex < customerCount) {
            const Customer& customer = customers[customerIndex];
            cout << "Accounts for Customer " << customer.getName() << ":" << endl;
            for (int i = 0; i < accountCount; ++i) {
                if (accounts[i]->getCustomer().getName() == customer.getName()) {
                    cout << "Account Number: " << accounts[i]->getAccountNumber() << ", Balance: " << accounts[i]->getBalance() << endl;
                }
            }
        }
        else {
            cout << "Invalid customer index." << endl;
        }
    }

    void managerMenu() {
        int pin;
        cout << "Enter manager pin: ";
        cin >> pin;
        if (pin == managerPin) {
            int choice;
            do {
                cout << "\nManager Menu:" << endl;
                cout << "1. Add customer" << endl;
                cout << "2. Open account" << endl;
                cout << "3. Delete account" << endl;
                cout << "4. View customer accounts" << endl;
                cout << "5. Exit" << endl;
                cout << "Enter your choice: ";
                cin >> choice;
                switch (choice) {
                case 1:
                    addCustomer();
                    break;
                case 2:
                    openAccount();
                    break;
                case 3:
                    deleteAccount();
                    break;
                case 4:
                    int customerIndex;
                    cout << "Enter customer index: ";
                    cin >> customerIndex;
                    displayCustomerAccounts(customerIndex);
                    break;
                case 5:
                    cout << "Exiting manager menu." << endl;
                    break;
                default:
                    cout << "Invalid choice." << endl;
                }
            } while (choice != 5);
        }
        else {
            cout << "Invalid pin." << endl;
        }
    }

    void customerMenu() {
        int pin;
        cout << "Enter customer pin: ";
        cin >> pin;
        int customerIndex = -1;
        for (int i = 0; i < customerCount; ++i) {
            if (customers[i].getPin() == pin) {
                customerIndex = i;
                break;
            }
        }
        if (customerIndex == -1) {
            cout << "Invalid pin." << endl;
            return;
        }
        int choice;
        do {
            cout << "\nCustomer Menu:" << endl;
            cout << "1. Add money" << endl;
            cout << "2. Withdraw money" << endl;
            cout << "3. Transfer money" << endl;
            cout << "4. View recent transactions" << endl;
            cout << "5. View account balance" << endl;
            cout << "6. Exit" << endl;
            cout << "Enter your choice: ";
            cin >> choice;
            int accountNumber;
            double amount;
            switch (choice) {
            case 1:
                cout << "Enter account number: ";
                cin >> accountNumber;
                cout << "Enter amount: ";
                cin >> amount;
                performTransaction(accountNumber, 'd', amount);
                break;
            case 2:
                cout << "Enter account number: ";
                cin >> accountNumber;
                cout << "Enter amount: ";
                cin >> amount;
                performTransaction(accountNumber, 'w', amount);
                break;
            case 3:
                int targetAccountNumber;
                cout << "Enter your account number: ";
                cin >> accountNumber;
                cout << "Enter target account number: ";
                cin >> targetAccountNumber;
                cout << "Enter amount: ";
                cin >> amount;
                performTransaction(accountNumber, 't', amount, targetAccountNumber);
                break;
            case 4:
                cout << "Enter account number: ";
                cin >> accountNumber;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accountNumber) {
                        accounts[i]->displayRecentTransactions();
                        break;
                    }
                }
                break;
            case 5:
                cout << "Enter account number: ";
                cin >> accountNumber;
                for (int i = 0; i < accountCount; ++i) {
                    if (accounts[i]->getAccountNumber() == accountNumber) {
                        cout << "Account balance: " << accounts[i]->getBalance() << endl;
                        break;
                    }
                }
                break;
            case 6:
                cout << "Exiting customer menu." << endl;
                break;
            default:
                cout << "Invalid choice." << endl;
            }
        } while (choice != 5);
    }

    ~Bank() {
        for (int i = 0; i < accountCount; ++i) {
            delete accounts[i];
        }
    }
};

int main() {
    Bank bank;

    int choice;
    do {
        cout << "\nMain Menu:" << endl;
        cout << "1. Bank Manager" << endl;
        cout << "2. Customer" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            bank.managerMenu();
            break;
        case 2:
            bank.customerMenu();
            break;
        case 3:
            cout << "Exiting program." << endl;
            break;
        default:
            cout << "Invalid choice." << endl;
        }
    } while (choice != 3);

    return 0;
}
