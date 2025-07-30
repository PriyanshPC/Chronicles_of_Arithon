#include <iostream>
#include <chrono>
#include <thread>
#include "globals.h"

namespace Arithon {
	void lineBreak(Lines lines) {
		for (int i = 0; i < lines; i++) {
			cout << "\n";
		}
	}
	
	void Break() {
		cout << "\n\nPress Enter to Continue ..." << endl;
		cin.ignore();
		cin.get();
	}

	void sleep(long long time) {
		this_thread::sleep_for(chrono::seconds(time));
	}

	void line() {
		cout << "==================================================" << endl;
	}

	void loading() {
		cout << "Loading";
		Arithon::sleep(SLEEP_ONE);
		for (int i = 0; i < 3; i++) {
			cout << ".";
			Arithon::sleep(SLEEP_ONE);
		}
		
	}
}
