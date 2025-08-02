#pragma once
#define AUTHENTICATOR_H

#include <string>
class PlayerState;

struct UserData {
    std::string username;
    std::string password;
    PlayerState* playerState;
    bool isNewUser;
};

void showHelp();
bool processArguments(int argc, char* argv[], UserData& user);
bool createUser(UserData& user);
bool loginUser(UserData& user);
std::string encrypt(const std::string& text);
bool isValidInput(const std::string& username, const std::string& password, const std::string& confirmPass = "");
void showGameWindow(const UserData& user);
bool handleMenuChoice(int choice, UserData& user);
bool deleteConfirmation();
