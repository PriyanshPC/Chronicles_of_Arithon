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
	bool debugMode;

	vector<string> inputArgs;		// to store commandline input arguments
	unordered_map<string, string> userCredentials; // this is a hash table, used to store username & password. for faster lookup

	// Function to compute SHA256 hash of a string
	string sha256(const string& input);

	// Function to check if username and password are of proper format
	bool isValidUsername(const string& username);
	bool isValidPassword(const string& password);

	// Function to check if a particular user exists in the current registration list. Registration cannot be a single entry
	bool userExists(const string& username);
	
	// Function to display Input Argument summary
	void displayInputArguments();

	// Function to identify username, password from the command line arguments 
	// This also identifies if the -newuser flag is set and if -debugmode is requested
	void processInputArguments(int argCount, char** argVector);
	bool validateCredentials(const string& username, const string& password); // to match with existing username and password
	bool createNewUser(); // creating a new user

	bool logIn(FileSystem& fileIO); // overall function to login a user
	
	//FILE IO ----------------------------------------------------------------------------------------
	void loadUserCredentials(FileSystem& fileIO); // need data from File System to collect the list
	void storeNewUser(FileSystem& fileIO); // storing the new user through file system 
	//FILE IO ----------------------------------------------------------------------------------------

public:
	Authenticator(FileSystem& fileIO, int& argCount, char** argVector); // this is a special function which called parameterized constructor
	~Authenticator(); // destructor
	
	void toggleDebugMode(); // to turn the debug mode to on or off
	bool isDebugMode() const; // true when the debug mode is on
	bool isLoggedIn() const; // login is successful or not
	string encryptData(const string& inputString); // returns string in an encryption format
	string getLoggedInUser() const; // returns username string to show who is logged in
	vector<string> getInputArguments() const; // returns the inputArgs (getter, private data) which is supplied by the user
};

//Written by Rami
class user {
private:
	string username;
	string encryptedPassword;

public:
	user() : username(""), encryptedPassword("") {}
	user(string name, string pass) : username(name), encryptedPassword(pass) {}

	string getUsername() {
		return username;
	}

	string getPassword() {
		return encryptedPassword;
	}

	void setUsername(string username) {
		this->username = username;
	}

	void setPassword(string password) {
		this->encryptedPassword = password;
	}
};

