#pragma once
#include <vector>
#include <string>
#include <unordered_map>

#define MAX_USERNAME_LENGTH 20
#define MIN_USERNAME_LENGTH 3
#define MIN_PASSWORD_LENGTH 6

using namespace std;

struct UserData; // Forward declaration
class Authenticator
{
private:
	string loggedInUserName;
	string inputUserName;
	string inputPassword;
	string inputPasswordConfirmation;
	bool isNewUser;
	bool DebugMode;
	//bool DebugMode = true;

	int countArgs;
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

	//Function to validate credentials against stored user credentials
	bool validateCredentials(const string& username, const string& password);

	//Function to create a new user - just adding to the userCredentials map - saving is separate
	bool createNewUser();

	//Function to trim whitespace from a string
	string trim(const string& str);

	// Function to load all user information and credentials from files
	void loadUserCredentialsFromFiles();

	bool isFlag(const string& text);

public:
	vector<string> getArgVector()const;
	int getArgCount()const;
	Authenticator(int& argCount, char** argVector);
	bool isNewUserFlag() const;
	string encryptData(const string& inputString);
	string getUsername() const;
	string getPassword() const;
	bool processInputArguments();
	bool isDebugMode() const;
	bool logIn(UserData& user);
	void setInputVariables(const string& username, const string& password, bool newUser, bool debugMode);

	//Unused functions
	void toggleDebugMode();
	bool isLoggedIn() const;
	string getLoggedInUser() const;
	vector<string> getInputArguments() const;
};

