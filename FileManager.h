#include <iostream>
#include <fstream>

#define FILENAME "filesave.dat"
#define FILENAME "filesave.dat"

enum fileLine{
	userNameFileLine = 1,
	passwordFileLine,
	itemFileLine,
	locationFileLine
};

using namespace std;

string getUsername(/*Player.userName*/);
string getPassword(/*Player.password*/);
string getStoryLocation(/*Player.storyLocation*/);
string getItem(/*Player.Item*/);


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

	void saveFile(/*Player.player*/) {
		if (!checkForFile()) {
			perror("ERROR: File could not be found...\n");
		}
		else {
			ofstream file(fileName, ios::app); // open file with append (do not overwrite);
			file << getUsername();
			file << getPassword();
			file << getStoryLocation();
			file << getItem();
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