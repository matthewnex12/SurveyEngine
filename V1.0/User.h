/* 
 * File:   database.h
 * Author: Matthew Rodriguez
 *
 * Created on April 29, 2024, 2:46 PM
 */

#ifndef USER_H
#define USER_H

#include <string>
using namespace std;

struct User {
    string username;
    string password;
    string email;
    bool isAdmin;
};

void saveUser(const User& user);
void readUsers();
void changeUserPassword();

#endif

