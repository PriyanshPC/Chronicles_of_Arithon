#include <iostream>
#include <fstream>
#include "FileManager.h"

using namespace std;


void newUser(playerStats& player, char** argv) {

	player.usr.setUsername(argv[0]);
	player.usr.setNewPassword(argv[1]);
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