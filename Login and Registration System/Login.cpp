//////////////////////   Login and Registration System //////////////////////////////////////
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

class User {
private:
    string username;
    string password;

public:
    User() {}
    User(string uname, string pwd) : username(uname), password(pwd) {}

    string getUsername() { return username; }
    string getPassword() { return password; }

    bool registerUser() {
        if (username.empty() || password.empty()) {
            cout << "Username or password cannot be empty." << endl;
            return false;
        }

        ofstream outFile("database.txt" ,ios::app);
        if (outFile.is_open()) {
            outFile << username << " " << password << endl;
            outFile.close();
            cout << "Registration successful." << endl;
            return true;
        }
        else {
            cout << "Unable to open file." << endl;
            return false;
        }
    }

    bool loginUser() {
        ifstream inFile("database.txt");
        if (inFile.is_open()) {
            string uname, pwd;
            while (inFile >> uname >> pwd) {
                if (uname == username && pwd == password) {
                    inFile.close();
                    return true;
                }
            }
            inFile.close();
            return false;
        }
        else {
            cout << "Unable to open file." << endl;
            return false;
        }
    }
};

void showMenu() {
    cout << "1. Register" << endl;
    cout << "2. Login" << endl;
    cout << "3. Exit" << endl;
    cout << "Enter your choice: ";
}

int main() {
    int choice;
    string username, password;
    User user;

    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            user = User(username, password);
            user.registerUser();
            break;

        case 2:
            cout << "Enter username: ";
            cin >> username;
            cout << "Enter password: ";
            cin >> password;
            user = User(username, password);
            if (user.loginUser()) {
                cout << "Login successful." << endl;
            }
            else {
                cout << "Invalid credentials." << endl;
            }
            break;

        case 3:
            cout << "Exiting..." << endl;
            return 0;

        default:
            cout << "Invalid choice." << endl;
        }
    }

    return 0;
}
