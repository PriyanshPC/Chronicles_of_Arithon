#pragma once                            // Ensures the header is included only once during compilation
#include <string>                       // Provides the std::string class
#include "PlayerState.h"                // Header for player state management
// Structure to hold user-related data
struct UserData {
    std::string username;      // Stores the user's username
    std::string password;      // Stores the user's password
    PlayerState* playerState;  // Pointer to the player's state object
    bool isNewUser = false;            // Indicates if the user is new
};

// Displays help information to the user
void showHelp();

// Displays the main game window for the authenticated user
void showGameWindow(const UserData& user);

// Handles the user's menu choice and updates UserData as needed
bool handleMenuChoice(int choice, UserData& user);

// Prompts the user for confirmation before deleting data
bool deleteConfirmation();

void showMainMenu();    