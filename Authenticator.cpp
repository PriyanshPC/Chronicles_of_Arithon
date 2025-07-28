#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <unordered_map>
#include <openssl/sha.h>

#include "Authenticator.h"
#include "FileManager.h"

using namespace std;

Authenticator::Authenticator(FileIO& fileIO, int& argCount, char** argVector) {

	cout << "[Authenticator] Credential map initialized.\n";
	loadUserCredentials(fileIO);

	// Process command line arguments
	processInputArguments(argCount, argVector);

	// if DebugMode is true then print out the input arguments
	if (DebugMode == true) {
		displayInputArguments();
	}

	//login if the user is not already logged in
	if (isLoggedIn() == false) {
		logIn(fileIO);
	}
	else {
		cout << "Already logged in as: " << loggedInUserName << endl;
	}
}

Authenticator::~Authenticator() {
	// Destructor can be used for cleanup if needed
}

string Authenticator::sha256(const string& input) {
	// Create a SHA256 hash of the input string
	unsigned char hash[SHA256_DIGEST_LENGTH];
	SHA256(reinterpret_cast<const unsigned char*>(input.c_str()), input.size(), hash);
	// Convert the hash to a hexadecimal string
	stringstream ss;
	for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
	return ss.str();
}

string Authenticator::encryptData(const string& inputString)
{
	//Calling the sha256 function to encrypt the input string
	return sha256(inputString);
}

string Authenticator::getLoggedInUser() const {
	return isLoggedIn() ? loggedInUserName : "";
}

void Authenticator::processInputArguments(int argCount, char** argVector) {

	if (argCount < 5) {
		cout << "Error: Not enough arguments provided." << endl;
		cout << "Usage: " << argVector[0] << "-u username -p password <-newuser> <-help>" << endl;
		exit(1);
	}

	inputArgs.clear(); // to clear any previously stored values inside the vector inputArgs

	for (int i = 0; i < argCount; ++i) {
		inputArgs.push_back(string(argVector[i]));
	}

	for (int i = 1; i < argCount; i++) {
		if (argVector[i] == string("-u") && i + 1 < argCount) {
			inputUserName = argVector[++i];
		}
		else if (argVector[i] == string("-p") && i + 1 < argCount) {
			inputPassword = argVector[++i];
		}
		else if (argVector[i] == string("-new")) {
			isNewUser = true;
		}
		else if (argVector[i] == string("-debug")) {
			DebugMode = true;
		}
		else if (argVector[i] == string("-help")) {
			cout << "Usage: " << argVector[0] << " -u <username> -p <password> [-new] [-debug]" << endl;
			cout << "-u <username> : Specify the username" << endl;
			cout << "-p <password> : Specify the password" << endl;
			cout << "-new          : Create a new user" << endl;
			cout << "-debug        : Enable debug mode" << endl;
			cout << "-help         : Display this help message" << endl;
			exit(0);
		}
		else {
			cout << "Error: Unknown argument " << argVector[i] << endl;
			exit(1);
		}
	}
}

void Authenticator::displayInputArguments() {
	cout << "Username: " << inputUserName << endl;
	cout << "Password: " << inputPassword << endl;
	cout << "Encrypted Password: " << encryptData(inputPassword) << endl;
	cout << "Is New User: " << (isNewUser ? "Yes" : "No") << endl;
	cout << "Debug Mode: " << (DebugMode ? "Enabled" : "Disabled") << endl;
}

void Authenticator::toggleDebugMode() {
	DebugMode = !DebugMode;
}

bool Authenticator::isLoggedIn() const {
	return !loggedInUserName.empty();
}

bool Authenticator::isDebugMode() const {
	return DebugMode;
}

bool Authenticator::isValidUsername(const string& username) {
	// Length between 3 and 20 characters, 
	// Alphanumeric and underscores only, i.e. no special characters
	// Checking length of username
	bool isProperLengths = (username.length() >= MIN_USERNAME_LENGTH && username.length() <= MAX_USERNAME_LENGTH);
	if (isProperLengths == false) {
		cout << "Error: Username must be between " << MIN_USERNAME_LENGTH << " and " << MAX_USERNAME_LENGTH << " characters long." << endl;
		return false;
	}

	// Check if username is alphanumeric and underscores only
	bool isAlphanumeric = username.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") == string::npos;
	if (isAlphanumeric == false) {
		cout << "Error: Username can only contain alphanumeric characters and underscores." << endl;
		return false;
	}
	return true;
}

