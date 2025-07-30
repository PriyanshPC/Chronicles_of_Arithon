#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define FILENAME "filesave.dat"
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

enum fileLine{
	userNameFileLine = 1,
	passwordFileLine,
	itemFileLine,
	locationFileLine
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

	// Requirements from Authenticator module
	// This vector simulates the credential table in memory. 
	// This interface is a requirement from Authenticator class.
	vector<User> credentialList;

public:
	// -------- Methods required by Auth module----------------------------------------
	// Returns a vector of User structs
	// This function definition is used to read the credential table from the file. 
	// This interface is a requirement from Authenticator class and can't be changed 
	vector<User> readCredentialTable();

	// This function definition is used to add a new credential to the file. 
	// This interface is a requirement from Authenticator class and can't be changed 
	bool addCredential(const string& username, const string& hashedPassword);
	// --------------------------------------------------------------------------------

	FileSystem();
	void fakeFileIO();
	void loadFile();
	bool checkForFile();
	void printToFile(string printThis, fileLine fLine);
	void saveFile(playerStats player);
	void readFromFile();
};