#ifndef DATABASE_H
#define DATABASE_H

#include "User.h"

extern const std::string dbPath;
void saveUser(const User& user);
void readUsers();

#endif
