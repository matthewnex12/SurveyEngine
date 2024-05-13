#include "Admin.h"
#include "User.h"
#include "Menu.h"
#include "UserResponse.h"
#include "database.h"
#include "UserSession.h"
#include <iostream>
#include <regex>
#include <filesystem>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <cstring>

using namespace std;
namespace fs = filesystem;

User loggedInUser = {"", "", "", false};

vector<User> loadUsers(const string& userFilePath) {
    vector<User> users;
    ifstream userFile(userFilePath, ios::binary);
    if (!userFile) {
        cout << "Unable to open the user file." << endl;
        return users;
    }

    User user;
    while (userFile.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        users.push_back(user);
    }

    userFile.close();
    return users;
}

vector<UserResponse> loadAllUserResponses(const string& directory) {
    vector<UserResponse> responses;
    for (const auto& entry : fs::directory_iterator(directory)) {
        if (entry.path().extension() == ".bin") {
            ifstream in(entry.path(), ios::binary);
            if (!in) continue;

            UserResponse response;
            in.read(response.username, sizeof(response.username));
            in.read(response.surveyTitle, sizeof(response.surveyTitle));

            int numAnswers = 0;
            in.read(reinterpret_cast<char*>(&numAnswers), sizeof(numAnswers));

            for (int i = 0; i < numAnswers; ++i) {
                int answerLength = 0;
                in.read(reinterpret_cast<char*>(&answerLength), sizeof(answerLength));
                string answer(answerLength, ' ');
                in.read(&answer[0], answerLength);
                response.answers.push_back(answer);
            }

            responses.push_back(response);
            in.close();
        }
    }
    return responses;
}

void saveUsers(const vector<User>& users, const string& filePath) {
    ofstream out(filePath, ios::binary | ios::trunc);
    if (!out) {
        cout << "Error opening file for saving users." << endl;
        return;
    }

    for (const User& user : users) {
        out.write(reinterpret_cast<const char*>(&user), sizeof(User));
    }

    out.close();
}


void editUser() {
    string userFilePath = "Z:\\CIS 17B\\individualProject\\DB\\users.bin";
    vector<User> users = loadUsers(userFilePath);

    if (users.empty()) {
        cout << "No users available to edit." << endl;
        return;
    }

    // Display all usernames for selection
    cout << "Choose a user to edit:" << endl;
    for (size_t i = 0; i < users.size(); ++i) {
        cout << i + 1 << ". " << users[i].username << endl;
    }

    // Get the admin's selection
    size_t choice = 0;
    cout << "Enter the number corresponding to the user: ";
    cin >> choice;

    if (choice == 0 || choice > users.size()) {
        std::cout << "Invalid choice." << endl;
        return;
    }

    // Access the selected user
    User& selectedUser = users[choice - 1];
    string originalEmail(selectedUser.email);

    // Display options for editing the user
    int action = 0;
    cout << "Edit Options for " << selectedUser.username << ":\n";
    cout << "1. Change Username\n";
    cout << "2. Change Email\n";
    cout << "3. Change Password\n";
    cout << "Enter the action number: ";
    cin >> action;

    // Update based on the admin's selection
    switch (action) {
        case 1: {
            string newUsername;
            cout << "Enter new username: ";
            cin >> newUsername;
            strncpy(selectedUser.username, newUsername.c_str(), sizeof(selectedUser.username) - 1);
            selectedUser.username[sizeof(selectedUser.username) - 1] = '\0';
            break;
        }
        case 2: {
            string newEmail;
            cout << "Enter new email: ";
            cin >> newEmail;
            strncpy(selectedUser.email, newEmail.c_str(), sizeof(selectedUser.email) - 1);
            selectedUser.email[sizeof(selectedUser.email) - 1] = '\0';
            break;
        }
        case 3: {
            string newPassword;
            cout << "Enter new password: ";
            cin >> newPassword;
            strncpy(selectedUser.password, newPassword.c_str(), sizeof(selectedUser.password) - 1);
            selectedUser.password[sizeof(selectedUser.password) - 1] = '\0';
            break;
        }
        default:
            cout << "Invalid option." << endl;
            return;
    }

    // Save all users back to the file
    saveUsers(users, userFilePath);
    cout << "User details updated successfully." << endl;
}




