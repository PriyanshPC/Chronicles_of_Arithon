#pragma once
#include <iostream>
#include <random>
#include "globals.h"


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
		return randomNumber(-1, 10) + 15;

	}

	int Turn() {
		bool MoveOn = false;
		int choice;

		while (!MoveOn) {
			cout << "\n1. Normal Attack" << endl;
			cout << "2. Random Attack" << endl;
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

	string getName() {
		return name;
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
		return randomNumber(1, 2);
	}

	void PrintHealth() {
		if (strength < 0) {
			strength = 0;
		}

		cout << "Computer Health: " << strength << endl;
	}

	int Turn() {
		return randomNumber(1, 2);
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

		cout << "You encountered " << comp.getName() << ", You must fight!" << endl;

		while (fight) {
			comp.SetStrength(player.MoveChoice(player.Turn()));
			if (!isAlive(comp)) {
				cout << "You Beat " << comp.getName() << endl;
				fight = false;
				break;
			}

			Arithon::sleep(SLEEP_ONE);

			cout << endl;
			player.SetStrength(comp.MoveChoice(comp.Turn()));
			cout << endl;

			Arithon::sleep(SLEEP_ONE);

			player.PrintHealth();
			comp.PrintHealth();

			if (!isAlive(player)) {
				cout << "You Lost to " << comp.getName() << endl;
				fight = false;
				break;
			}
			Arithon::Break();
		}

		cout << "\n Fight Over!" << endl;
	}
};

void startBattle();