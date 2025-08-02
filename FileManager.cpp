#include "FileManager.h"
#include "Authenticator.h"
#include "PlayerState.h"
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

string getFileName(const string& username) {
    return username + ".dat";
}

bool userExists(const string& username) {
    ifstream file(getFileName(username));
    return file.good();
}

bool saveUser(const UserData& user) {
    string filename = getFileName(user.username);

    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return false;
    }

    file << "Username : " << user.username << "\n";
    file << "Password : " << user.password << "\n";
    file << user.playerState->serialize();

    file.close();
    return true;
}



// Helper to trim leading/trailing spaces
static inline string trim(const string& s) {
    size_t start = s.find_first_not_of(" \t");
    size_t end = s.find_last_not_of(" \t");
    return (start == string::npos) ? "" : s.substr(start, end - start + 1);
}

bool loadUser(const string& username, UserData& user) {
    ifstream file(getFileName(username));
    if (!file.is_open()) return false;

    user.playerState->resetToDefault();

    string line;
    while (getline(file, line)) {
        if (line.find("Username :") == 0) {
            user.username = trim(line.substr(line.find(":") + 1));
        }
        else if (line.find("Password :") == 0) {
            user.password = trim(line.substr(line.find(":") + 1));
        }
        else if (line.find("chapterNumber :") == 0) {
            user.playerState->setChapterNumber(stoi(trim(line.substr(line.find(":") + 1))));
        }
        else if (line.find("chapterEvent :") == 0) {
            user.playerState->setChapterEvent(stoi(trim(line.substr(line.find(":") + 1))));
        }
        else if (line.find("health :") == 0) {
            user.playerState->setHealth(stoi(trim(line.substr(line.find(":") + 1))));
        }
        else if (line.find("weapon :") == 0) {
            user.playerState->setWeapon(stoi(trim(line.substr(line.find(":") + 1))));
        }
        else if (line.find("currentEventsDecisions[6] :") == 0) {
            string values = trim(line.substr(line.find(":") + 1));
            stringstream ss(values);
            int val, idx = 0;
            char comma;
            while (ss >> val && idx < 6) {
                user.playerState->setCurrentDecision(idx++, val);
                ss >> comma;
            }
        }
        else if (line.find("previousChaptersOutcome[5] :") == 0) {
            string values = trim(line.substr(line.find(":") + 1));
            stringstream ss(values);
            int val, idx = 0;
            char comma;
            while (ss >> val && idx < 5) {
                user.playerState->setPreviousOutcome(idx++, val);
                ss >> comma;
            }
        }
        else if (line.find("currentDecisionIndex :") == 0) {
            user.playerState->setCurrentDecisionIndex(stoi(trim(line.substr(line.find(":") + 1))));
        }
        else if (line.find("outcomeIndex :") == 0) {
            user.playerState->setOutcomeIndex(stoi(trim(line.substr(line.find(":") + 1))));
        }
    }

    file.close();
    return true;
}

bool deleteUser(const string& username) {
    return (remove(getFileName(username).c_str()) == 0);
}