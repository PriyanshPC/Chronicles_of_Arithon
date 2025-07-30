// Chronicles_of_ Arithon.cpp : This file contains the 'main' function. Program execution begins and ends there.
#include <iostream>
#include "FileManager.h"
#include "Authenticator.h"
using namespace std;

/**-----------------------------------------------------------------------------------------------------
* Main function to demonstrate the usage of the Authenticator class
* The fake File IO function loads these fictitious user credentials
* use any of these combinations to log in
* 
* alice			password123
* bob			password456
* joy			Maple234
* Robin123		flower235
* skinchild		bobcatOzi
* furbaby		password254
* 
* Use Ctrl+Shift+B to build the project and run the executable with command line arguments
* Example: Chronicles_of_Arithon.exe -u alice -p password123
* 
* if you want to add a new user
* Example: Chronicles_of_Arithon.exe -u newUsername -p newPassword -new
* 
* if you want to see the help message that explains the command line options
* Example: Chronicles_of_Arithon.exe -help
-----------------------------------------------------------------------------------------------------*/

int main(int argCount, char** argVector)
{
    //FileSystem file;
    //file.loadFile();

    //playerStats player;
    //file.saveFile(player);
	
	// Create an instance of FileSystem to read/write user credentials
	// If the author of FileSystem module decides to change the FileSystem class 
	// We have to change it here
	FileSystem fileIOInstance;

	// Create an instance of the Authorization class:
	// The constructor takes in the FileSystem instance to access user credentials
	// If the author of FileSystem module decides to change the FileSystem class 
	// We have to change it here
	Authenticator AuthInstant(fileIOInstance, argCount, argVector);

	if (AuthInstant.isDebugMode()) {
		AuthInstant.isLoggedIn() ?
			cout << "Logged in user = " << AuthInstant.getLoggedInUser() << endl :
			cout << "No one is logged in" << endl;
	}
	
	return 0;
}
