#include <iostream>
#include "battle.h"

using namespace std;

random_device rd;
std::mt19937 gen(rd());

void Break() {
	cout << "\n\nPress Enter to Continue ..." << endl;
	cin.ignore();
	cin.get();
}

int randomNumber(int min, int max) {

	uniform_int_distribution<int> dist(min, max);
	return dist(gen);
}