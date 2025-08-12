#include "FileManager.h"                  // Include header for file manager declarations
#include "Authenticator.h"                // Include header for authentication utilities
#include "PlayerState.h"                  // Include header for player state management
#include <fstream>                        // Include file stream for file I/O
#include <iostream>                       // Include input/output stream for console I/O
#include <string>                         // Include string class
#include <sstream>                        // Include string stream for parsing
#include <algorithm>                      // Include algorithms (e.g., for trimming)

using namespace std;                      // Use the standard namespace

// Returns the filename for a given username by appending ".dat"
string getFileName(const string& username) {
    return username + ".dat";             // Concatenate username with ".dat" extension
}

// Checks if a user file exists for the given username
bool userExists(const string& username) {
    ifstream file(getFileName(username)); // Try to open the file for reading
    return file.good();                   // Return true if file exists and is accessible
}

// Saves user data to a file
bool saveUser(const UserData& user) {
    string filename = getFileName(user.username); // Get the filename for the user

    ofstream file(filename);                      // Open file for writing
    if (!file.is_open()) {                       // Check if file opened successfully
        cout << "Failed to open file: " << filename << endl; // Print error message
        return false;                            // Return false if file couldn't be opened
    }

    file << "Username : " << user.username << "\n";          // Write username to file
    file << "Password : " << user.password << "\n";          // Write password to file
    file << user.playerState->serialize();                   // Write serialized player state

    file.close();                                            // Close the file
    return true;                                             // Return true on success
}

// Helper function to trim leading and trailing spaces from a string
static inline string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t");               // Find first non-space/tab character
    size_t end = s.find_last_not_of(" \t");                  // Find last non-space/tab character
    return (start == string::npos) ? "" : s.substr(start, end - start + 1); // Return trimmed string
}

// Loads user data from a file into a UserData object
bool loadUser(const string& username, UserData& user) {
    ifstream file(getFileName(username));                    // Open the user file for reading
    if (!file.is_open()) return false;                       // Return false if file can't be opened

    user.playerState->resetToDefault();                      // Reset player state to default values

    string line;                                             // String to hold each line from file
    while (getline(file, line)) {                            // Read file line by line
        if (line.find("Username :") == 0) {                  // If line contains username
            user.username = trim(line.substr(line.find(":") + 1)); // Extract and trim username
        }
        else if (line.find("Password :") == 0) {           // If line contains password
            user.password = trim(line.substr(line.find(":") + 1)); // Extract and trim password
        }
        else if (line.find("chapterNumber :") == 0) {      // If line contains chapter number
            user.playerState->setChapterNumber(stoi(trim(line.substr(line.find(":") + 1)))); // Set chapter number
        }
        else if (line.find("chapterEvent :") == 0) {       // If line contains chapter event
            user.playerState->setChapterEvent(stoi(trim(line.substr(line.find(":") + 1))));   // Set chapter event
        }
        else if (line.find("health :") == 0) {             // If line contains health
            user.playerState->setHealth(stoi(trim(line.substr(line.find(":") + 1))));         // Set health
        }
        else if (line.find("weapon :") == 0) {             // If line contains weapon
            user.playerState->setWeapon(stoi(trim(line.substr(line.find(":") + 1))));         // Set weapon
        }
        else if (line.find("currentEventsDecisions[6] :") == 0) { // If line contains current decisions array
            string values = trim(line.substr(line.find(":") + 1));   // Extract values string
            stringstream ss(values);                                 // Create stringstream for parsing
            int val, idx = 0;                                        // Value and index for array
            char comma;                                              // To consume commas
            while (ss >> val && idx < 6) {                           // Parse up to 6 values
                user.playerState->setCurrentDecision(idx++, val);    // Set each decision value
                ss >> comma;                                         // Consume comma separator
            }
        }
        else if (line.find("previousChaptersOutcome[5] :") == 0) { // If line contains previous outcomes array
            string values = trim(line.substr(line.find(":") + 1));   // Extract values string
            stringstream ss(values);                                 // Create stringstream for parsing
            int val, idx = 0;                                        // Value and index for array
            char comma;                                              // To consume commas
            while (ss >> val && idx < 5) {                           // Parse up to 5 values
                user.playerState->setPreviousOutcome(idx++, val);    // Set each outcome value
                ss >> comma;                                         // Consume comma separator
            }
        }
        else if (line.find("currentDecisionIndex :") == 0) {       // If line contains current decision index
            user.playerState->setCurrentDecisionIndex(stoi(trim(line.substr(line.find(":") + 1)))); // Set index
        }
        else if (line.find("outcomeIndex :") == 0) {               // If line contains outcome index
            user.playerState->setOutcomeIndex(stoi(trim(line.substr(line.find(":") + 1))));         // Set index
        }
        else if (line.find("gameEnded :") == 0) {                    // If line contains game ended flag
            user.playerState->setGameEnded(stoi(trim(line.substr(line.find(":") + 1))) != 0);       // Set flag
        }
    }

    file.close();                                                    // Close the file
    return true;                                                     // Return true on success
}

// Deletes the user file for the given username
bool deleteUser(const string& username) {
    return (remove(getFileName(username).c_str()) == 0);             // Remove file, return true if successful
}