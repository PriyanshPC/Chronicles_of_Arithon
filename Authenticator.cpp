#include <filesystem> 
#include <fstream>
#include <iomanip>
#include <iostream>
#include <openssl/sha.h>
#include <sstream>
#include <unordered_map>
#include <vector>
#include "Authenticator.h"
#include "FileManager.h"
#include "MenuController.h"
using namespace std;

vector<string> Authenticator::getArgVector() const{
	return inputArgs;
}

int Authenticator::getArgCount() const{
	return countArgs;
}

string Authenticator::trim(const std::string& str)
{
	size_t first = str.find_first_not_of(" \t");
	size_t last = str.find_last_not_of(" \t");
	return (first == string::npos || last == string::npos) ? "" : str.substr(first, last - first + 1);
}

void Authenticator::loadUserCredentialsFromFiles()
{
	if (DebugMode) {
		cout << "[Authenticator] Loading user credentials from files..." << endl;
		cout << " File path" << getExecutableDirectory() << endl;
	}

	for (const auto& entry : filesystem::directory_iterator(getExecutableDirectory()))
	{
		if (entry.path().extension() == ".dat")
		{
			ifstream file(entry.path());
			if (!file.is_open()) continue;

			string line;
			string username;
			string password;

			while (getline(file, line))
			{
				istringstream iss(line);
				string key, value;

				if (getline(iss, key, ':') && getline(iss, value))
				{
					key = trim(key);
					value = trim(value);

					if (key == "Username")
						username = value;
					else if (key == "Password")
						password = value;
				}
			}

			if (!username.empty() && !password.empty())
			{
				if (DebugMode) {
					cout << "[Authenticator] Loaded user: " << username << " with password: " << password << endl;
				}
				userCredentials[username] = password;
			}
		}
	}
}

