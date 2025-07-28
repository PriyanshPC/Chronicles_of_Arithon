// Chronicles_of_ Arithon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "FileManager.h"
#include "Authenticator.h"

using namespace std;

int main(int argCount, char** argVector)
{
    //FileSystem file;
    //file.loadFile();

    //playerStats player;
    //file.saveFile(player);
	
	// Create an instance of FileIO to read/write user credentials
	// If the author of FileIO module decides to change the FileIO class 
	// We have to change it here
	FileIO fileIOInstance;

	// Create an instance of the Authorization class:
	// The constructor takes in the FIleIO instance to access user credentials
	// If the author of FileIO module decides to change the FileIO class 
	// We have to change it here
	Authenticator AuthInstant(fileIOInstance, argCount, argVector);

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file