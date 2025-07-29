// Chronicles_of_Arithon.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "FileManager.h"

using namespace std;

int main()
{
    std::cout << "GAME START!\n";

    FileSystem file;

    if (!file.checkForFile()) { // if file is not found
        exit(EXIT_FAILURE); // exit with error
    }

    cout << "We have a save file named: " << file.readUsername() << endl;
    cout << " Would you like to use this File? (Y/N): ";

    char answer;
    cin >> answer;
    
    switch (tolower(answer)) {
    case 'y':
        break;
    case 'n':
        cout << "Ok Bye!";
        return 0;
    default:
        perror("Invalid entry");
        exit(EXIT_FAILURE);
    }
      
    cout << file.readPassword() << endl;

    string temp;
    cout << "enter Password: ";
    cin >> temp;
    if (file.checkPassword(temp)) {
        cout << "\nlogged in!" << endl;
    }

    else {
        cout << "Wrong password" << endl;
        return 1;
    }
         
    file.loadFile();

    playerStats player;
    file.saveFile(player);

    

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
