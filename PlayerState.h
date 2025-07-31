#define PLAYERSTATE_H


class PlayerState {

private:
    int currentChapter;
    int currentEvent;
    int health;
    int currentWeapon;
    int chapterEventDecisions[5];
    int previousChapterOutcomes[5];

public:
    // Constructor
    PlayerState();

    // Getters and Setters
    int getCurrentChapter() const;
    void setCurrentChapter(int chapter);

    int getCurrentEvent() const;
    void setCurrentEvent(int event);

    int getHealth() const;
    void setHealth(int health);

    int getCurrentWeapon() const;
    void setCurrentWeapon(const int& weapon);

    void getChapterEventDecisions(int decisions[], int size) const;
    void setChapterEventDecisions(const int decisions[], int size);

    void getPreviousChapterOutcomes(int outcomes[], int size) const;
    void setPreviousChapterOutcomes(const int outcomes[], int size);

    // State Management
    void updateState(int chapter, int event, int health, const int& weapon,
        const int decisions[], int decisionSize,
        const int outcomes[], int outcomeSize);
    void retrieveState();

};