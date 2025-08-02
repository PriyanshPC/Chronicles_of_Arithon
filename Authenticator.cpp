#include <iostream>
#include <string>
#include <cstdlib>
#include "Authenticator.h"
#include "FileManager.h"
#include "PlayerState.h"
#include "StoryEngine.h"

using namespace std;

void showHelp() {
    cout << "\n=== CHRONICLES OF ARITHON - HELP ===" << endl;
    cout << "CREATE NEW USER:\n  .\\TeamsProject.exe \"nuser-john-pswd-pass123-cpswd-pass123\"\n";
    cout << "LOGIN EXISTING USER:\n  .\\TeamsProject.exe \"euser-john-pswd-pass123\"\n";
    cout << "Use -init for help.\n";
}

bool processArguments(int argc, char* argv[], UserData& user) {
    user.playerState = new PlayerState();  // allocate memory

    if (argc != 2) {
        cout << "Error: Invalid number of arguments!\nUse -init for help.\n";
        return false;
    }

    string input = argv[1];

    if (input == "-init") {
        showHelp();
        return false;
    }

    if (input.find("nuser-") == 0) {
        user.isNewUser = true;
        int userStart = 6;
        int pswdPos = input.find("-pswd-");
        int cpswdPos = input.find("-cpswd-");

        if (pswdPos == string::npos || cpswdPos == string::npos) {
            cout << "Error: Wrong format! Use -init for help.\n";
            return false;
        }

        user.username = input.substr(userStart, pswdPos - userStart);
        string password = input.substr(pswdPos + 6, cpswdPos - pswdPos - 6);
        string confirmPass = input.substr(cpswdPos + 7);

        if (!isValidInput(user.username, password, confirmPass)) return false;

        user.password = password;
        return true;
    }

    if (input.find("euser-") == 0) {
        user.isNewUser = false;

        int userStart = 6;
        int pswdPos = input.find("-pswd-");
        if (pswdPos == string::npos) {
            cout << "Error: Wrong format! Use -init for help.\n";
            return false;
        }

        user.username = input.substr(userStart, pswdPos - userStart);
        user.password = input.substr(pswdPos + 6);

        if (!isValidInput(user.username, user.password)) return false;
        return true;
    }

    cout << "Error: Unknown command! Use -init for help.\n";
    return false;
}

bool isValidInput(const string& username, const string& password, const string& confirmPass) {
    if (username.empty()) {
        cout << "Error: Username cannot be empty!\n";
        return false;
    }
    if (password.length() < 4) {
        cout << "Error: Password must be at least 4 characters!\n";
        return false;
    }
    if (!confirmPass.empty() && password != confirmPass) {
        cout << "Error: Passwords don't match!\n";
        return false;
    }
    return true;
}

string encrypt(const string& text) {
    string encrypted = text;
    for (char& c : encrypted) c += 3;
    return encrypted;
}

bool createUser(UserData& user) {
    if (userExists(user.username)) {
        cout << "Error: User '" << user.username << "' already exists!\n";
        return false;
    }

    user.password = encrypt(user.password);
    user.playerState->resetToDefault();

    if (saveUser(user)) {
        return true;
    }
    else {
        cout << " Failed to save user to file!\n";
        return false;
    }
}

bool loginUser(UserData& user) {
    if (!userExists(user.username)) {
        cout << "Error: User '" << user.username << "' not found!\n";
        return false;
    }

    UserData storedUser;
    storedUser.playerState = new PlayerState();

    if (!loadUser(user.username, storedUser)) {
        cout << "Error: Failed to load user data!\n";
        return false;
    }

    if (storedUser.password != encrypt(user.password)) {
        cout << "Error: Wrong password!\n";
        return false;
    }

    *user.playerState = *storedUser.playerState;
    user.password = storedUser.password;
    return true;
}

void showGameWindow(const UserData& user) {
    system("cls");

    cout << "========================================\n";
    cout << "||  WELCOME TO CHRONICLES OF ARITHON  ||\n";
    cout << "========================================\n\n";

    if (user.isNewUser) {
        cout << "Welcome, " << user.username << "!\nYour adventure begins here!\n";
    }
    else {
        cout << "Welcome back, " << user.username << "!\n";
        cout << "\n=== PLAYER STATS ===\n";
        cout << "Chapter: " << user.playerState->getChapterNumber() << "\n";
        cout << "Event: " << user.playerState->getChapterEvent() << "\n";
        cout << "Health: " << user.playerState->getHealth() << "\n";

        cout << "Weapon: ";
        switch (user.playerState->getWeapon()) {
        case 1: cout << "Sword"; break;
        case 2: cout << "Crossbow"; break;
        case 3: cout << "Twin-Blade Sword"; break;
        default: cout << "Unknown"; break;
        }
        cout << "\n";
    }

    cout << "\n===|| GAME MENU ||===\n";
    if (!user.isNewUser) {
        cout << "1. Continue Game\n2. Start New Game\n3. Delete Profile\n4. Exit Game\n\n";
    }
    else {
        cout << "1. Start New Game\n2. Delete Profile\n3. Exit Game\n\n";
    }

    bool menuActive = true;
    while (menuActive) {
        string input;
        int choice = 0;
        cout << "Enter your choice: ";
        getline(cin, input);
        try {
            choice = stoi(input);
            menuActive = !handleMenuChoice(choice, const_cast<UserData&>(user));
        }
        catch (...) {
            cout << "Invalid input! Please enter a number.\n";
        }
    }
}

bool handleMenuChoice(int choice, UserData& user) {
    cout << endl;
    if (!user.isNewUser) {
        if (choice < 1 || choice > 4) {
            cout << "Invalid menu option! Please choose a valid number.\n";
            return false;
        }

        switch (choice) {
        case 1:
            cout << "CONTINUING GAME...\n";
            initializeStoryEngine(user);
            return true;

        case 2:
            cout << "STARTING NEW GAME...\n";
            user.playerState->resetToDefault();
            initializeStoryEngine(user);
            return true;

        case 3:
            cout << "DELETE PROFILE...";
            if (deleteConfirmation()) {
                if (deleteUser(user.username)) {
                    cout << "Profile deleted. Exiting...\n";
                    exit(0);
                }
                else {
                    cout << "Failed to delete profile.\n";
                }
            }
            else {
                cout << "Deletion canceled.\n";
            }
            return false;


        case 4:
            if (exitRequest(user)) {
                exit(0);
            }
            return false;

        default:
            cout << "Invalid choice.\n";
            return false;
        }
    }
    else {
        if (choice < 1 || choice > 3) {
            cout << "Invalid menu option! Please choose a valid number.\n";
            return false;
        }

        switch (choice) {
        case 1:
            cout << "STARTING NEW GAME...\n";
            user.playerState->resetToDefault();
            initializeStoryEngine(user);
            return true;

        case 2:
            cout << "DELETE PROFILE...";
            if (deleteConfirmation()) {
                if (deleteUser(user.username)) {
                    cout << "Profile deleted. Exiting...\n";
                    exit(0);
                }
                else {
                    cout << "Failed to delete profile.\n";
                }
            }
            else {
                cout << "Deletion canceled.\n";
            }
            return false;

        case 3:
            if (exitRequest(user)) {
                exit(0);
            }
            return false;

        default:
            cout << "Invalid choice.\n";
            return false;
        }
    }

    return true;
}

bool deleteConfirmation() {
    std::string input;

    cout << "\nAre you sure you want to delete your profile? (y/n): ";
    getline(cin, input);

    return !input.empty() && (toupper(input[0]) == 'Y');
}
