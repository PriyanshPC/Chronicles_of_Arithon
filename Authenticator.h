#pragma once // Ensures the header is included only once during compilation

#define AUTHENTICATOR_H // Macro guard for additional include protection

#include <string> // Provides the std::string class

class PlayerState; // Forward declaration of PlayerState class

// Structure to hold user-related data
struct UserData {
    std::string username;      // Stores the user's username
    std::string password;      // Stores the user's password
    PlayerState* playerState;  // Pointer to the player's state object
    bool isNewUser;            // Indicates if the user is new
};

// Displays help information to the user
void showHelp();

// Processes command-line arguments and populates the UserData structure
bool processArguments(int argc, char* argv[], UserData& user);

// Creates a new user account using the provided UserData
bool createUser(UserData& user);

// Authenticates an existing user using the provided UserData
bool loginUser(UserData& user);

// Encrypts the given text and returns the encrypted string
std::string encrypt(const std::string& text);

// Validates the username and password (and optionally confirmation password)
bool isValidInput(const std::string& username, const std::string& password, const std::string& confirmPass = "");

// Displays the main game window for the authenticated user
void showGameWindow(const UserData& user);

// Handles the user's menu choice and updates UserData as needed
bool handleMenuChoice(int choice, UserData& user);

// Prompts the user for confirmation before deleting data
bool deleteConfirmation();