bool isValidEmail(const string& email) {
    // email validation
    const regex pattern(R"((\w+)(\.\w+)*@(\w+)(\.\w+)+)");
    return regex_match(email, pattern);
}

bool isValidPassword(const string& password) {
    // Password must have at least 12 characters, numbers, upper/lowercase letters, and special characters
    const regex pattern(R"((?=.*[a-z])(?=.*[A-Z])(?=.*\d)(?=.*[@$!%*?&])[A-Za-z\d@$!%*?&]{12,})");
    return regex_match(password, pattern);
}

void loginUser() {
    string usernameInput, passwordInput;

    cout << "Enter your username: ";
    cin >> usernameInput;

    cout << "Enter your password: ";
    cin >> passwordInput;

    // Check for the special Admin credentials
    if (usernameInput == "Admin" && passwordInput == "Admin123") {
        cout << "Admin login successful! Welcome, Admin." << endl;
        displayAdminMenu();  // Show the admin menu
        return;
    }

    // Check for regular users in the database
    ifstream in(dbPath, std::ios::binary);
    if (!in) {
        cout << "Unable to open file or no users found." << endl;
        return;
    }

    User user;
    bool found = false;

    while (in.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        if (usernameInput == user.username && passwordInput == user.password) {
            User& currentUser = UserSession::getInstance().getCurrentUser();
            currentUser = user;  // Fully populate all fields
            cout << "Login successful! Welcome, " << currentUser.username << "." << endl;
            found = true;

            if (currentUser.isAdmin) {
                displayAdminMenu();
            } else {
                displayUserMenu();
            }
            break;
        }
    }

    in.close();

    if (!found) {
        std::cout << "Invalid username or password." << std::endl;
    }
}


void registerUser() {
    string email, username, password;
    User newUser;

    cout << "Enter your email: ";
    cin >> email;
    cout << "Enter a username: ";
    cin >> username;
    cout << "Enter a password (at least 12 characters): ";
    cin >> password;

    strncpy(newUser.email, email.c_str(), sizeof(newUser.email));
    newUser.email[sizeof(newUser.email) - 1] = '\0';  // Null-terminate
    
    strncpy(newUser.username, username.c_str(), sizeof(newUser.username));
    newUser.username[sizeof(newUser.username) - 1] = '\0';
    
    strncpy(newUser.password, password.c_str(), sizeof(newUser.password));
    newUser.password[sizeof(newUser.password) - 1] = '\0';
    
    newUser.isAdmin = false;

    saveUser(newUser);
}

void updateUserField(const User& updatedUser, const string& originalIdentifier) {
    vector<User> users;
    ifstream in(dbPath, ios::binary);
    if (!in) {
        cout << "Unable to open file at path: " << dbPath << endl;
        return;
    }

    User tempUser;
    while (in.read(reinterpret_cast<char*>(&tempUser), sizeof(User))) {
        users.push_back(tempUser);
    }
    in.close();

    // Use the original identifier to find the correct record
    bool found = false;
    for (auto& user : users) {
        if (strcmp(user.email, originalIdentifier.c_str()) == 0) {
            user = updatedUser;  // Update the entire record
            found = true;
            break;
        }
    }

    if (!found) {
        cout << "Warning: User not found, data was not updated." << endl;
        return;
    }

    // Write all users back to the file
    ofstream out(dbPath, ios::binary | ios::trunc);
    if (!out) {
        cout << "Error opening file for writing." << endl;
        return;
    }

    for (const auto& user : users) {
        out.write(reinterpret_cast<const char*>(&user), sizeof(User));
    }

    out.close();
    cout << "User data successfully updated in the database." << endl;
}

User& getLoggedInUser() {
    static User loggedInUser = {"defaultUser", "defaultPass", "defaultEmail", false};
    return loggedInUser;
}

