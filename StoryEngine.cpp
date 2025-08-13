#include "StoryEngine.h" // Include the header for this module
#include "FileManager.h" // File operations (e.g., save/load)
#include "PlayerState.h" // Player state management
#include "EventGenerator.h" // Event generation logic
#include "MenuController.h"//
#include <iostream> // Standard I/O stream
#include <conio.h> // Console input/output (Windows-specific)
#include <fstream> // File stream for file operations
#include <sstream> // String stream for parsing

using namespace std; // Use the standard namespace

nlohmann::json storyData; // Global JSON object to hold story data

// Clears the console screen
void clearStoryScreen() {
    system("cls"); // Windows command to clear screen
}

// Determines if the screen should be cleared for a given chapter/event
bool shouldClearScreen(int chapterNumber, int eventNumber) {
    try {
        // Access the choices for the current event
        const auto& choices = storyData[to_string(chapterNumber)]["events"][to_string(eventNumber)]["choices"];
        for (const auto& choice : choices) {
            string desc = choice["description"].get<string>(); // Get the description of the choice
            if (desc == "Combat" || desc == "Puzzle") { // If the event is Combat or Puzzle
                return true; // Indicate screen should be cleared
            }
        }
    }
    catch (...) {
        return false; // On error, do not clear screen
    }
    return false; // Default: do not clear screen
}

// Initializes the story engine for a user
void initializeStoryEngine(UserData& user) {
    clearStoryScreen(); // Clear the screen at start

    ifstream file("storyData.json"); // Open the story data file
    if (file.is_open()) {
        file >> storyData; // Load JSON data into storyData
        file.close(); // Close the file
    }
    else {
        cerr << "Failed to load story data. Exit and reload the game\n"; // Error message
        exit(1); // Exit program on failure
    }

    storyManager(user); // Start the story manager loop
}

// Returns weapon damage based on weapon type
int getWeaponDamage(int weaponType) {
    switch (weaponType) {
    case 1: return 10;  // Sword
    case 2: return 20;  // Crossbow
    case 3: return 30;  // Twin-Blade
    default: return 5;  // Default damage
    }
}

