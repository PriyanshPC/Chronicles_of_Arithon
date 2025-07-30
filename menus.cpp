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
		newUser(player);
		file.saveFile(player);
	}

	else {
		cout << "Found Player save: " << file.readUsername();
		cout << ", would you like to use this save file or start new game?" << endl;
		cout << "(starting a new game will overwrite your existing save file)\n" << endl;

		cout << "1. Use existing save\n2. Create New\n\nChoose: ";
		int choice;

		cin >> choice;
		string pass;

		switch (choice) {
		case 1:
			Arithon::lineBreak(TWO);
			
			cout << "Enter Your Password: ";
			cin >> pass;
			if(file.checkPassword(pass)){
				file.loadFile(player);
			}
			else {
				perror("ERROR: Password Incorrect... Exiting.");
				Arithon::sleep(SLEEP_THREE);
				exit(EXIT_FAILURE);
			}
			
			break;

		case 2:
			newUser(player);
			file.saveFile(player);
			break;

		default:
			perror("ERROR: Invalid Entry... Exiting.");
			Arithon::sleep(SLEEP_THREE);
			exit(EXIT_FAILURE);
		}

		cout << "Hi, " << player.getName() << endl;
		cout << "Game Should Start Here" << endl;
	
		startBattle();
	}

	
}