bool Authenticator::isValidPassword(const string& password)
{
	// Minimum length MIN_PASSWORD_LENGTH
	// Both Uppercase and Lowercase letters needed
	// At least one digit needed
	// At least one special character needed
	bool isProperLength = (password.length() >= MIN_PASSWORD_LENGTH);
	if (!isProperLength) {
		cout << "Error: Password must be at least " << MIN_PASSWORD_LENGTH << " characters long." << endl;
		return false;
	}

	bool hasUppercase = password.find_first_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ") != string::npos;
	if (!hasUppercase) {
		cout << "Error: Password must contain at least one uppercase letter." << endl;
		return false;
	}

	bool hasLowercase = password.find_first_of("abcdefghijklmnopqrstuvwxyz") != string::npos;
	if (!hasLowercase) {
		cout << "Error: Password must contain at least one lowercase letter." << endl;
		return false;
	}

	bool hasDigit = password.find_first_of("0123456789") != string::npos;
	if (!hasDigit) {
		cout << "Error: Password must contain at least one digit." << endl;
		return false;
	}

	bool hasSpecialChar = password.find_first_of("!@#$%^&*()-_=+[]{}|;:',.<>?") != string::npos;
	if (!hasSpecialChar) {
		cout << "Error: Password must contain at least one special character." << endl;
		return false;
	}

	return true;
}

bool Authenticator::userExists(const string& username) {
	// Check if the user already exists in the credentials map
	return userCredentials.find(username) != userCredentials.end();
}

vector<string> Authenticator::getInputArguments() const {
	return inputArgs;
}

void Authenticator::loadUserCredentials(FileSystem& fileIO)
{
	vector<User> users = fileIO.readCredentialTable();

	for (const auto& user : users) {
		userCredentials[user.username] = user.encryptedPassword;
	}
}

bool Authenticator::validateCredentials(const string& username, const string& password)
{
	if (userExists(inputUserName) == false) {
		cout << "Error: User does not exist. Register using the -newuser keyword." << endl;
		return false;
	}
	string encryptedPassword = encryptData(password);
	if (userCredentials[username] == encryptedPassword) {
		return true; // Credentials are valid
	}
	else {
		return false; // Credentials are invalid
	}
}

bool Authenticator::userExists(const string& username) {
	// Check if the user already exists in the credentials map
	return userCredentials.find(username) != userCredentials.end();
}

bool Authenticator::logIn(FileIO& fileIO)
{
	//if new user is requested, then create a new user
	//	Check if the username exists 
	//	Check if the username and password are valid
	//	Create a new user if it doesn't exist
	//	Otherwise error message that username exists
	//If newuser is not requested, then check if the username exists
	//	If username exists then validate username and password against stored encrypted password
	//	If matched then update loggedinUsername
	//	otherwise print that invalid username or password

	if (inputUserName.empty() || inputPassword.empty()) {
		cout << "Error: Username and password cannot be empty." << endl;
		return false;
	}

	if (isNewUser == true) {
		if (createNewUser() == true) {
			// If the new user is created successfully, log them in
			loggedInUserName = inputUserName;

			// Store the new user credentials
			storeNewUser(fileIO);

			cout << "New user created and logged in successfully." << endl;
			return true;
		}
		else {
			cout << "Error: Failed to create new user." << endl;
			loggedInUserName = "";
			return false;
		}
	}
	else {
		// If the user exists, validate the credentials
		if (validateCredentials(inputUserName, inputPassword) == true)
		{
			loggedInUserName = inputUserName;
			cout << "Logged in successfully as " << loggedInUserName << "." << endl;
			return true;
		}
		else {
			cout << "Error: Invalid username or password." << endl;
			loggedInUserName = "";
			return false;
		}
	}
}

bool Authenticator::createNewUser()
{
	//if username exists then print error message and return false
	if (userExists(inputUserName) == true) {
		cout << "Error: User already exists. Choose a new username" << endl;
		return false;
	}

	//if username is not valid then print error message and return false
	if (isValidUsername(inputUserName) == false) {
		cout << "Error: Invalid user name" << endl;
		cout << "Length between 3 and 20 characters" << endl;
		cout << "Alphanumeric and underscores only, i.e.no special characters." << endl;
		return false;
	}

	//if password is not valid then print error message and return false
	if (isValidPassword(inputPassword) == false) {
		cout << "Error: Invalid password" << endl;
		cout << "Minimum length " << MIN_PASSWORD_LENGTH << endl;
		cout << "Both Uppercase and Lowercase letters needed" << endl;
		cout << "At least one digit needed" << endl;
		cout << "At least one special character needed" << endl;
		return false;
	}
	userCredentials[inputUserName] = encryptData(inputPassword);
	cout << "User " << inputUserName << " created successfully." << endl;
	return true;
}

void Authenticator::storeNewUser(FileIO& fileIO)
{
	fileIO.addCredential(inputUserName, encryptData(inputPassword));
}