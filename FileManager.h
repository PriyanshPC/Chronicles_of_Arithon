#pragma once // Ensures the header is included only once during compilation
#include <string> // Provides the std::string class
#include <filesystem>
#define FILEMANAGER_H // Macro guard for additional include protection
struct UserData; // Forward declaration of UserData structure

// Saves the user data to persistent storage
bool saveUser(const UserData& user);

// Loads user data for the given username into the provided UserData structure
bool loadUser(const std::string& username, UserData& user);

// Checks if a user with the given username exists
bool userExists(const std::string& username);

// Deletes the user data for the given username
bool deleteUser(const std::string& username);

std::filesystem::path getExecutableDirectory();