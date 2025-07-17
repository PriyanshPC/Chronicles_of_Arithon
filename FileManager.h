#include <iostream>
#include <fstream>

#define FILENAME "filesave.dat"
#define TEMPFILENAME "filesave.dat"

enum fileLine{
	userNameFileLine = 1,
	passwordFileLine,
	itemFileLine,
	locationFileLine
};

using namespace std;

class FileSystem {
private:
	string fileName;

public:
	FileSystem() : fileName(FILENAME) {}

	void fileSystemStart() {
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
		return file.good(); // returns true if file exists, false if not
	}

	void printToFile(string printThis, fileLine fLine) {
		if (!checkForFile()) {
			perror("ERROR: File could not be found...\n");
		}
		else {
			ofstream file(fileName, ios::app); // open file with append (do not overwrite);
			file << printThis;
			cout << "File Updated" << endl;
		}
	}

	void readToFile() {
		if (!checkForFile()) {
			perror("ERROR: File could not be found...\n");
		}
		else {
			ofstream file(fileName, ios::in); // open file with read (do not overwrite);
		}
	}
};