// Main story manager loop
void storyManager(UserData& user) {
    PlayerState* playerState = user.playerState; // Get pointer to player's state

    // Check if game has ended
    if (playerState->isGameEnded()) {
        clearStoryScreen(); // Clear screen

        int lastOutcome = playerState->getPreviousOutcome(playerState->getOutcomeIndex() - 1); // Get last outcome
        showUltimateOutcome(lastOutcome); // Show final outcome

        cout << "\nYou have already completed the game.\n";
        cout << "Do you want to start a new game? (y/n): ";
        char restartChoice;
        cin >> restartChoice; // Get user input

        if (toupper(restartChoice) == 'Y') { // If user wants to restart
            playerState->resetToDefault(); // Reset player state
            saveProgress(user); // Save progress
            clearStoryScreen(); // Clear screen
            displayChapter(1); // Show first chapter
            storyManager(user); // Restart story manager
            return;
        }
        else {
            cout << "Thank you for playing!\n";
            exit(0); // Exit game
        }
    }

    int chapter = playerState->getChapterNumber(); // Get current chapter
    int event = playerState->getChapterEvent(); // Get current event

    displayChapter(chapter); // Display current chapter

    cout << "====================\n";
    cout << "||     Events     ||\n";
    cout << "====================\n\n";

    while (true) {
        // === Handle Combat Scenarios
        if ((chapter == 2 && event == 10) ||
            (chapter == 3 && event == 10) ||
            (chapter == 6 && (event == 6 || event == 7)) ||
            (chapter == 7 && event == 7)) {

            clearStoryScreen(); // Clear screen for combat
            EnemyState enemy; // Create enemy state

            // Set enemy stats based on chapter
            if (chapter == 2) enemy = { 60, 60, 2, 20, 0, true };
            else if (chapter == 3) enemy = { 80, 80, 2, 20, 0, true };
            else if ((chapter == 6 || chapter == 7)) enemy = { 100, 100, 2, 20, 0, true };

            int weaponDamage = getWeaponDamage(playerState->getWeapon()); // Get player's weapon damage
            char combatOutcome = combat(enemy, weaponDamage); // Run combat, get outcome
            if (chapter == 7 && event == 6) { // Special puzzle after combat
                clearStoryScreen();
                char puzzleOutcome = puzzle(); // Run puzzle, get outcome
                int outcome = getGameData(chapter, event, puzzleOutcome, "outcome"); // Get outcome from data

                playerState->addCurrentDecision(outcome); // Record decision
                saveProgress(user); // Save progress
                break; // Exit loop
            }
            int outcome = getGameData(chapter, event, combatOutcome, "outcome"); // Get outcome from data

            playerState->addCurrentDecision(outcome); // Record decision
            saveProgress(user); // Save progress
            break; // Exit loop
        }

        // === Chapter 5 split logic
        if (chapter == 5 && (event == 8 || event == 9)) {
            char choice = displayEvent(chapter, event, user); // Show event, get choice
            int weapon = playerState->getWeapon(); // Get weapon
            int outcome = getGameData(chapter, event, choice, "outcome"); // Get outcome

            playerState->addCurrentDecision(outcome); // Record decision
            playerState->setChapterEvent((choice == 'A') ? 10 : (weapon == 3 ? 10 : 11)); // Set next event

            saveProgress(user); // Save progress
            event = playerState->getChapterEvent(); // Update event
            continue; // Continue loop
        }

        // === Start of Chapter 6 — branch to Covert path (Chapter 7) if needed
        if (chapter == 6 && event == 1) {
            int outcome = outcomeAlgorithm(*playerState); // Calculate outcome
            int weapon = playerState->getWeapon(); // Get weapon
            bool useWarPath = (outcome == 1) || ((outcome == 1 || outcome == 2) && weapon == 3); // Determine path

            if (!useWarPath) { // If not war path, branch to chapter 7
                playerState->setChapterNumber(7);
                playerState->setChapterEvent(1);
                chapter = 7;
                event = 1;
                displayChapter(chapter); // Show new chapter
                continue; // Continue loop
            }
        }

        // === Chapter 7 branching into Puzzle or Combat after event 5
        if (chapter == 7 && event == 5) {
            int lastOutcome = playerState->getCurrentDecision(playerState->getCurrentDecisionIndex() - 1); // Get last decision
            playerState->setChapterEvent((lastOutcome == 3) ? 6 : 7); // Set next event based on outcome
            event = playerState->getChapterEvent(); // Update event
            continue; // Continue loop
        }

        // === Standard Event Display
        if (shouldClearScreen(chapter, event)) {
            clearStoryScreen(); // Clear screen if needed
        }

        char choice = displayEvent(chapter, event, user); // Show event, get choice
        int decisionOutcome = getGameData(chapter, event, choice, "outcome"); // Get outcome
        int nextEvent = getGameData(chapter, event, choice, "nextEventId"); // Get next event

        playerState->addCurrentDecision(decisionOutcome); // Record decision

        // Weapon update logic based on chapter/event/choice
        if (chapter == 1 && event == 10) {
            if (choice == 'A') playerState->setWeapon(3);  // Twin-Blade
            else               playerState->setWeapon(1);  // Sword
        }

        if (chapter == 5 && event == 10) {
            if (choice == 'A') playerState->setWeapon(1);  // Sword
            else               playerState->setWeapon(2);  // Crossbow
        }

        if (chapter == 5 && event == 11) {
            if (choice == 'A') playerState->setWeapon(3);  // Twin-Blade
            else               playerState->setWeapon(2);  // Crossbow
        }

        // End chapter if no next event or decisions full
        if (nextEvent == 0 || playerState->getCurrentDecisionIndex() >= 6) {
            break; // Exit loop
        }

        playerState->setChapterEvent(nextEvent); // Set next event
        event = nextEvent; // Update event
    }

    // === Final Chapter Ends (Ch.6 or Ch.7) — End Game
    if (chapter == 6 || chapter == 7) {
        int ultimateOutcome = playerState->getCurrentDecision(playerState->getCurrentDecisionIndex() - 1);  // final combat or puzzle outcome

        playerState->addPreviousOutcome(ultimateOutcome); // Record outcome
        saveProgress(user); // Save progress

        if (ultimateOutcome == 1) {
            cout << "You emerged victorious in battle.\n";
        }
        else if (ultimateOutcome == 2) {
            cout << "You were defeated... but your legacy lives on.\n";
        }
        else if (ultimateOutcome == 3) {
            cout << "You successfully deceived your enemies and protected the realm from the shadows.\n";
        }
        else {
            cout << "The final decision is clouded in mystery...\n";
        }

        cout << "\nPress Enter to view the final outcome...";
        cin.ignore();
        cin.get();

        clearStoryScreen();
        showUltimateOutcome(ultimateOutcome); // Show final outcome

        cout << "\nPress Enter to exit the game...";
        cin.get();
        playerState->setGameEnded(true); // Mark game as ended
        saveProgress(user); // Save progress
        exit(0); // Exit game
    }

    // === Normal Chapters (1–5 only)
    int chapterOutcome = outcomeAlgorithm(*playerState); // Calculate chapter outcome
    playerState->addPreviousOutcome(chapterOutcome); // Record outcome
    playerState->setChapterNumber(chapter + 1); // Advance to next chapter
    playerState->setChapterEvent(1); // Reset event to 1
    playerState->resetCurrentDecisions(); // Reset decisions

    cout << "\nChapter " << chapter << " completed.\n";
    saveProgress(user); // Save progress
    cout << "Progress Saved! Press Enter to continue...";
    cin.ignore();
    cin.get();

    clearStoryScreen(); // Clear screen
    displayChapter(playerState->getChapterNumber()); // Show next chapter
    storyManager(user); // Continue story manager
}

