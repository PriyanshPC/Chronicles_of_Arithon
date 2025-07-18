#include <iostream>
#include <fstream>
#include "FileManager.h"

using namespace std;


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