Authenticator::Authenticator(int& argCount, char** argVector) {

	// Load user credentials from files
	loadUserCredentialsFromFiles();

	// Store the argCount into the member Variable
	countArgs = argCount;

	// To clear any previously stored values inside the vector inputArgs
	inputArgs.clear();
	// Load all input arguments into the vector
	for (int i = 0; i < argCount; ++i) {
		inputArgs.push_back(string(argVector[i]));
	}
	inputPassword = "";
	inputUserName = "";
	inputPasswordConfirmation = "";
	loggedInUserName = "";
	isNewUser = false;
	DebugMode = false;

	if (DebugMode) {
		cout << "[Authenticator] Credential map initialized.\n";
		// Display the loaded user credentials
		for (const auto& pair : userCredentials) {
			cout << "[Authenticator] User: " << pair.first << ", Encrypted Password: " << pair.second << endl;
		}
	}
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

bool Authenticator::isFlag(const string& text) {
	bool notEmpty = !text.empty();
	bool hasMark = (text[0] == '-');
	return notEmpty && hasMark;
}

bool Authenticator::processInputArguments() {
	// Check if -info appears anywhere in the CLA list
	for (int i = 1; i < countArgs; ++i) {
		if (inputArgs[i] == "-info") {
			showHelp(false, inputArgs[0]);
			return false;
		}
	}

	string fullPath(inputArgs[0]);
	size_t pos = fullPath.find_last_of("\\/");
	string filename = ".\\" + ((pos != string::npos) ? fullPath.substr(pos + 1) : fullPath);

	if (countArgs < 2) {
		cout << "Error: Not enough arguments provided." << endl;
		cout << "Run " << filename << " -info to learn proper usage." << endl;
		cout << "Or use without command line arguments and use the main menu instead." << endl;
		return false;
	}

	for (int i = 1; i < countArgs; ++i) {
		string arg = inputArgs[i];

		if ((arg == "-euser" || arg == "-nuser" || arg == "-pswd" || arg == "-cpswd") && (i + 1 >= countArgs || isFlag(inputArgs[i + 1]))) {
			cout << "Error: Flag '" << arg << "' requires a value." << endl;
			return false;
		}

		if (arg == "-nuser" && i + 1 < countArgs && !isFlag(inputArgs[i+1])) {
			inputUserName = inputArgs[++i];
			isNewUser = true;
		}
		else if (arg == "-euser" && i + 1 < countArgs && !isFlag(inputArgs[i + 1])) {
			inputUserName = inputArgs[++i];
			isNewUser = false;
		}
		else if (arg == "-pswd" && i + 1 < countArgs && !isFlag(inputArgs[i + 1])) {
			inputPassword = inputArgs[++i];
		}
		else if (arg == "-cpswd" && i + 1 < countArgs && !isFlag(inputArgs[i + 1])) {
			inputPasswordConfirmation = inputArgs[++i];
		}
		else if (arg == "-debug") {
			DebugMode = true;
		}
		else {
			cout << "Error: Unknown argument '" << arg << "'" << endl;
			return false;
		}
	}

	if (DebugMode) {
		displayInputArguments();
	}

	return true;
}

void Authenticator::displayInputArguments() {
	cout << "Username: " << inputUserName << endl;
	cout << "Password: " << inputPassword << endl;
	cout << "Repeated Password: " << inputPasswordConfirmation << endl;
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
		if(DebugMode) {
			cout << "Debug: Username length is invalid." << endl;
			cout << "Error: Username must be between " << MIN_USERNAME_LENGTH << " and " << MAX_USERNAME_LENGTH << " characters long." << endl;
		}
		return false;
	}

	// Check if username is alphanumeric and underscores only
	bool isAlphanumeric = username.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789_") == string::npos;
	if (isAlphanumeric == false) {
		if (DebugMode) {
			cout << "Error: Username can only contain alphanumeric characters and underscores." << endl;
		}
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

bool Authenticator::validateCredentials(const string& username, const string& password)
{
	if (inputUserName.empty() || inputPassword.empty()) {
		cout << "Error: Username and password cannot be empty." << endl;
		return false;
	}

	if (userExists(inputUserName) == false) {
		cout << "Error: User does not exist." << endl;
		return false;
	}
	string encryptedPassword = encryptData(password);
	if (userCredentials[username] == encryptedPassword) {
		return true; // Credentials are valid
	}
	else {
		cout << "Error: User exists but password does not match." << endl;
		return false; // Credentials are invalid
	}
}

bool Authenticator::logIn(UserData& user)
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
	if (isNewUser == true) {
		if (createNewUser() == true) {
			// If the new user is created successfully, log them in
			loggedInUserName = inputUserName;

			// set user.password to the userCredentials password
			// set user.playerState to default values
			user.password = userCredentials[inputUserName];
			user.playerState->resetToDefault();

			// Store the new user credentials
			if (saveUser(user)) {
				if (DebugMode) {
					cout << "New user created and logged in successfully." << endl;
				}
				return true;
			}
			else {
				if (DebugMode){
					cout << "Error: Failed to save new user to file." << endl;
				}
				loggedInUserName = "";
				return false;
			}
		}
		else {
			if(DebugMode){
				// If the new user creation failed, print an error message
				cout << "Error: User creation failed." << endl;
			}
			loggedInUserName = "";
			return false;
		}
	}
	else {
		// If the user exists, validate the credentials
		if (validateCredentials(inputUserName, inputPassword) == true)
		{
			loggedInUserName = inputUserName;

			// set user.password to the userCredentials password
			user.password = userCredentials[inputUserName];

			// Load user data from file
			UserData storedUser;								// Structure to hold loaded user data
			storedUser.playerState = new PlayerState();			// Allocate memory for player state
			if (loadUser(user.username, storedUser) == false) { // Load user data from file
				if (DebugMode) {
					cout << "Failed to load game data for: " << user.username << endl;
				}
				return false;
			}
			*user.playerState = *storedUser.playerState; // Copy player state

			if(DebugMode) {
				cout << "Logged in successfully as " << loggedInUserName << "." << endl;
			}
			return true;
		}
		else {
			if (DebugMode)	 {
				cout << "Error: Invalid username or password." << endl;
			}
			loggedInUserName = "";
			return false;
		}
	}
}

bool Authenticator::createNewUser()
{
	if (inputUserName.empty() || inputPassword.empty()) {
		cout << "Error: Username and password cannot be empty." << endl;
		return false;
	}

	//if username exists then print error message and return false
	if (userExists(inputUserName) == true) {
		cout << "Error: User already exists. Choose a different username" << endl;
		return false;
	}

	if (inputPassword != inputPasswordConfirmation) {
		cout << "Error: Passwords don't match. Try again." << endl;
		return false;
	}

	//if username is not valid then print error message and return false
	if (isValidUsername(inputUserName) == false) {
		cout << endl;
		cout << "Error: Invalid user name" << endl;
		cout << "\tLength between 3 and 20 characters" << endl;
		cout << "\tAlphanumeric and underscores only, i.e.no special characters." << endl;
		cout << endl;
		return false;
	}

	//if password is not valid then print error message and return false
	if (isValidPassword(inputPassword) == false) {
		cout << endl;
		cout << "Error: Invalid password" << endl;
		cout << "\tMinimum length " << MIN_PASSWORD_LENGTH << endl;
		cout << "\tBoth Uppercase and Lowercase letters needed" << endl;
		cout << "\tAt least one digit needed" << endl;
		cout << "\tAt least one special character needed" << endl;
		cout << endl;
		return false;
	}


	userCredentials[inputUserName] = encryptData(inputPassword);
	cout << "User " << inputUserName << " created successfully." << endl;
	return true;
}

bool Authenticator::isNewUserFlag() const {
	return isNewUser;
}

string Authenticator::getUsername() const {
	return inputUserName;
}

string Authenticator::getPassword() const {
	return inputPassword;
}

void Authenticator::setInputVariables(const string& username, const string& password, const string& passwordConfirm, bool newUser, bool debugMode)
{
	inputUserName = username;
	inputPassword = password;
	inputPasswordConfirmation = passwordConfirm;
	isNewUser = newUser;
	DebugMode = debugMode;
}