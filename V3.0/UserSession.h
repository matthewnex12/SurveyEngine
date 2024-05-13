#ifndef USER_SESSION_H
#define USER_SESSION_H

#include "User.h"

class UserSession {
private:
    User currentUser;

    // Private constructor to enforce the singleton pattern
    UserSession() {}

public:
    // Access the single instance of UserSession
    static UserSession& getInstance() {
        static UserSession instance;
        return instance;
    }

    // Delete copy constructor and assignment operator to prevent copies
    UserSession(const UserSession&) = delete;
    void operator=(const UserSession&) = delete;

    // Getter for the current user
    User& getCurrentUser() {
        return currentUser;
    }

    // Setter for the current user
    void setCurrentUser(const User& user) {
        currentUser = user;
    }
};

#endif // USER_SESSION_H
