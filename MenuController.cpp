#include <iostream> // For input/output stream operations
#include <string>   // For using std::string
#include <cstdlib>  // For system() and exit()
#include <conio.h>
#include "MenuController.h" // Header for authentication-related declarations
#include "FileManager.h"   // Header for file management functions
#include "StoryEngine.h"   // Header for story engine functions
using namespace std; // Use the standard namespace for convenience


// Shows the main menu
void showMainMenu() {
    cout << "==============================" << endl;             // Print game title banner
    cout << "||  CHRONICLES OF ARITHON  ||" << endl;
    cout << "==============================" << endl << endl;
    cout << "1. Login" << endl;                                   // Menu option: Login
    cout << "2. Create New Profile" << endl;                      // Menu option: Create new user
    cout << "3. Help" << endl;                                    // Menu option: Show help
    cout << "4. Exit" << endl;                                    // Menu option: Exit game
    cout << "------------------------------" << endl;             // Separator line
    cout << "Please select an option: ";
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
            if (exitRequest(user, 'Y')) { // Confirm exit
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
            if (exitRequest(user, 'Y')) { // Confirm exit
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

// Displays the help menu
void showHelp(bool fromMenu, string exeName)
{
    string fullPath(exeName);
    size_t pos = fullPath.find_last_of("\\/");
    string filename = ".\\" + ((pos != string::npos) ? fullPath.substr(pos + 1) : fullPath);
    system("cls");

    if(!fromMenu) { 
        cout << "========================================================================================================" << endl;
        cout << "============================= CHRONICLES OF ARITHON - HELP MENU ========================================" << endl;
        cout << "========================================================================================================" << endl;
        cout << "Welcome to Chronicles of Arithon! " << endl;
        cout << "This is a text-based interactive story game." << endl;
        cout << endl;
        cout << "Choices you make affect the story's outcome." << endl;
        cout << "Make decisions wisely - your story depends on them." << endl;
        cout << endl;
        cout << "Before playing, create a new user or log-in as an existing one." << endl;
        cout << "Use the main menu or the command line arguments to login/register" << endl;
        cout << endl;
        cout << "===== Using the Command Line ===== " << endl;
        cout << "" << endl;
        cout << "To create a new profile:" << endl;
        cout << filename << " -nuser <username> -pswd  <password> -cpswd <same password>" << endl;
        cout << "\nExample:" << endl;
        cout << filename << " -nuser arin -pswd eldora123!  -cpswd eldora123!" << endl;
        cout << "\nTo login with an existing profile:" << endl;
        cout << filename << " -euser <username> -pswd <password>" << endl;
        cout << "\nExample:" << endl;
        cout << filename << " -euser arin -pswd eldora123!" << endl;
        cout << "\nTo display help screen : " << endl;
        cout << filename << " -help" << endl;
        cout << "========================================================================================================" << endl;
    }
    //Wait for a key stroke if being called from the menu
    if (fromMenu) {

        cout << "========================================================================================================" << endl;
        cout << "============================= CHRONICLES OF ARITHON - HELP MENU ========================================" << endl;
        cout << "========================================================================================================" << endl;
        cout << "\nWelcome to Chronicles of Arithon! " << endl;
        cout << "\nThis is a text-based interactive story game." << endl;
        cout << "Choices you make affect the story's outcome." << endl;
        cout << "Make decisions wisely - your story depends on them." << endl;
        cout << endl;
        cout << "Before playing, create a new user or log-in as an existing one." << endl;
        cout << "Use the main menu or the command line arguments to login/register" << endl;
        cout << endl;
        cout << "===== Using the Main Menu ====="<< endl;
        cout << "" << endl;
        cout << "-> Option 1 : Log In as an existing user" << endl;
        cout << "-> Option 2 : Register as a new user" << endl;
        cout << "-> Option 3 : Display this help menu" << endl;
        cout << "-> Option 4 : Exit the game" << endl;
        cout << "\n" << endl;
        cout << "Once logged in, you can:" << endl;
        cout << "-> Continue your saved story" << endl;
        cout << "-> Start a new adventure" << endl;
        cout << "-> Delete your profile" << endl;
        cout << "-> Exit the game" << endl;
        cout << "========================================================================================================" << endl;
        cout << "\nPress any key to continue ... ";
        int ch = _getch();
        system("cls");
    }
}

// Process the CLA login process
bool handleCommandLineLogin(UserData& user, Authenticator& AuthInstance)
{
    if (!AuthInstance.processInputArguments()) {                                // Process the command-line arguments
        return false;
    }
    user.username = AuthInstance.getUsername();
    user.password = AuthInstance.getPassword();
    user.isNewUser = AuthInstance.isNewUserFlag();

    if (!AuthInstance.logIn(user)) {                         // If authentication or creation failed
        return false;
    }
    return true;
}

// Process the Interactive login process (use the menu)
bool handleInteractiveLogin(UserData& user, Authenticator& AuthInstance)
{
    int choice = 0;                                                     // Store user's menu choice
    char confirm;                                                       // Store confirmation input
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

            AuthInstance.setInputVariables(user.username, user.password, user.password, user.isNewUser, false);
            success = AuthInstance.logIn(user); // Attempt to log in using Authenticator

            if (success) {                              // If login succeeded
                return true;                            // Get out of the loop and return true
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

            //Set the internal data for the auth object
            AuthInstance.setInputVariables(user.username, user.password, confirm, user.isNewUser, false);
            
            // Attempt to create a new user using Authenticator
            // Using Authenticator login function to create a new user
            success = AuthInstance.logIn(user); // Attempt to log in with new user credentials
            if (success) {
                return true;
            }
        }
        else if (choice == 3) {
            showHelp(true, (AuthInstance.getInputArguments())[0]);                                      // If user chooses to see help
        }
        else if (choice == 4) {                                                 // If user chooses to exit
            cout << endl;
            cout << "Are you sure you want to exit? (y/n): ";                   // Prompt for confirmation
            cin >> confirm;                                                     // Read confirmation

            if (toupper(confirm) == 'Y') {
                return false;
            }
        }
        else {                                                                 // If input is invalid
            cout << "\nInvalid input! Try again." << endl;                     // Inform user
        }
    }
    return true;
}
