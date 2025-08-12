#pragma once // Ensures the header is included only once during compilation

#define EVENTGENERATOR_H // Macro guard for additional include protection

// Structure to represent the state of an enemy in the game
struct EnemyState {
    int enemyHP;         // Current health points of the enemy
    int enemyMaxHP;      // Maximum health points of the enemy
    int enemyPotions;    // Number of potions the enemy has
    int enemyDamage;     // Damage the enemy can inflict
    int enemyAction;     // Current action/state of the enemy
    bool enemyAttack;    // Indicates if the enemy is attacking
};

// Displays an event based on chapter and event number, modifies user state
char displayEvent(int chapterNumber, int eventNumber, UserData& user);

// Handles combat logic between the user and an enemy
char combat(EnemyState enemy, int weaponDamage);

// Handles a puzzle event in the game
char puzzle();
