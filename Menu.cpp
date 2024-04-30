#include "Menu.h"
#include "Authentication.h"  // Uncomment or verify the correct header file name
#include "Admin.h"           // Uncomment or verify the correct header file name
#include "User.h"
#include <iostream>
#include <limits>            // Include for numeric_limits

using namespace std;

void displayMainMenu() {
    int choice = 0;
    while (true) {
        cout << "Main Menu\n";
        cout << "1. Login\n";
        cout << "2. Register\n";
        cout << "3. Exit\n";
        cin >> choice;

        if (cin.fail()) {
            cin.clear(); // Clears error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignores incorrect input
            cout << "Invalid input. Please enter a number.\n";
            continue;
        }

        switch(choice) {
            case 1:
                loginUser();
                return;
            case 2:
                registerUser();
                return;
            case 3:
                cout << "Exiting program.\n";
                exit(0);
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}

void displayUserMenu() {
    int choice = 0;
    cout << "User Menu\n";
    cout << "1. Take Survey\n";
    cout << "2. Change Password\n";
    cout << "3. Logout\n";
    cin >> choice;

    switch(choice) {
        case 1:
            takeSurvey();
            break;
        case 2:
            changeUserPassword();
            break;
        case 3:
            displayMainMenu();
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
    }
}

void displayAdminMenu() {
    int choice = 0;
    cout << "Admin Menu\n";
    cout << "1. Create Survey\n";
    cout << "2. Edit User\n";
    cout << "3. View Surveys\n";
    cout << "4. Logout\n";
    cin >> choice;

    switch(choice) {
        case 1:
            createSurvey();
            break;
        case 2:
            editUser();
            break;
        case 3:
            viewSurveys();
            break;
        case 4:
            displayMainMenu();
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
    }
}
