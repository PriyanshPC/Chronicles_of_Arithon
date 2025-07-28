#include <iostream>
#include <fstream>
#include <vector>
#include <string>

#define FILENAME "filesave.dat"
#define FILENAME "filesave.dat"

using namespace std;

// ------------------------------------ Start of the section added for Authenticator Module -------------------------
// This structure is used to represent a user credential.
// This interface is a requirement from the Authorization class and can't be changed.
typedef struct User {
	string username;
	string encryptedPassword;
}User;

// This class is used to test integration with the authenticator module
// The requirement here is to have the member function behave the same way
// The developer of the FILE Manager module can decide to move these methods 
// into a different class for their convenience

class FileIO {
private:
	// This vector simulates the credential table in memory. 
	// This interface is a requirement from Authorization class.
	vector<User> credentialList;

public:
	FileIO();

	// Returns a vector of User structs
	// This function definiton is used to read the credential table from the file. 
	// This interface is a requirement from Authorization class and can't be changed 
	vector<User> readCredentialTable();

	// This function definiton is used to add a new credential to the file. 
	// This interface is a requirement from Authorization class and can't be changed 
	bool addCredential(const string& username, const string& hashedPassword);
};

// ------------------------------------ End of the section added for Authenticator Module -------------------------


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
};