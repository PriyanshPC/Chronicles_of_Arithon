#pragma once
#include <iostream>

using namespace std;


// *** FUNCTIONS ***

void Break();


// *** CLASSES ***
class Fighter {
protected:
	string name;
	int strength;

public:
	Fighter(string n) : name(n), strength(100) {}

	int NormalMove() {
		return 15;
	}

	int StrongMove() {
		int random = (rand() % 10 + -5);
		return random + 15;
	}

	int Turn() {
		bool MoveOn = false;
		int choice;

		while (!MoveOn) {
			cout << "\n1. Normal Attack" << endl;
			cout << "2. Strong Attack" << endl;
			cout << "What move would you like to do: ";
			cin >> choice;

			switch (choice) {
			case 1:
				return choice;
				MoveOn = true;
			case 2:
				return choice;
				MoveOn = true;
				break;
			default:
				break;
			}
		}
	}

	void PrintHealth() {
		cout << "Player Health: " << strength << endl;
	}

	int MoveChoice(int choice) {

		if (choice == 1) {
			int x = NormalMove();
			cout << "Attack Did " << x << " Damage!" << endl;
			return x;
		}
		else if (choice == 2) {
			int x = StrongMove();
			cout << "Attack Did " << x << " Damage!" << endl;
			return x;
		}
	}

	void SetStrength(int damage) {
		strength = strength - damage;
	}

	int GetStrength() {
		return strength;
	}
};

class CompFighter : public Fighter {


public:
	CompFighter(string n) : Fighter(n) {}

	int SelectMove() {
		return rand() % 2 + 1;
	}

	void PrintHealth() {
		if (strength < 0) {
			strength = 0;
		}

		cout << "Computer Health: " << strength << endl;
	}

	int Turn() {
		return rand() % 2 + 1;
	}

	int MoveChoice(int choice) {

		if (choice == 1) {
			cout << "Opponent used Normal Attack!" << endl;

			int x = NormalMove();
			cout << "Opponents Attack Did " << x << " Damage!" << endl;
			return x;
		}
		else if (choice == 2) {
			cout << "Opponent used Random Attack!" << endl;
			int x = StrongMove();
			cout << "Opponents Attack Did " << x << " Damage!" << endl;
			return x;
		}
	}

};

class battle
{

public:
	bool isAlive(Fighter fighter) {
		return fighter.GetStrength() > 0;
	}
	void Fight(Fighter player, CompFighter comp) {

		bool fight = true;

		while (fight) {
			comp.SetStrength(player.MoveChoice(player.Turn()));
			if (!isAlive(comp)) {
				cout << "You Win" << endl;
				fight = false;
				break;
			}


			player.SetStrength(comp.MoveChoice(comp.Turn()));
			player.PrintHealth();
			comp.PrintHealth();
			if (!isAlive(player)) {
				cout << "You Lose" << endl;
				fight = false;
				break;
			}
			Break();
		}

		cout << "\n Fight Over!" << endl;
	}
};

