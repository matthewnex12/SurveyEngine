/*
/* 
 * File:   database.cpp
 * Author: Matthew Rodriguez
 * 
 * Created on April 29, 2024, 2:46 PM
 */

#include "User.h"
#include <fstream>
using namespace std;

void saveUser(const User& user) {
    ofstream out("users.bin", ios::binary | ios::app);
    out.write((char*)&user, sizeof(User));
    out.close();
}

void readUsers() {
    ifstream in("users.bin", ios::binary);
    User user;
    while (in.read((char*)&user, sizeof(User))) {
        // Process user
    }
    in.close();
}

