
// MADE BY KARKAS

#include <iostream>
#include <Windows.h>
#include <cstdlib>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include <ctime>
#define _CRT_SECURE_NO_WARNINGS 1

using namespace std;

//############################## Log Action Function #############################

void LogAction(const string& action, const string& desc, double amount) {
    ofstream logFile("expense_log.txt", ios::app);
    auto now = chrono::system_clock::now();
    time_t now_time = chrono::system_clock::to_time_t(now);
    string timeStr = ctime(&now_time);
    if (!timeStr.empty() && timeStr.back() == '\n') timeStr.pop_back();
    logFile << "[" << timeStr << "] " << action << ": \"" << desc << "\" - " << amount << endl;
    logFile.close();
}

//############################## Export to CSV Function #############################

void ExportToCSV(const vector<pair<string, double>>& expenses) {
    ofstream exportFile("exported_expenses.csv");
    exportFile << "Description,Amount\n";
    for (const auto& exp : expenses) {
        exportFile << "\"" << exp.first << "\"," << exp.second << "\n";
    }
    exportFile.close();
    cout << "Expenses exported to exported_expenses.csv\n" << endl;
}

//############################## Save Expenses Function #############################

void SaveExpenses(const vector<pair<string, double>>& expenses) {
    ofstream file("expenses.csv");
    file << "Description,Amount\n";
    for (const auto& exp : expenses) {
        file << "\"" << exp.first << "\"," << exp.second << "\n";
    }
    file.close();
}

//################################# Expense Tracker Function #############################

void LoadExpenses(vector<pair<string, double>>& expenses, double& total) {
    ifstream file("expenses.csv");
    string line;
    getline(file, line); 
    while (getline(file, line)) {
        size_t comma = line.find_last_of(',');
        if (comma != string::npos) {
            string desc = line.substr(0, comma);
            if (desc.front() == '"' && desc.back() == '"') {
                desc = desc.substr(1, desc.size() - 2);
            }
            double amount = stod(line.substr(comma + 1));
            expenses.push_back({ desc, amount });
            total += amount;
        }
    }
    file.close();
}

void DeleteExpense(vector<pair<string, double>>& expenses, double& total) {
    if (expenses.empty()) {
        cout << "No expenses to delete.\n" << endl;
        return;
    }
    cout << "\n--- Expenses ---\n";
    for (size_t i = 0; i < expenses.size(); ++i) {
        cout << i + 1 << ". " << expenses[i].first << " - " << expenses[i].second << endl;
    }
    cout << "Enter the number of the expense to delete: ";
    size_t idx;
    cin >> idx;
    cin.ignore();
    if (idx < 1 || idx > expenses.size()) {
        cout << "Invalid number.\n" << endl;
        return;
    }
    // Log deletion
    LogAction("Deleted", expenses[idx - 1].first, expenses[idx - 1].second);
    total -= expenses[idx - 1].second;
    expenses.erase(expenses.begin() + idx - 1);
    SaveExpenses(expenses);
    cout << "Expense deleted!\n" << endl;
}

void ExpenseTracker() {
    double total = 0.0;
    string desc;
    double amount;
    int option;
    vector<pair<string, double>> expenses;

    // Load previous expenses from CSV
    LoadExpenses(expenses, total);

    ofstream file("expenses.csv", ios::app);
    ifstream check("expenses.csv");
    if (check.peek() == ifstream::traits_type::eof()) {
        file << "Description,Amount\n";
    }
    check.close();

    do {
        cout << "\n------ Expense Tracker ------\n";
        cout << "1. Add Expense\n";
        cout << "2. Show Total\n";
        cout << "3. Export to CSV\n";
        cout << "4. Delete Expense\n";
        cout << "5. Exit to Menu\n";
        cout << "Choose option: ";
        cin >> option;
        cin.ignore(); 

        cout << endl;

        switch (option) {
        case 1:
            cout << "Enter description: ";
            getline(cin, desc);
            cout << "Enter amount: ";
            cin >> amount;
            cin.ignore();
            total += amount;
            expenses.push_back({ desc, amount });
            // Write to CSV
            file << "\"" << desc << "\"," << amount << "\n";
            // Log addition
            LogAction("Added", desc, amount);
            cout << "Expense added!\n" << endl;
            break;
        case 2:
            cout << "Total expenses: " << total << "\n" << endl;
            break;
        case 3:
            ExportToCSV(expenses);
            break;
        case 4:
            DeleteExpense(expenses, total);
            break;
        case 5:
            cout << "Returning to main menu...\n" << endl;
            break;
        default:
            cout << "Invalid option.\n" << endl;
        }
    } while (option != 5);

    file.close();
}

void RegisterUser() {
    string username, password;
    cout << "\nChoose a username: ";
    cin >> username;
    cout << "Choose a password: ";
    cin >> password;

    // Save credentials to users.txt
    ofstream usersFile("users.txt", ios::app);
    usersFile << username << " " << password << endl;
    usersFile.close();

    cout << "Registration successful! You can now log in.\n" << endl;
}

bool Login() {
    string name, password;
    cout << "\nGive username: ";
    cin >> name;
    cout << "Give password: ";
    cin >> password;

    ifstream usersFile("users.txt");
    string fileUser, filePass;
    while (usersFile >> fileUser >> filePass) {
        if (fileUser == name && filePass == password) {
            cout << "\nSuccess\n" << endl;
            usersFile.close();
            return true;
        }
    }
    usersFile.close();
    cout << "\nWrong username or password\n" << endl;
    cout << "Closing in 10 seconds..." << endl;
    exit(1000);
    return false;
}

int main() {

    system("Color 0B");
    cout << "EXPENSE MENU\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Exit\n";

    int choice;
    bool loggedIn = false;
    cout << "Choose option: ";
    cin >> choice;
    cout << endl;
    switch (choice) {
    case 1:
        RegisterUser();
        main();
        break;
    case 2:
        loggedIn = Login();
        if (loggedIn) {
            ExpenseTracker();
        }
        cout << endl;
        break;
    case 3:
        exit(0);
    default:
        cout << "Invalid input\n";
    }
}