void changeUsername() {
    string newUsername;
    cout << "Enter new username: ";
    cin >> newUsername;

    User& currentUser = UserSession::getInstance().getCurrentUser();

    if (strlen(currentUser.email) == 0) {
        cout << "Error: Email field is empty. Cannot update without an identifier." << endl;
        return;
    }

    // Store the original email to identify the user
    string originalEmail(currentUser.email);

    // Update the username only
    strncpy(currentUser.username, newUsername.c_str(), sizeof(currentUser.username) - 1);
    currentUser.username[sizeof(currentUser.username) - 1] = '\0';

    // Pass both the user object and the original identifier to update
    updateUserField(currentUser, originalEmail);
}

void changeEmail() {
    string newEmail;
    cout << "Enter new email: ";
    cin >> newEmail;

    User& currentUser = UserSession::getInstance().getCurrentUser();

    // Ensure the username or original email is set
    if (strlen(currentUser.email) == 0 || strlen(currentUser.username) == 0) {
        cout << "Error: Username or email field is empty. Cannot update." << endl;
        return;
    }

    // Validate email length
    if (newEmail.length() >= sizeof(currentUser.email)) {
        cout << "Error: Email is too long. Maximum length is " << (sizeof(currentUser.email) - 1) << " characters." << endl;
        return;
    }

    // Store the original email to identify the user
    string originalEmail(currentUser.email);

    // Update the logged-in user's email
    strncpy(currentUser.email, newEmail.c_str(), sizeof(currentUser.email) - 1);
    currentUser.email[sizeof(currentUser.email) - 1] = '\0';

    // Pass the fully populated user object with the original identifier
    updateUserField(currentUser, originalEmail);
}

void changePassword() {
    string newPassword;
    cout << "Enter new password: ";
    cin >> newPassword;

    // Retrieve the current user
    User& currentUser = UserSession::getInstance().getCurrentUser();

    // Ensure the original identifier is available
    if (strlen(currentUser.email) == 0) {
        cout << "Error: Email field is empty. Cannot update without an identifier." << endl;
        return;
    }

    // Store the original email to identify the user
    string originalEmail(currentUser.email);

    // Update the password only
    strncpy(currentUser.password, newPassword.c_str(), sizeof(currentUser.password) - 1);
    currentUser.password[sizeof(currentUser.password) - 1] = '\0';

    // Pass both the user object and the original identifier to update
    updateUserField(currentUser, originalEmail);
}

void userSurveys() {
    string userFilePath = "Z:\\CIS 17B\\individualProject\\DB\\users.bin";
    vector<User> users;

    // Load all users
    ifstream userFile(userFilePath, ios::binary);
    if (!userFile) {
        cout << "Unable to open the user file." << endl;
        return;
    }

    User user;
    while (userFile.read(reinterpret_cast<char*>(&user), sizeof(User))) {
        users.push_back(user);
    }
    userFile.close();

    if (users.empty()) {
        cout << "No users found." << endl;
        return;
    }

    // Display all users and allow selection
    cout << "Choose a user to view their surveys:" << endl;
    for (size_t i = 0; i < users.size(); ++i) {
        cout << i + 1 << ". " << users[i].username << endl;
    }

    // Get the admin's selection
    size_t userIndex = 0;
    cout << "Enter the number corresponding to the user: ";
    cin >> userIndex;

    if (cin.fail() || userIndex == 0 || userIndex > users.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid number." << endl;
        return;
    }

    string selectedUsername = users[userIndex - 1].username;

    // Load responses for the selected user
    vector<UserResponse> responses = loadAllUserResponses("Z:\\CIS 17B\\individualProject\\DB\\userResponse");

    // Print out the surveys taken by the selected user
    cout << "Surveys taken by " << selectedUsername << ":" << endl;
    bool foundSurveys = false;

    for (const auto& response : responses) {
        if (string(response.username) == selectedUsername) {
            cout << "Survey: " << response.surveyTitle << endl;
            for (size_t i = 0; i < response.answers.size(); ++i) {
                cout << "Answer " << i + 1 << ": " << response.answers[i] << endl;
            }
            cout << "------------------------" << endl;
            foundSurveys = true;
        }
    }

    if (!foundSurveys) {
        cout << "No surveys found for the user." << endl;
    }
}