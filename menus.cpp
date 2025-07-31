#include <iostream>
#include "menus.h"
#include "globals.h"
#include "FileManager.h"
#include "battle.h"


using namespace std;

void intro() {
	cout << "*** Welcome to Chronicles of Arithon ***" << endl;
	Arithon::line();
	Arithon::loading(SLEEP_ONE);
	Arithon::lineBreak(TWO);
}
void chroniclesOfArithon(int argc, char** argv) {
	intro();

	if (argc < 2) {
		cout << "ERROR: NOT ENOUGH ARGUMENTS" << endl;
		exit(EXIT_FAILURE);
	}

	FileSystem file(argv[0]);
	playerStats player;

	if (!file.checkForFile()) {
		cout << "There are no save files, starting a new game." << endl;
		file.loadFile(player);
		newUser(player, argv);
		file.saveFile(player);
	}

	else {
		cout << "Found Player save: " << file.readUsername() << endl;
		cout << "Would you like to use this save file or start new game?" << endl;
		cout << "(starting a new game will overwrite your existing save file)\n" << endl;

		cout << "1. Use existing save\n2. Create New\n3. Exit\n\nChoose: ";
		
		int choice;
		cin >> choice;

		switch (choice) {
		case 1:
			Arithon::lineBreak(TWO);
			
			if(file.checkPassword(argv[1])) {
				file.loadFile(player);
			}
			else {
				perror("ERROR: Password Incorrect... Exiting.");
				Arithon::sleep(SLEEP_THREE);
				exit(EXIT_FAILURE);
			}
			
			break;

		case 2:
			newUser(player, argv);
			file.saveFile(player);
			break;
		case 3:
			exit(EXIT_SUCCESS);

		default:
			perror("ERROR: Invalid Entry... Exiting.");
			Arithon::sleep(SLEEP_THREE);
			exit(EXIT_FAILURE);
		}

	}

	cout << "Hi, " << player.getName() << endl;
	Arithon::line();

	startBattle();

	file.saveFile(player);
}

