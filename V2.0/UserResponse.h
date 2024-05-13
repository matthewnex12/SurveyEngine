#ifndef USER_RESPONSE_H
#define USER_RESPONSE_H

#include <string>
#include <vector>

// Structure for user responses
struct UserResponse {
    char username[50];
    char surveyTitle[100];
    std::vector<std::string> questions;
    std::vector<std::string> answers;
};

// Function declaration
void saveUserResponse(const UserResponse& response, const std::string& directory);

#endif