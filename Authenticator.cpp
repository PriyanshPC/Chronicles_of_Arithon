#include <iostream>

using namespace std;

string encrypt(const string& input) {
	string output = input;
	for (char& c : output) {
		if (c == 'z') {
			c = 'b';
		}
		else if (c == 'Z') {
			c = 'B';
		}
		else if (c == 'y') {
			c = 'a';
		}
		else if (c == 'Y') {
			c = 'A';
		}
		else {
			c = c + 2;
		}
	}
	return output;
}

string decrypt(const string& input) {
	string output = input;
	for (char& c : output) {
		if (c == 'b') {
			c = 'z';
		}
		else if (c == 'B') {
			c = 'Z';
		}
		else if (c == 'a') {
			c = 'y';
		}
		else if (c == 'A') {
			c = 'Y';
		}
		else {
			c = c - 2;
		}
	}
	return output;
}