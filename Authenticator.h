#pragma once

#include <iostream>

using namespace std;

class user {
private:
	string username;
	string encryptedPassword;

public:
	user() : username(""), encryptedPassword("") {}
	user(string name, string pass) : username(name), encryptedPassword(pass) {}

	string getUsername() {
		return username;
	}

	string getPassword() {
		return encryptedPassword;
	}

	void setUsername(string username) {
		this->username = username;
	}

	void setPassword(string password) {
		this->encryptedPassword = password;
	}
};