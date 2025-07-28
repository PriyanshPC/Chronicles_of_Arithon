#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include "FileManager.h"

#define MAX_USERNAME_LENGTH 20
#define MIN_USERNAME_LENGTH 3
#define MIN_PASSWORD_LENGTH 6

using namespace std;	

class Authenticator
{
	string loggedInUserName;
	string inputUserName;
	string inputPassword;
	bool isNewUser;
	bool DebugMode;

	vector<string> inputArgs;
	unordered_map<string, string> userCredentials; 

	// Function to compute SHA256 hash of a string
	string sha256(const string& input);

	// Function to check if username and password are of proper format
	bool isValidUsername(const string& username);
	bool isValidPassword(const string& password);

	// Function to check if a particular user exists in the current registration list
	bool userExists(const string& username);
	
	// Function to display Input Argument summary
	void displayInputArguments();

	// Function to identify username, password from the command line arguments 
	// This also identifies if the -newuser flag is set and if debugmode is requested
	void processInputArguments(int argCount, char** argVector);

	void loadUserCredentials(FileIO& fileIO);
	bool validateCredentials(const string& username, const string& password);
	bool logIn(FileIO& fileIO);
	bool createNewUser();
	void storeNewUser(FileIO& fileIO);
	
public:
	Authenticator(FileIO& fileIO, int& argCount, char** argVector);
	~Authenticator();
	
	void toggleDebugMode();
	bool isDebugMode() const;
	bool isLoggedIn() const;
	string encryptData(const string& inputString);
	string getLoggedInUser() const;
	vector<string> getInputArguments() const;
};