// Displays the chapter introduction and story
void displayChapter(int chapterNumber) {
    int displayChapterNumber = chapterNumber;
    if (chapterNumber == 7) {
        displayChapterNumber = 6;  // Always show chapter 6 instead of 7
    }

    string story = getStoryData(chapterNumber, 0, "story"); // Get story text
    string name = "[Chapter Title Missing]";
    try {
        name = storyData[to_string(chapterNumber)]["name"].get<string>(); // Get chapter name
    }
    catch (...) {}

    clearStoryScreen(); // Clear screen
    cout << "\n===== Chapter " << displayChapterNumber << ": " << name << " =====\n";
    cout << story << "\n\n";
    cout << "Press Enter to continue...";
    cin.get();
    clearStoryScreen(); // Clear screen after input
}

// Calculates the outcome for a chapter based on player decisions
int outcomeAlgorithm(PlayerState& ps) {
    int counts[4] = { 0 }; // Count outcomes (1-based)
    for (int i = 0; i < 6; i++) {
        int val = ps.getCurrentDecision(i); // Get each decision
        if (val >= 1 && val <= 3)
            counts[val]++; // Count each outcome
    }

    int maxOutcome = 1; // Start with outcome 1
    for (int i = 2; i <= 3; i++) {
        if (counts[i] > counts[maxOutcome])
            maxOutcome = i; // Find most frequent outcome
    }
    return maxOutcome; // Return most common outcome
}

// Displays the final outcome to the player
void showUltimateOutcome(int outcomeId) {
    clearStoryScreen(); // Clear screen
    cout << "\n===== Final Outcome =====\n\n";

    switch (outcomeId) {
    case 1:
        cout << "Arin emerged as a mighty ruler after winning the War of Peace, forged in fire and loyalty. Eldora stands tall, united under his command.\n";
        cout << "The Empire's enemies bow in awe, and peace, built on valor, settles across the realm.\n";
        break;

    case 2:
        cout << "Arin chose wisdom over war. Through careful diplomacy and unseen sacrifice, he preserved the soul of the empire.\n";
        cout << "Eldora thrives — not because of bloodshed, but because of vision and restraint at the cost of an Ultimate hero Arin who never saw the sun rise after the battle protecting Eldora.\n";
        break;

    case 3:
        cout << "Arin walked a lonely path — abandoned, misunderstood, and sacrificed. But his actions spared Eldora’s collapse.\n";
        cout << "Though history may forget his name, Eldora breathes today because of his silent endurance.\n";
        break;

    default:
        cout << "The outcome is unclear. Perhaps history will yet write a different ending...\n";
        break;
    }

    cout << "\nThank you for playing *Chronicles of Arithon*. Until the next reckoning...\n";
}

// Handles user request to exit the game
bool exitRequest(UserData& user) {
    cout << "\nAre you sure you want to exit? (y/n): ";
    char confirm;
    cin >> confirm;

    if (confirm == 'y' || confirm == 'Y') {
        saveProgress(user); // Save progress before exit
        cout << "Progress saved. Exiting game...\nType exit again to exit the console.";
        return true; // Confirm exit
    }

    return false; // Do not exit
}

// Retrieves story or event description from JSON data
string getStoryData(int chapterNumber, int eventNumber, const string& dataType) {
    string result;
    try {
        if (dataType == "story") {
            result = storyData[to_string(chapterNumber)]["story"].get<string>(); // Get chapter story
        }
        else if (dataType == "description") {
            result = storyData[to_string(chapterNumber)]["events"][to_string(eventNumber)]["description"].get<string>(); // Get event description
        }
    }
    catch (...) {
        result = "[Data not found]"; // On error, return default
    }
    return result;
}

// Retrieves outcome or next event ID from JSON data based on choice
int getGameData(int chapterNumber, int eventNumber, char choice, const string& dataType) {
    try {
        const auto& choices = storyData[to_string(chapterNumber)]["events"][to_string(eventNumber)]["choices"];
        for (const auto& c : choices) {
            if (c["option"].get<string>()[0] == choice) { // Match choice
                return c[dataType].get<int>(); // Return requested data
            }
        }
    }
    catch (...) {
        return 0; // On error, return 0
    }
    return 0; // Default return
}

// Saves the user's progress to file
void saveProgress(UserData& user) {
    saveUser(user); // Call save function
}