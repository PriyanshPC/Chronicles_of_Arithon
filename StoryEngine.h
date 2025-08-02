#pragma once

#define STORYENGINE_H

#include <string>
#include "PlayerState.h"
#include "Authenticator.h"

void clearStoryScreen();
bool shouldClearScreen(int chapterNumber, int eventNumber);
void initializeStoryEngine(UserData& user);
void storyManager(UserData& user);
void displayChapter(int chapterNumber);
void displayEvent(int chapterNumber, int eventNumber);
int outcomeAlgorithm(PlayerState& ps);
bool exitRequest(UserData& user);
void saveProgress(UserData& user);


// JSON Data Access
std::string getStoryData(int chapterNumber, int eventNumber, const std::string& dataType);
int getGameData(int chapterNumber, int eventNumber, char choice, const std::string& dataType);

