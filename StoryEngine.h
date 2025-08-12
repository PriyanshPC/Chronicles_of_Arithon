#pragma once // Ensures the header is included only once during compilation

#define STORYENGINE_H // Macro guard for additional include protection

#include <string> // Provides the std::string class
#include "PlayerState.h" // Includes the PlayerState class definition
#include "Authenticator.h" // Includes the UserData structure and related functions
#include <nlohmann/json.hpp> // Includes the JSON library

extern nlohmann::json storyData; // Externally defined JSON object holding story data

// Clears the story screen in the UI
void clearStoryScreen();

// Determines if the screen should be cleared for a given chapter and event
bool shouldClearScreen(int chapterNumber, int eventNumber);

// Initializes the story engine for the given user
void initializeStoryEngine(UserData& user);

// Manages the story flow for the user
void storyManager(UserData& user);

// Displays the specified chapter
void displayChapter(int chapterNumber);

// Shows the final outcome based on the outcome ID
void showUltimateOutcome(int outcomeId);

// Calculates the outcome based on the player's state
int outcomeAlgorithm(PlayerState& ps);

// Handles user request to exit the story
bool exitRequest(UserData& user);

// Saves the user's progress in the story
void saveProgress(UserData& user);

// Retrieves story data as a string for a specific chapter, event, and data type
std::string getStoryData(int chapterNumber, int eventNumber, const std::string& dataType);

// Retrieves game data as an integer for a specific chapter, event, choice, and data type
int getGameData(int chapterNumber, int eventNumber, char choice, const std::string& dataType);



