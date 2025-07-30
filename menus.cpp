#include <iostream>
#include "menus.h"
#include "globals.h"
#include "FileManager.h"
#include "battle.h"


using namespace std;

void chroniclesOfArithon() {
	cout << "*** Welcome to Chronicles of Arithon ***" << endl;
	Arithon::line();
	Arithon::loading();
	Arithon::lineBreak(THREE);

	FileSystem file;
	playerStats player;


	if (!file.checkForFile()) {
		cout << "There are no save files, starting a new game." << endl;
		file.loadFile(player);
		string username, password;

		cout << "Enter Username: ";
		cin >> username;

		cout << "\nEnter Password: ";
		cin >> password;

		player.usr.setUsername(username);
		player.usr.setPassword(password);
		player.setItem("None");
		player.setLocation("Beginning");
		file.saveFile(player);
	}

	else {
		file.loadFile(player);
		cout << "Found Player save: " << player.getName();
	}

	
}
