#include "PlayerState.h"
#include <sstream>

PlayerState::PlayerState() {
    resetToDefault();
}

void PlayerState::resetToDefault() {
    chapterNumber = 1;
    chapterEvent = 1;
    health = 100;
    weapon = 1;
    currentDecisionIndex = 0;
    outcomeIndex = 0;

    for (int i = 0; i < 6; i++) {
        currentEventsDecisions[i] = 0;
    }

    for (int i = 0; i < 5; i++) {
        previousChaptersOutcome[i] = 0;
    }
}

int PlayerState::getChapterNumber() const { return chapterNumber; }
int PlayerState::getChapterEvent() const { return chapterEvent; }
int PlayerState::getHealth() const { return health; }
int PlayerState::getWeapon() const { return weapon; }
int PlayerState::getCurrentDecision(int index) const {
    if (index >= 0 && index < 6) return currentEventsDecisions[index];
    return 0;
}
int PlayerState::getPreviousOutcome(int index) const {
    if (index >= 0 && index < 5) return previousChaptersOutcome[index];
    return 0;
}
int PlayerState::getCurrentDecisionIndex() const { return currentDecisionIndex; }
int PlayerState::getOutcomeIndex() const { return outcomeIndex; }

void PlayerState::setChapterNumber(int chapter) { chapterNumber = chapter; }
void PlayerState::setChapterEvent(int event) { chapterEvent = event; }
void PlayerState::setHealth(int hp) { health = hp; }
void PlayerState::setWeapon(int weaponType) { weapon = weaponType; }
void PlayerState::setCurrentDecision(int index, int decision) {
    if (index >= 0 && index < 6) currentEventsDecisions[index] = decision;
}
void PlayerState::setPreviousOutcome(int index, int outcome) {
    if (index >= 0 && index < 5) previousChaptersOutcome[index] = outcome;
}
void PlayerState::setCurrentDecisionIndex(int index) { currentDecisionIndex = index; }
void PlayerState::setOutcomeIndex(int index) { outcomeIndex = index; }

void PlayerState::resetCurrentDecisions() {
    currentDecisionIndex = 0;
    for (int i = 0; i < 6; i++) {
        currentEventsDecisions[i] = 0;
    }
}

void PlayerState::addCurrentDecision(int decision) {
    if (currentDecisionIndex < 6) {
        currentEventsDecisions[currentDecisionIndex] = decision;
        currentDecisionIndex++;
    }
}

void PlayerState::addPreviousOutcome(int outcome) {
    if (outcomeIndex < 5) {
        previousChaptersOutcome[outcomeIndex] = outcome;
        outcomeIndex++;
    }
}

// To organize the data when saving to file
std::string PlayerState::serialize() const {
    std::ostringstream oss;
    oss << "PlayerState :\n";
    oss << "chapterNumber : " << chapterNumber << "\n";
    oss << "chapterEvent : " << chapterEvent << "\n";
    oss << "health : " << health << "\n";
    oss << "weapon : " << weapon << "\n";
    oss << "currentEventsDecisions[6] : ";
    for (int i = 0; i < 6; ++i) {
        oss << currentEventsDecisions[i];
        if (i < 5) oss << ", ";
    }
    oss << "\n";
    oss << "previousChaptersOutcome[5] : ";
    for (int i = 0; i < 5; ++i) {
        oss << previousChaptersOutcome[i];
        if (i < 4) oss << ", ";
    }
    oss << "\n";
    oss << "currentDecisionIndex : " << currentDecisionIndex << "\n";
    oss << "outcomeIndex : " << outcomeIndex << "\n";
    return oss.str();
}