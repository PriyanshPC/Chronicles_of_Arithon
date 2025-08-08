#include <iostream> // For input/output stream operations
#include <string>   // For using std::string
#include <cstdlib>  // For system() and exit()
#include "Authenticator.h" // Header for authentication-related declarations
#include "FileManager.h"   // Header for file management functions
#include "PlayerState.h"   // Header for player state management
#include "StoryEngine.h"   // Header for story engine functions

using namespace std; // Use the standard namespace

// Displays the help menu to the user
void showHelp() {
    cout << "\n=== CHRONICLES OF ARITHON - HELP MENU ===\n\n";
    cout << "This is a text-based interactive story game. Choices you make affect the story's outcome.\n";
    cout << "Before playing, you must either create a new user profile or log in to an existing one.\n\n";

    cout << "=== CREATE A NEW USER ===\n";
    cout << "Use this format to create a new profile:\n";
    cout << "  .\\TeamsProject.exe \"nuser-<username>-pswd-<password>-cpswd-<confirmPassword>\"\n";
    cout << "Example:\n";
    cout << "  .\\TeamsProject.exe \"nuser-arin-pswd-eldora123-cpswd-eldora123\"\n\n";

    cout << "=== LOGIN AN EXISTING USER ===\n";
    cout << "Use this format to login with an existing profile:\n";
    cout << "  .\\TeamsProject.exe \"euser-<username>-pswd-<password>\"\n";
    cout << "Example:\n";
    cout << "  .\\TeamsProject.exe \"euser-arin-pswd-eldora123\"\n\n";

    cout << "=== GET HELP ===\n";
    cout << "To show this help menu at any time:\n";
    cout << "  .\\TeamsProject.exe -info\n";

    cout << "\nOnce logged in, you can:\n";
    cout << "  -> Continue your saved story\n";
    cout << "  -> Start a new adventure\n";
    cout << "  -> Delete your profile\n";
    cout << "  -> Exit the game\n";

    cout << "\nMake decisions wisely — your story depends on them.\n";
    cout << "=============================================\n";
}

// Processes command-line arguments and populates the user data structure
bool processArguments(int argc, char* argv[], UserData& user) {
    user.playerState = new PlayerState();  // Allocate memory for player state

    if (argc <= 1) { // No command-line arguments provided
        return false; // Handled in main() fallback
    }

    string input = argv[1]; // Get the first argument

    if (input == "-info") { // Show help menu if requested
        showHelp();
        return false;
    }

    // Handle new user creation
    if (input.find("nuser-") == 0) {
        user.isNewUser = true; // Mark as new user
        int userStart = 6; // Start index after "nuser-"
        int pswdPos = input.find("-pswd-"); // Find password delimiter
        int cpswdPos = input.find("-cpswd-"); // Find confirm password delimiter

        // Check for correct format
        if (pswdPos == string::npos || cpswdPos == string::npos) {
            cout << "Error: Wrong format! Use -init for help.\n";
            return false;
        }

        // Extract username, password, and confirm password from input
        user.username = input.substr(userStart, pswdPos - userStart);
        string password = input.substr(pswdPos + 6, cpswdPos - pswdPos - 6);
        string confirmPass = input.substr(cpswdPos + 7);

        // Validate input fields
        if (!isValidInput(user.username, password, confirmPass)) return false;

        user.password = password; // Set password
        return true;
    }

    // Handle existing user login
    if (input.find("euser-") == 0) {
        user.isNewUser = false; // Mark as existing user

        int userStart = 6; // Start index after "euser-"
        int pswdPos = input.find("-pswd-"); // Find password delimiter
        if (pswdPos == string::npos) {
            cout << "Error: Wrong format! Use -init for help.\n";
            return false;
        }

        // Extract username and password from input
        user.username = input.substr(userStart, pswdPos - userStart);
        user.password = input.substr(pswdPos + 6);

        // Validate input fields
        if (!isValidInput(user.username, user.password)) return false;
        return true;
    }

    // Unknown command
    cout << "Error: Unknown command! Use -init for help.\n";
    return false;
}

// Validates username, password, and (optionally) confirm password
bool isValidInput(const string& username, const string& password, const string& confirmPass) {
    if (username.empty()) { // Username must not be empty
        cout << "Error: Username cannot be empty!\n";
        return false;
    }
    if (password.length() < 4) { // Password must be at least 4 characters
        cout << "Error: Password must be at least 4 characters!\n";
        return false;
    }
    if (!confirmPass.empty() && password != confirmPass) { // Passwords must match if confirmPass is provided
        cout << "Error: Passwords don't match!\n";
        return false;
    }
    return true; // All checks passed
}

// Encrypts the given text using a simple transformation
string encrypt(const string& text) {
    unsigned int hash = 0x811C9DC5; // FNV offset basis (not used for actual encryption here)
    for (char c : text) {
        hash ^= static_cast<unsigned char>(c); // XOR character with hash
        hash *= 0x01000193; // FNV prime multiplication
    }
    string encrypted = text; // Copy original text
    for (char& c : encrypted) c += 3; // Shift each character by 3 (Caesar cipher)
    return encrypted; // Return encrypted string
}

// Creates a new user profile and saves it
bool createUser(UserData& user) {
    if (userExists(user.username)) { // Check if user already exists
        cout << "Error: User '" << user.username << "' already exists!\n";
        return false;
    }

    user.password = encrypt(user.password); // Encrypt the password
    user.playerState->resetToDefault(); // Reset player state to default

    if (saveUser(user)) { // Save user data to file
        return true;
    }
    else {
        cout << " Failed to save user to file!\n";
        return false;
    }
}

// Logs in an existing user by verifying credentials
bool loginUser(UserData& user) {
    if (!userExists(user.username)) { // Check if user exists
        cout << "Error: User '" << user.username << "' not found!\n";
        return false;
    }

    UserData storedUser; // Structure to hold loaded user data
    storedUser.playerState = new PlayerState(); // Allocate memory for player state

    if (!loadUser(user.username, storedUser)) { // Load user data from file
        cout << "Error: Failed to load user data!\n";
        return false;
    }

    if (storedUser.password != encrypt(user.password)) { // Compare encrypted passwords
        cout << "Error: Wrong password!\n";
        return false;
    }

    *user.playerState = *storedUser.playerState; // Copy player state
    user.password = storedUser.password; // Set encrypted password
    return true;
}

// Displays the main game window and menu
void showGameWindow(const UserData& user) {
    system("cls"); // Clear the console screen

    cout << "========================================\n";
    cout << "||  WELCOME TO CHRONICLES OF ARITHON  ||\n";
    cout << "========================================\n\n";

    if (user.isNewUser) { // Greet new user
        cout << "Welcome, " << user.username << "!\nYour adventure begins here!\n";
    }
    else { // Greet returning user
        cout << "Welcome back, " << user.username << "!\n";
        cout << "\n";
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
