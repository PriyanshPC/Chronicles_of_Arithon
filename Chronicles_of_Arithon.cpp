#include <iostream>                                                         // Include standard input/output stream library
#include "GameEngine.h"                                                     // Include custom authentication header
#include "Authenticator.h"                                                  // Include custom authenticator header
using namespace std; // Use the standard namespace for convenience

bool handleCommandLineLogin(UserData& user, Authenticator& AuthInstance, int argCount, char** argVector)
{
    if (!AuthInstance.processInputArguments(argCount, argVector)) {             // Process the command-line arguments
        cout << "[Main] Command Line Argument Parsing Failed" << endl;           //If processing fails, inform user
        return false;
    }
    user.username = AuthInstance.getUsername();
    user.password = AuthInstance.getPassword();
    user.isNewUser = AuthInstance.isNewUserFlag();

    if (!AuthInstance.logIn(user)) {                         // If authentication or creation failed
        cout << "[Main] Command Line Login Failed!!";        // Inform user
        return false;
    }
    return true;
}

bool handleInteractiveLogin(UserData& user, Authenticator& AuthInstance)
{
    int choice = 0;                                                     // Store user's menu choice
    while (true) {                                                      // Loop until valid action is taken
        bool success = false;                                           // Reset success flag for each menu iteration
        showMainMenu();
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear(); // reset error flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');        // discard bad input
            continue;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if (choice == 1) {                                      // If user chooses Login (has to be an existing user)
            cout << "\nEnter Username: ";                       // Prompt for username
            getline(cin, user.username);                        // Read username
            cout << "Enter Password: ";                         // Prompt for password
            getline(cin, user.password);                        // Read password
            user.isNewUser = false;                             // Mark as existing user

            AuthInstance.setInputVariables(user.username, user.password, user.isNewUser, false);
            success = AuthInstance.logIn(user); // Attempt to log in using Authenticator

            if (success) {                              // If login succeeded
                cout << "\nLogin successful!\n";        // Inform user
                return true;
            }
            else {                                              // If login failed
                cout << "\nLogin failed. Try again.\n";         // Inform user
            }
        }
        else if (choice == 2) {                                 // If user chooses to create new profile
            user.isNewUser = true;                              // Mark as new user

            // Prompt for new user details
            cout << "Enter New Username: ";                     // Prompt for new username
            getline(cin, user.username);                        // Read username
            cout << "Enter Password: ";                         // Prompt for password
            getline(cin, user.password);                        // Read password
            cout << "Confirm Password: ";                       // Prompt for password confirmation
            string confirm;                                     // Store confirmation input
            getline(cin, confirm);                              // Read confirmation

            // only set the authenticator variables if the user is creating a new user
            // and if the confirm password matches the password
            if (user.password == confirm) {
                AuthInstance.setInputVariables(user.username, user.password, user.isNewUser, false);
            }
            else {
                cout << "Passwords do not match!! Try again." << endl;
                continue;
            }

            // Attempt to create a new user using Authenticator
            // Using Authenticator login function to create a new user
            success = AuthInstance.logIn(user); // Attempt to log in with new user credentials
            if (success) {
                cout << "New user created and logged in." << endl;
                return true;
            }
            else {                                                                  // If creation failed
                cout << "User creation failed! Try again." << endl;                 // Inform user
            }
        }
        else if (choice == 3) {                                                     // If user chooses to exit
            cout << "Exiting game." << endl;                                              // Inform user
            return false;
        }
        else {                                                                  // If input is invalid
            cout << "\nInvalid input! Try again." << endl;                            // Inform user
        }
    }
    return true;
}

int main(int argCount, char** argVector) {                                  // Main entry point; argCount/argVector for command-line arguments

    UserData user;                                                          // Create a UserData object to store user info
    user.playerState = new PlayerState();                                   // Dynamically allocate PlayerState for the user
    Authenticator AuthInstance(argCount, argVector);                        // Initialize Authenticator with command-line arguments

    bool success = (argCount > 1)                                           // If command-line arguments are provided
        ? handleCommandLineLogin(user, AuthInstance, argCount, argVector)   // Attempt command-line login or user creation
        : handleInteractiveLogin(user, AuthInstance);                       // Otherwise, handle interactive login

    // === Launch game window if login or creation succeeded
    if (success) {
        showGameWindow(user);
    }

    delete user.playerState;                                                            // Clean up allocated PlayerState memory
    return 0;                                                                           // Exit program successfully
}
