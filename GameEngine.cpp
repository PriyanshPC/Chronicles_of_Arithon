#include <iostream> // For input/output stream operations
#include <string>   // For using std::string
#include <cstdlib>  // For system() and exit()
#include "GameEngine.h" // Header for authentication-related declarations
#include "FileManager.h"   // Header for file management functions
#include "StoryEngine.h"   // Header for story engine functions

using namespace std; // Use the standard namespace for convenience

// Displays the help menu to the user
void showHelp() {
    std::cout << "\n=== CHRONICLES OF ARITHON - HELP MENU ===\n\n";
    std::cout << "This is a text-based interactive story game. Choices you make affect the story's outcome.\n";
    std::cout << "Before playing, you must either create a new user profile or log in to an existing one.\n\n";

    std::cout << "=== CREATE A NEW USER ===\n";
    std::cout << "Use this format to create a new profile:\n";
    std::cout << "  .\\TeamsProject.exe \"nuser-<username>-pswd-<password>-cpswd-<confirmPassword>\"\n";
    std::cout << "Example:\n";
    std::cout << "  .\\TeamsProject.exe \"nuser-arin-pswd-eldora123-cpswd-eldora123\"\n\n";

    std::cout << "=== LOGIN AN EXISTING USER ===\n";
    std::cout << "Use this format to login with an existing profile:\n";
    std::cout << "  .\\TeamsProject.exe \"euser-<username>-pswd-<password>\"\n";
    std::cout << "Example:\n";
    std::cout << "  .\\TeamsProject.exe \"euser-arin-pswd-eldora123\"\n\n";

    std::cout << "=== GET HELP ===\n";
    std::cout << "To show this help menu at any time:\n";
    std::cout << "  .\\TeamsProject.exe -info\n";

    std::cout << "\nOnce logged in, you can:\n";
    std::cout << "  -> Continue your saved story\n";
    std::cout << "  -> Start a new adventure\n";
    std::cout << "  -> Delete your profile\n";
    std::cout << "  -> Exit the game\n";

    std::cout << "\nMake decisions wisely — your story depends on them.\n";
    std::cout << "=============================================\n";
}

// Displays the main game window and menu
void showGameWindow(const UserData& user) {
    system("cls"); // Clear the console screen

    std::cout << "========================================\n";
    std::cout << "||  WELCOME TO CHRONICLES OF ARITHON  ||\n";
    std::cout << "========================================\n\n";

    if (user.isNewUser) { // Greet new user
        std::cout << "Welcome, " << user.username << "!\nYour adventure begins here!\n";
    }
    else { // Greet returning user
        std::cout << "Welcome back, " << user.username << "!\n";
        std::cout << "\n";
    }

    cout << "\n===|| GAME MENU ||===\n";
    if (!user.isNewUser) { // Menu for existing users
        cout << "1. Continue Game\n2. Start New Game\n3. Delete Profile\n4. Exit Game\n\n";
    }
    else { // Menu for new users
        cout << "1. Start New Game\n2. Delete Profile\n3. Exit Game\n\n";
    }

    bool menuActive = true; // Menu loop control
    while (menuActive) {
        string input; // User input string
        int choice = 0; // Menu choice
        cout << "Enter your choice: ";
        getline(cin, input); // Get user input
        try {
            choice = stoi(input); // Convert input to integer
            menuActive = !handleMenuChoice(choice, const_cast<UserData&>(user)); // Handle menu choice
        }
        catch (...) { // Handle invalid input
            cout << "Invalid input! Please enter a number.\n";
        }
    }
}

// Handles the user's menu selection and triggers the appropriate action
bool handleMenuChoice(int choice, UserData& user) {
    cout << endl;
    if (!user.isNewUser) { // Menu for existing users
        if (choice < 1 || choice > 4) { // Validate menu option
            cout << "Invalid menu option! Please choose a valid number.\n";
            return false;
        }

        switch (choice) {
        case 1: // Continue game
            cout << "CONTINUING GAME...\n";
            initializeStoryEngine(user);
            return true;

        case 2: // Start new game
            cout << "STARTING NEW GAME...\n";
            user.playerState->resetToDefault();
            initializeStoryEngine(user);
            return true;

        case 3: // Delete profile
            cout << "DELETE PROFILE...";
            if (deleteConfirmation()) { // Confirm deletion
                if (deleteUser(user.username)) { // Delete user file
                    cout << "Profile deleted. Exiting...\n";
                    exit(0); // Exit program
                }
                else {
                    cout << "Failed to delete profile.\n";
                }
            }
            else {
                cout << "Deletion canceled.\n";
            }
            return false;

        case 4: // Exit game
            if (exitRequest(user)) { // Confirm exit
                exit(0); // Exit program
            }
            return false;

        default: // Should not reach here
            cout << "Invalid choice.\n";
            return false;
        }
    }
    else { // Menu for new users
        if (choice < 1 || choice > 3) { // Validate menu option
            cout << "Invalid menu option! Please choose a valid number.\n";
            return false;
        }

        switch (choice) {
        case 1: // Start new game
            cout << "STARTING NEW GAME...\n";
            user.playerState->resetToDefault();
            initializeStoryEngine(user);
            return true;

        case 2: // Delete profile
            cout << "DELETE PROFILE...";
            if (deleteConfirmation()) { // Confirm deletion
                if (deleteUser(user.username)) { // Delete user file
                    cout << "Profile deleted. Exiting...\n";
                    exit(0); // Exit program
                }
                else {
                    cout << "Failed to delete profile.\n";
                }
            }
            else {
                cout << "Deletion canceled.\n";
            }
            return false;

        case 3: // Exit game
            if (exitRequest(user)) { // Confirm exit
                exit(0); // Exit program
            }
            return false;

        default: // Should not reach here
            cout << "Invalid choice.\n";
            return false;
        }
    }

    return true; // Continue menu loop
}

// Asks the user to confirm profile deletion
bool deleteConfirmation() {
    std::string input;

    cout << "\nAre you sure you want to delete your profile? (y/n): ";
    getline(cin, input); // Get user input

    return !input.empty() && (toupper(input[0]) == 'Y'); // Return true if 'Y' or 'y'
}

void showMainMenu() {
    cout << "=============================="<<endl;             // Print game title banner
    cout << "||  CHRONICLES OF ARITHON  ||" << endl;
    cout << "==============================" << endl << endl;
    cout << "1. Login" << endl;                                   // Menu option: Login
    cout << "2. Create New Profile" << endl;                      // Menu option: Create new user
    cout << "3. Exit" << endl;                                    // Menu option: Exit game
	cout << "------------------------------" << endl;             // Separator line
    cout << "Please select an option: ";
}

