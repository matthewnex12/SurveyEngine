#include "Menu.h"
#include "Authentication.h"  // Uncomment or verify the correct header file name
#include "Admin.h"           // Uncomment or verify the correct header file name
#include "User.h"
#include "surveys.h"
#include <iostream>
#include <limits> 
#include <cstdlib>

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
    cout << "2. Profile settings\n";
    cout << "3. Logout\n";
    cin >> choice;

    switch(choice) {
        case 1:
            takeSurvey();
            break;
        case 2:
            profileOptions();
            break;
        case 3:
            displayMainMenu();
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
    }
}

void profileOptions() {
    int choice = 0;

    while (true) {
        cout << "Profile Options\n";
        cout << "1. Change Username\n";
        cout << "2. Change Email\n";
        cout << "3. Change Password\n";
        cout << "4. Return to User Menu\n";
        cin >> choice;

        switch (choice) {
            case 1:
                changeUsername();
                break;
            case 2:
                changeEmail();
                break;
            case 3:
                changePassword();
                break;
            case 4:
                displayUserMenu();
                return;
            default:
                cout << "Invalid option. Try again.\n";
                continue;
        }
    }
}


void displayAdminMenu() {
    int choice = 0;
    cout << "Admin Menu\n";
    cout << "1. Create Survey\n";
    cout << "2. View Users\n";
    cout << "3. Edit Users\n";
    cout << "4. View Surveys\n";
    cout << "5. Edit Surveys\n";
    cout << "6. Logout\n";
    cin >> choice;

    switch(choice) {
        case 1:
            createSurvey();
            break;
        case 2:
            userSurveys();
            break;
        case 3:
            editUser();
            break;
        case 4:
            viewSurveys();
            break;
        case 5:
            editSurvey();
            break;
        case 6:
            displayMainMenu();
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
    }
}
