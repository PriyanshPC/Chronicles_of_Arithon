#pragma once // Ensures the header is included only once during compilation

#include <string> // Provides the std::string class

// Class representing the state of the player throughout the game
class PlayerState {
private:
    int chapterNumber;                // Current chapter number
    int chapterEvent;                 // Current event within the chapter
    int health;                       // Player's health points
    int weapon;                       // Player's weapon type or ID
    int currentEventsDecisions[6];    // Decisions made in the current chapter
    int previousChaptersOutcome[5];   // Outcomes from previous chapters
    int currentDecisionIndex;         // Index for the current decision
    int outcomeIndex;                 // Index for the current outcome
    bool gameEnded;                   // Flag indicating if the game has ended

public:
    PlayerState(); // Constructor

    // Getters for player state fields
    int getChapterNumber() const;
    int getChapterEvent() const;
    int getHealth() const;
    int getWeapon() const;
    int getCurrentDecision(int index) const;
    int getPreviousOutcome(int index) const;
    int getCurrentDecisionIndex() const;
    int getOutcomeIndex() const;

    // Setters for player state fields
    void setChapterNumber(int chapter);
    void setChapterEvent(int event);
    void setHealth(int hp);
    void setWeapon(int weaponType);
    void setCurrentDecision(int index, int decision);
    void setPreviousOutcome(int index, int outcome);
    void setCurrentDecisionIndex(int index);
    void setOutcomeIndex(int index);

    // Checks if the game has ended
    bool isGameEnded() const { return gameEnded; }

    // Sets the game ended flag
    void setGameEnded(bool val) { gameEnded = val; }

    // Resets the current decisions array
    void resetCurrentDecisions();

    // Adds a decision to the current decisions array
    void addCurrentDecision(int decision);

    // Adds an outcome to the previous outcomes array
    void addPreviousOutcome(int outcome);

    // Resets all player state fields to default values
    void resetToDefault();

    // Serializes the player state to a string
    std::string serialize() const;
};
