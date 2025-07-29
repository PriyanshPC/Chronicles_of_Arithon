#include <iostream>
#include <fstream>
#include <string>
#include "Authenticator.h"

#define FILENAME "filesave.dat"

using namespace std;

class playerStats {
private:
	string userName;
	string password;
	string storyLocation;
	string item;

public:
	playerStats() : userName("Player Name"), password("PlayerPassword"), storyLocation("Beginning"), item("None") {}

	string getName() {
		return userName;
	}

	string getPass() {
		return password;
	}

	string getLocation() {
		return storyLocation;
	}

	string getItem() {
		return item;
	}
};

enum fileLine {
	userNameFileLine = 1,
	passwordFileLine,
	itemFileLine,
	locationFileLine
};

string getUsername(playerStats player);
string getPassword(playerStats player);
string getStoryLocation(playerStats player);
string getItem(playerStats player);


class FileSystem {
private:
	string fileName;

public:
	FileSystem() : fileName(FILENAME) {}

	void loadFile() {
		if (!checkForFile()) { // If file does not exist
			ofstream file(fileName); // Create File
			file.close(); // Close file
			cout << "File " << fileName << " Created" << endl; // Print status
		}
		else { // If file does exist
			cout << "File " << fileName << " Found" << endl; // Print status
		}
	}

	bool checkForFile() { // Check if file exists
		ofstream file(fileName, ios::in); // opens for reading
		file.close(); // Close file
		return file.good(); // returns true if file exists, false if not
	}

	void printToFile(string printThis, fileLine fLine) {
		if (!checkForFile()) {
			perror("ERROR: File could not be found...\n");
		}
		else {
			ofstream file(fileName, ios::app); // open file with append (do not overwrite);
			file << printThis;
			file.close(); // Close file
			cout << "File Updated" << endl;
		}
	}

	void saveFile(playerStats player) {
		if (!checkForFile()) {
			perror("File could not be found... Creating new File\n");
		}
		else {
			ofstream file(fileName); // open file with append (do not overwrite);
			file << getUsername(player) << endl;
			file << getPassword(player) << endl;
			file << getStoryLocation(player) << endl;
			file << getItem(player) << endl;
			file.close(); // Close file
			cout << "File Updated" << endl;
		}


	}

	void readFromFile() {
		if (!checkForFile()) {
			perror("ERROR: File could not be found...\n");
		}
		else {
			ofstream file(fileName, ios::in); // open file with read (do not overwrite);
			file.close(); // Close file
		}
	}

	string readUsername() {
		string username;
		ifstream file(FILENAME);
		if (file.is_open()) {
			getline(file, username);
			file.close();
		}
		else {
			perror("ERROR: File could not be found...\n");
		}
		return username;
	}
		
	string readPassword() {
		string password;
		ifstream file(FILENAME);
		if (file.is_open()) {
			getline(file, password);
			file.close();
		}
		else {
			perror("ERROR: File could not be found...\n");
		}
		return password;
	}

	user readCredentials() {
		user usr(readUsername(), readPassword());
		return usr;
	}
};