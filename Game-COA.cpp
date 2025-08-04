#include <iostream>
#include "Authorization.h"
#include "FileIO.h"

using namespace std;

int main(int argCount, char** argVector)
{
	// Create an instance of FileIO to read/write user credentials
	FileIO fileIOInstance;

	// Create an instance of the Authorization class:
	// The constructor takes in the FIleIO instance to access user credentials
	Authorization AuthInstant(fileIOInstance,argCount, argVector);

	return 0;
}