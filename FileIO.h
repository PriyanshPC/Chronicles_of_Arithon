#pragma once
#include <vector>
#include <string>
#include <iostream>
using namespace std;

// This structure is used to represent a user credential.
// This interface is a requirement from the Authorization class and can't be changed.
typedef struct User {
    std::string username;
    std::string encryptedPassword;
}User;

class FileIO {
private:
	// This vector simulates the credential table in memory. 
    // This interface is a requirement from Authorization class.
    vector<User> credentialList;

public:
    FileIO();

    // Returns a vector of User structs
    // This function definiton is used to read the credential table from the file. 
    // This interface is a requirement from Authorization class and can't be changed 
    vector<User> readCredentialTable(); 

	// This function definiton is used to add a new credential to the file. 
    // This interface is a requirement from Authorization class and can't be changed 
    bool addCredential(const string& username, const string& hashedPassword);
};
