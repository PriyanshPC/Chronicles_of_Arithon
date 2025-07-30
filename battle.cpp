#include <iostream>
#include "battle.h"

using namespace std;

random_device rd;
std::mt19937 gen(rd());

int randomNumber(int min, int max) {
	uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}

void startBattle() {
    cout << "*** BATTLE TIME ***\n" << endl;
    Fighter plyr("player");
    CompFighter comp("Beast");

    battle battleOne;
    battleOne.Fight(plyr, comp);
}