#include "database.h"
#include <fstream>
#include <filesystem>
#include <iostream>
using namespace std;

const string dbPath = "Z:\\CIS 17B\\individualProject\\DB\\users.bin";

void ensureDirectoryExists() {
    string dirPath = "Z:\\CIS 17B\\individualProject\\DB";
    if (!filesystem::exists(dirPath)) {
        filesystem::create_directories(dirPath);
    }
}

void saveUser(const User& user) {
    ofstream out(dbPath, ios::binary | ios::app);
    if (!out) {
        cout << "Error opening file for saving user." << endl;
        return;
    }

    out.write(reinterpret_cast<const char*>(&user), sizeof(User));
    out.close();
}

void readUsers() {
    ifstream in(dbPath, ios::binary);
    if (!in) {
        cout << "No users found or unable to open file at path: " << dbPath << endl;
        return;
    }

    User user;
    cout << "Reading user data from file: " << dbPath << endl; // Debug message

    bool foundUsers = false; // Track if any users are found

    while (in.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        cout << "Found user: " << user.username << endl;
        foundUsers = true;
    }

    if (!foundUsers) {
        cout << "No users found in the binary file." << endl;
    }

    in.close();
}