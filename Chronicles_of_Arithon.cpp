#include <iostream> // Include standard input/output stream library
#include "Authenticator.h" // Include custom authentication header
#include "StoryEngine.h"   // Include custom story/game logic header

using namespace std; // Use the standard namespace for convenience

int main(int argc, char* argv[]) { // Main entry point; argc/argv for command-line arguments
    UserData user; // Create a UserData object to store user info
    user.playerState = new PlayerState(); // Dynamically allocate PlayerState for the user

    bool success = false; // Track authentication or user creation success

    // === CLA branch: Only run if actual arguments provided (besides program name)
    if (argc > 1) { // If command-line arguments are provided
        if (!processArguments(argc, argv, user)) { // Parse and validate arguments; update user
            delete user.playerState; // Clean up allocated memory
            return 1; // Exit with error code
        }

        if (user.isNewUser) { // If user is flagged as new
            success = createUser(user); // Attempt to create a new user profile
        }
        else { // Otherwise, existing user
            success = loginUser(user); // Attempt to log in user
        }

        if (!success) { // If authentication or creation failed
            cout << "\nAuthentication failed via CLA. Exiting...\n"; // Inform user
            delete user.playerState; // Clean up memory
            return 1; // Exit with error code
        }

        showGameWindow(user); // Launch the main game window for the user
    }
    // === In-game menu for normal users
    else { // If no command-line arguments, show interactive menu
        cout << "==============================\n"; // Print game title banner
        cout << "||  CHRONICLES OF ARITHON  ||\n";
        cout << "==============================\n\n";
        cout << "1. Login\n"; // Menu option: Login
        cout << "2. Create New Profile\n"; // Menu option: Create new user
        cout << "3. Exit\n"; // Menu option: Exit game

        int choice = 0; // Store user's menu choice
        while (true) { // Loop until valid action is taken
            cout << "Enter choice: "; // Prompt for menu choice
            if (!(cin >> choice)) {
                cout << "Invalid input. Please enter a number.\n";
                cin.clear(); // reset error flag
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // discard bad input
                continue;
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            if (choice == 1) { // If user chooses Login
                cout << "\nEnter Username: "; // Prompt for username
                getline(cin, user.username); // Read username
                cout << "Enter Password: "; // Prompt for password
                getline(cin, user.password); // Read password
                user.isNewUser = false; // Mark as existing user

                if (loginUser(user)) { // Attempt login
                    success = true; // Mark success
                    break; // Exit menu loop
                }
                else { // If login failed
                    cout << "\nLogin failed. Try again.\n"; // Inform user
                }
            }
            else if (choice == 2) { // If user chooses to create new profile
                cout << "Enter New Username: "; // Prompt for new username
                getline(cin, user.username); // Read username
                cout << "Enter Password: "; // Prompt for password
                getline(cin, user.password); // Read password
                cout << "Confirm Password: "; // Prompt for password confirmation
                string confirm; // Store confirmation input
                getline(cin, confirm); // Read confirmation

                if (!isValidInput(user.username, user.password, confirm)) continue; // Validate input; repeat if invalid

                user.isNewUser = true; // Mark as new user
                if (createUser(user)) { // Attempt to create user
                    success = true; // Mark success
                    break; // Exit menu loop
                }
                else { // If creation failed
                    cout << "\nUser creation failed. Try again.\n"; // Inform user
                }
            }
            else if (choice == 3) { // If user chooses to exit
                cout << "Exiting game.\n"; // Inform user
                delete user.playerState; // Clean up memory
                return 0; // Exit program
            }
            else { // If input is invalid
                cout << "\nInvalid input. Try again.\n"; // Inform user
            }
        }

        if (success) { // If login or creation succeeded
            showGameWindow(user); // Launch the main game window
        }
    }

    delete user.playerState; // Clean up allocated PlayerState memory
    return 0; // Exit program successfully
}
