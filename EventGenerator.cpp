#include "StoryEngine.h" // Include the StoryEngine header for story data access
#include "FileManager.h" // Include the FileManager header for file operations
#include "Authenticator.h" // Include the Authenticator header for user authentication
#include "PlayerState.h" // Include the PlayerState header for player state management
#include "EventGenerator.h" // Include the EventGenerator header for function declarations
#include <iostream> // Include for input/output stream operations
#include <fstream> // Include for file stream operations
#include <string> // Include for string operations
#include <limits> // Include for numeric limits

using namespace std; // Use the standard namespace

int optionOne = 0; // Index for the first choice option
int optionTwo = 1; // Index for the second choice option

// Displays an event, presents choices, and handles user input
char displayEvent(int chapterNumber, int eventNumber, UserData& user) {
    string description; // Holds the event description
    char choice; // Stores the user's choice

    cout << "Event: "; // Print event label
    description = storyData[to_string(chapterNumber)]["events"][to_string(eventNumber)]["description"].get<string>(); // Get event description from story data
    cout << description << "\n"; // Display event description

    string descA = storyData[to_string(chapterNumber)]["events"][to_string(eventNumber)]["choices"][optionOne]["description"].get<string>(); // Get description for choice A
    string descB = storyData[to_string(chapterNumber)]["events"][to_string(eventNumber)]["choices"][optionTwo]["description"].get<string>(); // Get description for choice B

    cout << "A: " << descA << "\n"; // Display choice A
    cout << "B: " << descB << "\n"; // Display choice B

    while (true) { // Loop until valid input is received
        cout << "Enter your choice (A/B or X to exit): "; // Prompt user for input
        cin >> choice; // Read user input
        choice = toupper(choice); // Convert input to uppercase

        if (choice == 'A') { // If user chose A
            cout << "\nYou chose option A: " << descA << "\n\n"; // Confirm choice
            return choice; // Return choice
        }
        else if (choice == 'B') { // If user chose B
            cout << "\nYou chose option B: " << descB << "\n\n"; // Confirm choice
            return choice; // Return choice
        }
        else if (choice == 'X') { // If user wants to exit
            char confirm; // Variable for exit confirmation
            while (true) { // Loop until valid confirmation
                cout << "Are you sure you want to exit? (y/n): "; // Prompt for confirmation
                cin >> confirm; // Read confirmation
                confirm = toupper(confirm); // Convert to uppercase
                if (confirm == 'Y') { // If confirmed
                    if (exitRequest(user)) exit(0); // Call exitRequest and exit if true
                    return displayEvent(chapterNumber, eventNumber, user); // Otherwise, redisplay event
                }
                else if (confirm == 'N') { // If not confirmed
                    break; // Break confirmation loop
                }
                else { // Invalid confirmation input
                    cout << "Invalid input. Please enter Y or N.\n"; // Prompt for valid input
                    cin.clear(); // Clear error state
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
                }
            }
        }
        else { // Invalid choice input
            cout << "Invalid choice. Please enter A, B, or X.\n"; // Prompt for valid input
            cin.clear(); // Clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
        }
    }
}

