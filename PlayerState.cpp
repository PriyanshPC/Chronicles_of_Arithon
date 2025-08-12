#include "PlayerState.h" // Include the header file for the PlayerState class
#include <sstream>       // Include for string stream operations (used in serialization)
#include <thread>        // Include for thread operations (not used in this file)
#include <chrono>        // Include for time utilities (not used in this file)
#include <iostream>      // Include for input/output stream operations (not used in this file)


// Constructor for PlayerState, initializes member variables to default values
PlayerState::PlayerState() {
    resetToDefault(); // Set all state variables to their default values
}

// Resets all player state variables to their default values
void PlayerState::resetToDefault() {
    chapterNumber = 1;         // Start at chapter 1
    chapterEvent = 1;          // Start at event 1
    health = 100;              // Set player health to 100
    weapon = 1;                // Set default weapon type to 1
    currentDecisionIndex = 0;  // No decisions made yet
    outcomeIndex = 0;          // No outcomes recorded yet
    gameEnded = false;         // Game is not ended

    // Initialize all current event decisions to 0
    for (int i = 0; i < 6; i++) {
        currentEventsDecisions[i] = 0;
    }

    // Initialize all previous chapter outcomes to 0
    for (int i = 0; i < 5; i++) {
        previousChaptersOutcome[i] = 0;
    }
}

// Getter for chapterNumber
int PlayerState::getChapterNumber() const { return chapterNumber; }
// Getter for chapterEvent
int PlayerState::getChapterEvent() const { return chapterEvent; }
// Getter for health
int PlayerState::getHealth() const { return health; }
// Getter for weapon
int PlayerState::getWeapon() const { return weapon; }
// Getter for a specific current decision by index
int PlayerState::getCurrentDecision(int index) const {
    if (index >= 0 && index < 6) return currentEventsDecisions[index]; // Return value if index is valid
    return 0; // Return 0 if index is out of bounds
}
// Getter for a specific previous outcome by index
int PlayerState::getPreviousOutcome(int index) const {
    if (index >= 0 && index < 5) return previousChaptersOutcome[index]; // Return value if index is valid
    return 0; // Return 0 if index is out of bounds
}
// Getter for currentDecisionIndex
int PlayerState::getCurrentDecisionIndex() const { return currentDecisionIndex; }
// Getter for outcomeIndex
int PlayerState::getOutcomeIndex() const { return outcomeIndex; }

// Setter for chapterNumber
void PlayerState::setChapterNumber(int chapter) { chapterNumber = chapter; }
// Setter for chapterEvent
void PlayerState::setChapterEvent(int event) { chapterEvent = event; }
// Setter for health
void PlayerState::setHealth(int hp) { health = hp; }
// Setter for weapon
void PlayerState::setWeapon(int weaponType) { weapon = weaponType; }
// Setter for a specific current decision by index
void PlayerState::setCurrentDecision(int index, int decision) {
    if (index >= 0 && index < 6) currentEventsDecisions[index] = decision; // Set value if index is valid
}
// Setter for a specific previous outcome by index
void PlayerState::setPreviousOutcome(int index, int outcome) {
    if (index >= 0 && index < 5) previousChaptersOutcome[index] = outcome; // Set value if index is valid
}
// Setter for currentDecisionIndex
void PlayerState::setCurrentDecisionIndex(int index) { currentDecisionIndex = index; }
// Setter for outcomeIndex
void PlayerState::setOutcomeIndex(int index) { outcomeIndex = index; }

// Resets all current decisions and the decision index
void PlayerState::resetCurrentDecisions() {
    currentDecisionIndex = 0; // Reset decision index
    for (int i = 0; i < 6; i++) {
        currentEventsDecisions[i] = 0; // Reset each decision to 0
    }
}

// Adds a new decision to the current decisions array
void PlayerState::addCurrentDecision(int decision) {
    if (currentDecisionIndex < 6) { // Only add if there is space
        currentEventsDecisions[currentDecisionIndex] = decision; // Store decision
        currentDecisionIndex++; // Move to next index
    }
}

// Adds a new outcome to the previous outcomes array
void PlayerState::addPreviousOutcome(int outcome) {
    if (outcomeIndex < 5) { // Only add if there is space
        previousChaptersOutcome[outcomeIndex] = outcome; // Store outcome
        outcomeIndex++; // Move to next index
    }
}



// Serializes the player state to a string for saving to a file
std::string PlayerState::serialize() const {
    std::ostringstream oss; // Create a string stream
    oss << "PlayerState :\n";
    oss << "chapterNumber : " << chapterNumber << "\n"; // Output chapter number
    oss << "chapterEvent : " << chapterEvent << "\n";   // Output chapter event
    oss << "health : " << health << "\n";               // Output health
    oss << "weapon : " << weapon << "\n";               // Output weapon
    oss << "currentEventsDecisions[6] : ";
    for (int i = 0; i < 6; ++i) {
        oss << currentEventsDecisions[i];               // Output each current decision
        if (i < 5) oss << ", ";                        // Add comma except after last
    }
    oss << "\n";
    oss << "previousChaptersOutcome[5] : ";
    for (int i = 0; i < 5; ++i) {
        oss << previousChaptersOutcome[i];              // Output each previous outcome
        if (i < 4) oss << ", ";                        // Add comma except after last
    }
    oss << "\n";
    oss << "currentDecisionIndex : " << currentDecisionIndex << "\n"; // Output decision index
    oss << "outcomeIndex : " << outcomeIndex << "\n";                 // Output outcome index
    oss << "gameEnded : " << (gameEnded ? 1 : 0) << "\n";             // Output game ended status (1 or 0)
    return oss.str(); // Return the serialized string
}