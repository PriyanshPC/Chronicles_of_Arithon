#pragma once

#define PLAYERSTATE_H

#include <string>

class PlayerState {
private:
    int chapterNumber;
    int chapterEvent;
    int health;
    int weapon;
    int currentEventsDecisions[6];
    int previousChaptersOutcome[5];
    int currentDecisionIndex;
    int outcomeIndex;

public:

    PlayerState();

    int getChapterNumber() const;
    int getChapterEvent() const;
    int getHealth() const;
    int getWeapon() const;
    int getCurrentDecision(int index) const;
    int getPreviousOutcome(int index) const;
    int getCurrentDecisionIndex() const;
    int getOutcomeIndex() const;

    void setChapterNumber(int chapter);
    void setChapterEvent(int event);
    void setHealth(int hp);
    void setWeapon(int weaponType);
    void setCurrentDecision(int index, int decision);
    void setPreviousOutcome(int index, int outcome);
    void setCurrentDecisionIndex(int index);
    void setOutcomeIndex(int index);

    void resetCurrentDecisions();
    void addCurrentDecision(int decision);
    void addPreviousOutcome(int outcome);
    void resetToDefault();
    std::string serialize() const;
};
