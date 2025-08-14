#include "MenuController.h"                                                 // Include custom authentication header
#include "Authenticator.h"                                                  // Include custom authenticator header
using namespace std; // Use the standard namespace for convenience

int main(int argCount, char** argVector) {                                  // Main entry point; argCount/argVector for command-line arguments

    UserData user;                                                          // Create a UserData object to store user info
    user.playerState = new PlayerState();                                   // Dynamically allocate PlayerState for the user
    Authenticator AuthInstance(argCount, argVector);                        // Initialize Authenticator module

    bool success = (argCount > 1)                                           // If command-line arguments are provided
        ? handleCommandLineLogin(user, AuthInstance)                        // Attempt command-line login or user creation
        : handleInteractiveLogin(user, AuthInstance);                       // Otherwise, handle interactive login

    if (success) {                                                          // If login or registration is successful
        showGameWindow(user);                                               // Launch game window
    }

    delete user.playerState;                                                // Clean up allocated PlayerState memory
    return 0;                                                               // Exit program successfully
}
