#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "FileManager.h"

using namespace std;

// ------------------------------------ Start of the section added for Authenticator Module -------------------------
// The Authorization module will work perfectly if the interface is followed.
// The data member: `credentialList` is used to simulate the credential table in memory.
// The methods `readCredentialTable` and `addCredential` are used to read and write user credentials respectively.
// The function prototypes are defined in the header file `FileManager.h`.
// Please ensure that the interface remains unchanged for compatibility with the Authorization class.

void FileSystem::fakeFileIO() {
	// This is just a mock implementation to simulate the credential table.
	// It can be used for testing purposes without actual file operations.
	credentialList.push_back({ "alice", "ef92b778bafe771e89245b89ecbc08a44a4e166c06659911881f383d4473e94f" });      //password123
	credentialList.push_back({ "bob", "c6ba91b90d922e159893f46c387e5dc1b3dc5c101a5a4522f03b987177a24a91" });        //password456
	credentialList.push_back({ "joy", "065433f78cb6e524d1291a9abe4604abe2b5e75ad7ac45c92d1f072694420b6c" });        //Maple234
	credentialList.push_back({ "Robin123", "b492a99c33869cda86f9c40bddb36e527959c202528689a6584774ba2b88f7cd" });   //flower235
	credentialList.push_back({ "skinchild", "20924ecc7b592f022c3c8febf7787f22da65c533b5cc209e5aa89c9e2e379a6b" });  //bobcatOzi
	credentialList.push_back({ "furbaby", "b16c276f602722f88c2f95a6858f976db90dc3e717e9272a74d925c3009629ab" });    //password254
	cout << "[FileSystem] Loaded mock credential list." << endl;
}

// This function returns the credential table to the caller
// This can be left as it is for actual implementation
vector<User> FileSystem::readCredentialTable() {
    cout << "[FileSystem] readCredentialTable() called." << endl;
    return credentialList;
}


// This function adds a new credential to the credential table
// For the actual implementation, additional code is needed to properly store the updated user information to appropriate files
bool FileSystem::addCredential(const std::string& username, const std::string& hashedPassword) {
    credentialList.push_back({ username, hashedPassword });

    // Simulated writing to a file by printing to console
    cout << "[FileSystem] wrote the info to the file: " << username << " -> " << hashedPassword << endl;

    // For actual implementation additional code is needed here to write to a file.
    // Return true to indicate success
    return true;
}
// ------------------------------------ End of the section added for Authenticator Module -------------------------


FileSystem::FileSystem() : fileName(FILENAME) {
	// Replace this section with actual file reading logic when implemented.
	// The actual implementation would read from a file. Using code that are yet to be implemented in the FILEIO class.
	fakeFileIO(); // Load mock data for testing purposes
}

void FileSystem::loadFile() {
	if (!checkForFile()) { // If file does not exist
		ofstream file(fileName); // Create File
		file.close(); // Close file
		cout << "File " << fileName << " Created" << endl; // Print status
	}
	else { // If file does exist
		cout << "File " << fileName << " Found" << endl; // Print status
	}
}

bool FileSystem::checkForFile() { // Check if file exists
	ofstream file(fileName, ios::in); // opens for reading
	file.close(); // Close file
	return file.good(); // returns true if file exists, false if not
}

void FileSystem::printToFile(string printThis, fileLine fLine) {
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

void FileSystem::saveFile(playerStats player) {
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

void FileSystem::readFromFile() {
	if (!checkForFile()) {
		perror("ERROR: File could not be found...\n");
	}
	else {
		ofstream file(fileName, ios::in); // open file with read (do not overwrite);
		file.close(); // Close file
	}
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

