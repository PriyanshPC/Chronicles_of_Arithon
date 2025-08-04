#pragma once
#include <iostream>
#include <fstream>
#include <vector>
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
	user usr;
	string storyLocation;
	string item;

public:
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
};

string getUsername(playerStats player);
string getPassword(playerStats player);
string getStoryLocation(playerStats player);
string getItem(playerStats player);

// This structure is used to represent a user credential.
// This interface is a requirement from the Authenticator class and can't be changed.
typedef struct User {
	string username;
	string encryptedPassword;
}User;

class FileSystem {
private:
	string fileName;

	// -------------------------- For Authenticator module ----------------------------
	// This vector simulates the credential table in memory. 
	// This interface is a requirement from Authenticator class.
	vector<User> credentialList;

public:
	// Returns a vector of User structs
	// This function definition is used to read the credential table from the file. 
	// This interface is a requirement from Authenticator class and can't be changed 
	vector<User> readCredentialTable();

	// This function definition is used to add a new credential to the file. 
	// This interface is a requirement from Authenticator class and can't be changed 
	bool addCredential(const string& username, const string& hashedPassword);
	// --------------------------------------------------------------------------------

	FileSystem();
	void fakeFileIO(); // For testing purposes only
	
	void loadFile();
	bool checkForFile();
	void printToFile(string printThis, fileLine fLine);
	
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


	bool checkPassword(string temp) {
		return temp == readPassword();
	}

	user readCredentials() {
		user usr(readUsername(), readPassword());
		return usr;
	}
};

