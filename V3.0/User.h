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
    char username[50];
    char email[50];
    char password[50];
    bool isAdmin;
};

extern User loggedInUser;


void saveUser(const User& user);
void readUsers();
void changeUserPassword();
void profileOptions();
void changeUsername();
void changeEmail();
void changePassword();

#endif

