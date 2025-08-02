#pragma once

#define FILEMANAGER_H

#include <string>

struct UserData;

bool saveUser(const UserData& user);
bool loadUser(const std::string& username, UserData& user);
bool userExists(const std::string& username);
bool deleteUser(const std::string& username);
