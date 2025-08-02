#include "StoryEngine.h"
#include "FileManager.h"
#include "Authenticator.h"
#include "PlayerState.h"
#include <iostream>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;
using namespace std;

json storyData;

void clearStoryScreen() {
    system("cls");
}

bool shouldClearScreen(int chapterNumber, int eventNumber) {
    try {
        const auto& choices = storyData[to_string(chapterNumber)]["events"][to_string(eventNumber)]["choices"];
        for (const auto& choice : choices) {
            string desc = choice["description"].get<string>();
            if (desc == "Combat" || desc == "Puzzle") {
                return true;
            }
        }
    }
    catch (...) {
        return false;
    }
    return false;
}

void initializeStoryEngine(UserData& user) {
    clearStoryScreen();

    ifstream file("storyData.json");
    if (file.is_open()) {
        file >> storyData;
        file.close();
    }
    else {
        cerr << "Failed to load story data. Exit and reload the game\n";
        exit(1);
    }

    storyManager(user);
}

void storyManager(UserData& user) {
    PlayerState* playerState = user.playerState;
    int chapter = playerState->getChapterNumber();
    int event = playerState->getChapterEvent();

    displayChapter(chapter);

    while (event <= 10) {
        // Special Combat Events
        if ((chapter == 2 && event == 10) || (chapter == 3 && event == 10)) {
            clearStoryScreen();
            cout << "\nCombat event in progress...\n";
            int outcome = 1; // Temporary hardcoded outcome
            playerState->addCurrentDecision(outcome);
            saveProgress(user);
            break;
        }

        // Chapter 5 Special Routing
        if (chapter == 5 && (event == 8 || event == 9)) {
            displayEvent(chapter, event);
            char choice;
            cout << "Enter your choice (A/B): ";
            cin >> choice;
            choice = toupper(choice);

            int weapon = playerState->getWeapon();
            int outcome = getGameData(chapter, event, choice, "outcome");
            playerState->addCurrentDecision(outcome);

            if (choice == 'A') {
                playerState->setChapterNumber(6);
                playerState->setChapterEvent(2);
            }
            else if (choice == 'B') {
                if (weapon == 1) playerState->setChapterEvent(11);
                else if (weapon == 3) playerState->setChapterEvent(10);
            }

            saveProgress(user);
            break;
        }

        // Chapter 6 story version decision
        if (chapter == 6 && event == 1) {
            int lastOutcome = playerState->getPreviousOutcome(playerState->getOutcomeIndex() - 1);
            int weapon = playerState->getWeapon();

            if ((lastOutcome == 1 && weapon == 1) ||
                ((lastOutcome == 1 || lastOutcome == 2) && weapon == 3)) {
                playerState->setChapterEvent(1); // story1 events1
            }
            else {
                playerState->setChapterEvent(2); // story2 events2
            }
        }

        if (shouldClearScreen(chapter, event)) {
            clearStoryScreen();
        }

        displayEvent(chapter, event);

        string input;
        cout << "Enter your choice (A/B or 'exit'): ";
        cin >> input;

        if (_kbhit()) {
            char ch = _getch();
            if (ch == 27) {
                if (exitRequest(user)) exit(0);
                else return;
            }
        }

        for (char& c : input) c = toupper(c);
        if (input == "EXIT") {
            if (exitRequest(user)) exit(0);
            else return;
        }

        char choice = toupper(input[0]);
        int decisionOutcome = getGameData(chapter, event, choice, "outcome");
        int nextEvent = getGameData(chapter, event, choice, "nextEventId");

        playerState->addCurrentDecision(decisionOutcome);

        if (nextEvent == 0 || playerState->getCurrentDecisionIndex() >= 6)
            break;

        playerState->setChapterEvent(nextEvent);
        event = nextEvent;
    }

    // End of Chapter
    if (playerState->getCurrentDecisionIndex() == 6 || event == 0) {
        int chapterOutcome = outcomeAlgorithm(*playerState);
        playerState->addPreviousOutcome(chapterOutcome);
        playerState->setChapterNumber(chapter + 1);
        playerState->setChapterEvent(1);
        playerState->resetCurrentDecisions();

        cout << "\nChapter " << chapter << " completed.\n";
        saveProgress(user);
        cout << "Progress Saved! Press Enter to continue...";
        cin.ignore();
        cin.get();

        clearStoryScreen();
        displayChapter(playerState->getChapterNumber());
        storyManager(user);
    }
}

void displayChapter(int chapterNumber) {
    string story = getStoryData(chapterNumber, 0, "story");
    string name = "[Chapter Title Missing]";
    try {
        name = storyData[to_string(chapterNumber)]["name"].get<string>();
    }
    catch (...) {}

    clearStoryScreen();
    cout << "\n===== Chapter " << chapterNumber << ": " << name << " =====\n";
    cout << story << "\n\n";
    cout << "Press Enter to continue...";
    cin.ignore();
    cin.get();
    clearStoryScreen();
}

void displayEvent(int chapterNumber, int eventNumber) {
    string description = getStoryData(chapterNumber, eventNumber, "description");

    cout << "\nEvent:\n" << description << "\n";

    try {
        const auto& choices = storyData[to_string(chapterNumber)]["events"][to_string(eventNumber)]["choices"];
        if (choices.is_array()) {
            for (const auto& choice : choices) {
                cout << choice["option"].get<string>() << ": " << choice["description"].get<string>() << endl;
            }
        }
    }
    catch (...) {}
}

int outcomeAlgorithm(PlayerState& ps) {
    int counts[4] = { 0 };
    for (int i = 0; i < 6; i++) {
        int val = ps.getCurrentDecision(i);
        if (val >= 1 && val <= 3)
            counts[val]++;
    }

    int maxOutcome = 1;
    for (int i = 2; i <= 3; i++) {
        if (counts[i] > counts[maxOutcome])
            maxOutcome = i;
    }
    return maxOutcome;
}

bool exitRequest(UserData& user) {
    cout << "\nAre you sure you want to exit? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        saveProgress(user);
        cout << "Progress saved. Exiting game...\nType exit again to exit the console.";
        return true;
    }

    return false;
}

string getStoryData(int chapterNumber, int eventNumber, const string& dataType) {
    string result;
    try {
        if (dataType == "story") {
            result = storyData[to_string(chapterNumber)]["story"].get<string>();
        }
        else if (dataType == "description") {
            result = storyData[to_string(chapterNumber)]["events"][to_string(eventNumber)]["description"].get<string>();
        }
    }
    catch (...) {
        result = "[Data not found]";
    }
    return result;
}

int getGameData(int chapterNumber, int eventNumber, char choice, const string& dataType) {
    try {
        const auto& choices = storyData[to_string(chapterNumber)]["events"][to_string(eventNumber)]["choices"];
        for (const auto& c : choices) {
            if (c["option"].get<string>()[0] == choice) {
                return c[dataType].get<int>();
            }
        }
    }
    catch (...) {
        return 0;
    }
    return 0;
}

void saveProgress(UserData& user) {
    saveUser(user);
}
