#pragma once

#define SLEEP_ONE 1 //1 second sleep
#define SLEEP_TWO 2 //1 second sleep
#define SLEEP_THREE 3 //3 second sleep
#define SLEEP_FIVE 5 //5 second sleep


enum Lines{ONE = 1, TWO, THREE, FOUR, FIVE};


using namespace std;

namespace Arithon {
	void lineBreak(Lines lines);
	void Break();
	void sleep(long long time);
	void line();
	void loading(long long time);
}

int randomNumber(int min, int max);



