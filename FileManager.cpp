#include <iostream>
#include <fstream>
#include "FileManager.h"

using namespace std;


void newUser(playerStats& player) {
	string username, password;

	cout << "Enter Username: ";
	cin >> username;

	cout << "\nEnter Password: ";
	cin >> password;

	player.usr.setUsername(username);
	player.usr.setNewPassword(password);
	player.setItem("None");
	player.setLocation("Beginning");
}

string getUsername(playerStats player) {
	return player.getName();
}

string getPassword(playerStats player) {
	return player.getPass();
}

string getStoryLocation(playerStats player) {
	return player.getLocation();
}

string getItem(playerStats player) {
	return player.getItem();
}