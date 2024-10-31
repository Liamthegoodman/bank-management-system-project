#include <iostream>
#include <string>
#include <map>
#include <fstream>

using namespace std;

#define MIN_BALANCE 500

class Account {
private:
    string fname;
    string lname;
    float balance;
    static long next;
    long accountNumber;

public:
    Account() {} 
    Account(string fname, string lname, float balance) {
        this->fname = fname;
        this->lname = lname;
        this->balance = balance;
        this->accountNumber = ++next;
    }

    void deposit(int amount) {
        balance += amount;
    }

    void withdrawal(float amount) {
        if (balance - amount < MIN_BALANCE) {
            cout << "Insufficient amount" << endl;
        } else {
            balance -= amount;
        }
    }

    string getFname() { return fname; }
    string getLname() { return lname; }
    float getBalance() { return balance; }
    long getAccountNumber() { return accountNumber; }

    static long getNext() { return next; }
    static void setNext(long nextAccountNumber) { next = nextAccountNumber; }

    friend ofstream& operator<<(ofstream& ofs, Account& acc);
    friend ifstream& operator>>(ifstream& ifs, Account& acc);
    friend ostream& operator<<(ostream& os, const Account& acc); 
};

ofstream& operator<<(ofstream& ofs, Account& acc) {
    ofs << acc.accountNumber << endl;
    ofs << acc.fname << endl;
    ofs << acc.lname << endl;
    ofs << acc.balance << endl;
    return ofs;
}

ifstream& operator>>(ifstream& ifs, Account& acc) {
    ifs >> acc.accountNumber;
    ifs >> acc.fname;
    ifs >> acc.lname;
    ifs >> acc.balance;
    return ifs;
}

ostream& operator<<(ostream& os, const Account& acc) {
    os << "Account Number: " << acc.accountNumber << endl;
    os << "First Name: " << acc.fname << endl;
    os << "Last Name: " << acc.lname << endl;
    os << "Balance: " << acc.balance << endl;
    return os;
}

long Account::next = 0;

class Bank {
private:
    map<long, Account> accounts;

public:
    Bank() {
        Account account;
        ifstream infile;
        infile.open("Bank.data");
        if (!infile) {
            cout << "Error in Opening. File Not Found" << endl;
            return;
        }
        while (infile >> account) {
            accounts.insert(pair<long, Account>(account.getAccountNumber(), account));
        }
        Account::setNext(account.getAccountNumber());
        infile.close();
    }

    Account OpenAccount(string fname, string lname, float balance) { 
        Account account(fname, lname, balance);
        accounts.insert(make_pair(account.getAccountNumber(), account));
        ofstream outfile("Bank.data", ios::trunc);
        for (auto itr = accounts.begin(); itr != accounts.end(); ++itr) {
            outfile << itr->second;
        }
        outfile.close();
        return account;
    }

    Account BalanceEnquiry(long accountNumber) {
        auto itr = accounts.find(accountNumber);
        if (itr == accounts.end()) {
            throw runtime_error("Account not found");
        }
        return itr->second;
    }

    void Deposit(long accountNumber, float amount) {
        auto itr = accounts.find(accountNumber);
        if (itr == accounts.end()) {
            throw runtime_error("Account not found");
        }
        itr->second.deposit(amount);
    }

    void Withdraw(long accountNumber, float amount) {
        auto itr = accounts.find(accountNumber);
        if (itr == accounts.end()) {
            throw runtime_error("Account not found");
        }
        itr->second.withdrawal(amount);
    }

    void CloseAccount(long accountNumber) {
        auto itr = accounts.find(accountNumber);
        if (itr == accounts.end()) {
            throw runtime_error("Account not found");
        }
        accounts.erase(itr); 
    }

    void ShowAllAccounts() {
        for (auto itr = accounts.begin(); itr != accounts.end(); ++itr) {
            cout << itr->second; 
            cout << "----------------------" << endl;
        }
    }

    ~Bank() {
        ofstream outfile("Bank.data", ios::trunc);
        for (auto itr = accounts.begin(); itr != accounts.end(); ++itr) {
            outfile << itr->second;
        }
        outfile.close();
    }
};

int main() {
    Bank b;
    Account acc;

    int choice;
    string fname, lname;
    long accountNumber;
    float balance;
    float amount;
    cout << "***Banking System***" << endl;
    do {
        cout << "\n\tSelect one option below ";
        cout << "\n\t1 Open an Account";
        cout << "\n\t2 Balance Enquiry";
        cout << "\n\t3 Deposit";
        cout << "\n\t4 Withdrawal";
        cout << "\n\t5 Close an Account";
        cout << "\n\t6 Show All Accounts";
        cout << "\n\t7 Quit";
        cout << "\nEnter your choice: ";
        cin >> choice;
        switch (choice) {
        case 1:
            cout << "Enter First Name: ";
            cin >> fname;
            cout << "Enter Last Name: ";
            cin >> lname;
            cout << "Enter Initial Balance: ";
            cin >> balance;
            acc = b.OpenAccount(fname, lname, balance); 
            cout << endl << "Congratulations! Account is created" << endl;
            cout << acc;
            break;
        case 2:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            try {
                acc = b.BalanceEnquiry(accountNumber);
                cout << endl << "Your Account Details" << endl;
                cout << acc;
            } catch (runtime_error &e) {
                cout << e.what() << endl;
            }
            break;
        case 3:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Amount: ";
            cin >> amount;
            try {
                b.Deposit(accountNumber, amount);
                cout << endl << "Amount is Deposited" << endl;
                acc = b.BalanceEnquiry(accountNumber);
                cout << acc;
            } catch (runtime_error &e) {
                cout << e.what() << endl;
            }
            break;
        case 4:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Amount: ";
            cin >> amount;
            try {
                b.Withdraw(accountNumber, amount);
                cout << endl << "Amount Withdrawn" << endl;
                acc = b.BalanceEnquiry(accountNumber);
                cout << acc;
            } catch (runtime_error &e) {
                cout << e.what() << endl;
            }
            break;
        case 5:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            try {
                b.CloseAccount(accountNumber);
                cout << endl << "Account is Closed" << endl;
            } catch (runtime_error &e) {
                cout << e.what() << endl;
            }
            break;
        case 6:
            b.ShowAllAccounts();
            break;
        case 7:
            break;
        default:
            cout << "\nEnter correct choice";
            exit(0);
        }
    } while (choice != 7);

    return 0;
}