// Handles a combat encounter between the player and an enemy
char combat(EnemyState enemy, int weaponDamage) {
    int healAmount = 20; // Amount healed per potion
    int playerHP = 100; // Player's starting HP
    int playerMaxHP = 100; // Player's maximum HP
    int healthPotions = 3; // Number of health potions player has
    bool playerDefend = false; // Whether player is defending
    bool playerAttack = false; // Whether player is attacking
    int playerDamage = weaponDamage; // Player's attack damage
    int action; // Player's chosen action

    int enemyHP = enemy.enemyHP; // Enemy's current HP
    int enemyMaxHP = enemy.enemyMaxHP; // Enemy's maximum HP
    int enemyPotions = enemy.enemyPotions; // Enemy's health potions
    int enemyDamage = enemy.enemyDamage; // Enemy's attack damage
    int enemyAction = enemy.enemyAction; // Enemy's chosen action
    bool enemyAttack = enemy.enemyAttack; // Whether enemy is attacking

    cout << "========================================\n"; // Print combat header
    cout << "||      Get Ready for the Combat      ||\n";
    cout << "========================================\n\n";

    while (enemyHP > 0 && playerHP > 0) { // Continue combat while both are alive
        cout << "Your HP: " << playerHP << "\n"; // Show player's HP
        cout << "Enemy HP: " << enemyHP << "\n"; // Show enemy's HP

        while (true) { // Loop until valid action is chosen
            cout << "1. Attack\n2. Defend\n3. Heal\nChoose your action (1/2/3): "; // Present action options
            if (cin >> action && (action >= 1 && action <= 3)) { // Read and validate input
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore extra input
                break; // Valid input, break loop
            }
            else { // Invalid input
                cout << "Invalid input. Please enter 1, 2, or 3.\n"; // Prompt for valid input
                cin.clear(); // Clear error state
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            }
        }

        if (action == 1) playerAttack = true; // Set attack flag if chosen
        else if (action == 2) { // If defend chosen
            playerDefend = true; // Set defend flag
            cout << "You chose to defend!\n"; // Confirm action
        }
        else if (action == 3) { // If heal chosen
            if (playerHP == playerMaxHP) { // If already at max HP
                cout << "You are already at full HP!\n"; // Inform player
                continue; // Skip to next turn
            }
            else if (healthPotions > 0) { // If potions available
                playerHP = min(playerMaxHP, playerHP + healAmount); // Heal player
                healthPotions--; // Use a potion
                cout << "You healed yourself for " << healAmount << " HP!\n"; // Inform player
            }
            else { // No potions left
                cout << "No health potions left!\n"; // Inform player
                continue; // Skip to next turn
            }
        }

        while (enemyAttack) { // Enemy's turn to act
            enemyAction = rand() % 3 + 1; // Randomly choose enemy action (1-3)

            if (enemyAction == 1) { // Enemy attacks
                if (!playerDefend) { // If player is not defending
                    if (playerAttack) { // If player attacked
                        enemyHP = max(0, enemyHP - playerDamage); // Deal damage to enemy
                        cout << "You attacked the enemy for " << playerDamage << " damage!\n"; // Inform player
                    }
                    playerHP = max(0, playerHP - enemyDamage); // Enemy deals damage to player
                    cout << "The enemy attacked you for " << enemyDamage << " damage!\n"; // Inform player
                }
                else { // Player defended
                    cout << "You defended against the enemy's attack!\n"; // Inform player
                }
                enemyAttack = false; // End enemy's turn
            }
            else if (enemyAction == 2) { // Enemy defends
                if (playerAttack) { // If player attacked
                    cout << "The enemy blocked your attack!\n"; // Inform player
                }
                else {
                    cout << "The enemy chose to defend.\n"; // Inform player
                }
                enemyAttack = false; // End enemy's turn
            }
            else if (enemyAction == 3 && enemyPotions > 0 && enemyHP < enemyMaxHP) { // Enemy heals
                if (playerAttack) { // If player attacked
                    enemyHP = max(0, enemyHP - playerDamage); // Deal damage to enemy
                    cout << "You attacked the enemy for " << playerDamage << " damage!\n"; // Inform player
                }
                enemyHP = min(enemyMaxHP, enemyHP + healAmount); // Heal enemy
                enemyPotions--; // Use a potion
                cout << "The enemy healed itself for " << healAmount << " HP!\n"; // Inform player
                enemyAttack = false; // End enemy's turn
            }
        }

        enemyAttack = true; // Reset enemy attack flag for next round
        playerAttack = false; // Reset player attack flag
        playerDefend = false; // Reset player defend flag
        cout << "\n"; // Print newline for readability
    }

    if (playerHP <= 0) { // If player is defeated
        cout << "You have been defeated in combat.\n"; // Inform player
        return 'B'; // Return failure
    }
    else { // If enemy is defeated
        cout << "You have defeated the enemy!\n"; // Inform player
        return 'A'; // Return success
    }
}

// Presents a puzzle to the player and checks their answer
char puzzle() {
    clearStoryScreen(); // Clear the screen for the puzzle
    cout << "========================================\\n"; // Print puzzle header
    cout << "||        Ancient Pattern Lock        ||\\n";
    cout << "========================================\\n\\n";

    cout << "To unlock the sacred seal, solve this numeric sequence:\\n"; // Present puzzle instructions
    cout << "2, 6, 12, 20, 30, ?\\n"; // Show the sequence
    cout << "Enter the next number in the pattern: "; // Prompt for answer

    int attempts = 0; // Number of attempts made
    int input; // User's input
    const int correctAnswer = 42; // Correct answer to the puzzle

    while (attempts < 3) { // Allow up to 3 attempts
        if (!(cin >> input)) { // If input is not a number
            cin.clear(); // Clear error state
            cin.ignore(numeric_limits<streamsize>::max(), '\\n'); // Ignore invalid input
            cout << "Invalid input. Please enter a number.\\n"; // Prompt for valid input
            continue; // Retry
        }

        if (input == correctAnswer) { // If answer is correct
            cout << "\\nThe lock clicks and ancient gears move. The path opens!\\n"; // Success message
            return 'A'; // Return success
        }
        else { // Incorrect answer
            attempts++; // Increment attempt count
            if (attempts == 1)
                cout << "Incorrect. Hint: Differences between numbers are increasing.\\n"; // First hint
            else if (attempts == 2)
                cout << "Still wrong. Think of how the gap changes between numbers.\\n"; // Second hint
            else
                cout << "The mechanism resets. You failed to unlock the seal.\\n"; // Failure message
        }
    }

    return 'B';  // failed puzzle
}
