#include <iostream>
#include <fstream>
#include <string>
#include "Authenticator.h"

#define FILENAME "filesave.dat"

using namespace std;


enum fileLine {
	userNameFileLine = 1,
	passwordFileLine,
	itemFileLine,
	locationFileLine
};

class playerStats {
private:
	string storyLocation;
	string item;

public:
	user usr;

	playerStats() :storyLocation("Beginning"), item("None") {
		usr.setPassword("password");
		usr.setUsername("username");
	}

	string getName() {
		return usr.getUsername();
	}

	string getPass() {
		return usr.getPassword();
	}

	bool checkPass(string temp) {
		return (temp == usr.getPassword());
	}

	string getLocation() {
		return storyLocation;
	}

	string getItem() {
		return item;
	}

	void setLocation(string location) {
		storyLocation = location;
	}

	void setItem(string item) {
		this->item = item;
	}
};


void newUser(playerStats& player);

string getUsername(playerStats player);
string getPassword(playerStats player);
string getStoryLocation(playerStats player);
string getItem(playerStats player);

class FileSystem {
private:
	string fileName;

public:
	FileSystem() : fileName(FILENAME) {}

	void loadFile(playerStats& player) {
		if (!checkForFile()) { // If file does not exist
			ofstream file(fileName); // Create File
			file.close(); // Close file
			cout << "File '" << fileName << "' Created" << endl; // Print status
		}
		else { // If file does exist
			cout << "File '" << fileName << "' Found" << endl; // Print status

			player.usr.setUsername(readUsername());
			player.usr.setPassword(readPassword());
			player.setItem(readItem());
			player.setLocation(readLocation());
		}
	}

	bool checkForFile() { // Check if file exists
		ifstream file(fileName); // opens for reading

		if (file.is_open()) {
			file.close();
			return true;
		}
		else {
			return false;
		}
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
		ofstream file(fileName); // open file with append (do not overwrite);
		file << getUsername(player) << endl; // line 1
		file << getPassword(player) << endl; // line 2
		file << getStoryLocation(player) << endl; // line 3
		file << getItem(player) << endl; // line 4
		file.close(); // Close file
		cout << "File Updated" << endl;
	}

	void readFromFile() {
		if (!checkForFile()) {
			perror("ERROR: File could not be found...\n");
		}
		else {
			ifstream file(fileName); // open file with read (do not overwrite);
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
			getline(file, password);
			file.close();
		}
		else {
			perror("ERROR: File could not be found...\n");
		}
		return password;
	}

	string readItem() {
		string item;
		ifstream file(FILENAME);
		if (file.is_open()) {
			getline(file, item);
			getline(file, item);
			getline(file, item);
			file.close();
		}
		else {
			perror("ERROR: File could not be found...\n");
		}
		return item;
	}

	string readLocation() {
		string location;
		ifstream file(FILENAME);
		if (file.is_open()) {
			getline(file, location);
			getline(file, location);
			getline(file, location);
			getline(file, location);
			file.close();
		}
		else {
			perror("ERROR: File could not be found...\n");
		}
		return location;
	}


	bool checkPassword(string temp) {
		return temp == readPassword();
	}

	user readCredentials() {
		user usr(readUsername(), readPassword());
		return usr;
	}
};
