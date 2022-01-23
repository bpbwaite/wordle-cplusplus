#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <chrono>
#include <thread>

#include "controlstrings.h"
#include "splashfont.h"

using namespace std;

void printSplash() {
	cout << clsr;
	for (int k = 0; k < splash.size(); k++) {
		cout << color::blu << splash.at(k).c_str() << endl;
	}
	cout << color::wht << water << "C++ Implementation by Bradyn Braithwaite 2022" << endl;
	std::this_thread::sleep_for(std::chrono::milliseconds(2000)); // show for a bit
	cout << clsr;
}
inline bool stringContains(string s, char c) {
	for (int j = 0; j < s.length(); j++) {
		if (s[j] == c) {
			return true;
		}
	}
	return false;
}
void printGameBoard(string answer, string prev[], int max) {
	int row;
	string posedCorrectly = {};
	for (row = 0; row < max; row++) {
		if (prev[row].empty())
			break; // leave color loop and go print the empty cells
		cout << endl << blocktab;

		string inRow = {};
		// complex relatability computation:
		for (int col = 0; col < answer.length(); col++) {
			int relatability = 0;
			char curchar = prev[row][col];

			if (stringContains(answer, curchar) && !stringContains(inRow, curchar)) { // duplicate yellows-prevention
				relatability = 4; // likely out of place; but need to make sure there is not a duplicate IN place
				inRow += curchar;
			}
			if (curchar == answer[col]) {
				relatability = 2;
				posedCorrectly += curchar; // reusing this part of the loop
			}
			if (relatability == 4) {
				relatability = 1;
				// look through the word again, if any character in place
				// in a different position, but not our character,
				// not intereseted in revealing anything
				for (int k = 0; k < answer.length(); k++) {
					// duplicated where both are out of place
					if (prev[row][k] == answer[k] && (k != col) && answer[k] == curchar) {
						relatability = 0;
						break;
					}
				}
			}

			// begin printing as we go:
			curchar = (char)toupper(curchar);
			switch (relatability) {
			case 0: // guessed
				cout << color::gry << "[ " << color::wht << curchar << color::gry << " ]";
				break;
			case 1: // exists
				cout << color::gry << "[ " << color::ylw << curchar << color::gry << " ]";
				break;
			case 2:// perfect
			default:
				cout << color::gry << "[ " << color::grn << curchar << color::gry << " ]";
				break;
			}
		}
	}
	// print everything else empty:
	for (; row < max; row++) {
		cout << endl << blocktab;
		for (int col = 0; col < answer.length(); col++) {
			cout << color::gry << "[   ]";
		}
	}

	// break between the game board and key board below
	cout << color::wht << endl;
	const string qwerty = { "QWERTYUIOPASDFGHJKLZXCVBNM" };
	const string qwertytolower = { "qwertyuiopasdfghjklzxcvbnm" };
	int alphaGuess[26] = {}; // zero out the keyboard

	int n = 0;
	while (n < max) {
		if (prev[n].empty()) // no need to compare with empty strings
			break;
		//cout << prev[n]; // if debugging
		// apply grey mask to all letters that have been guessed in the prev set
		for (int a = 0; a < qwerty.length(); a++) {
			if (stringContains(prev[n], qwertytolower[a])) 
				alphaGuess[a] = 1;
			if (stringContains(answer, qwertytolower[a]) && stringContains(prev[n], qwertytolower[a])) 
				alphaGuess[a] = 2;
			if (stringContains(posedCorrectly, qwertytolower[a])) 
				alphaGuess[a] = 3;
		}
		n++;
	}

	cout << endl << keytab;
	for (int c = 0; c < qwerty.length(); c++) {
		if (c == 10) {
			cout << endl << keytab << ' ';
		}
		else if (c == 19) {
			cout << endl << keytab /*<<"[] "*/; // 'enter key' option
			cout << "   ";
		}

		switch (alphaGuess[c]) {
		case 0: // unknown
			cout << color::wht << qwerty[c] << ' ' << color::wht;
			break;
		case 1: // guessed
			cout << color::gry << qwerty[c] << ' ' << color::wht;
			break;
		case 2: // exists
			cout << color::ylw << qwerty[c] << ' ' << color::wht;
			break;
		case 3: // perfect
		default:
			cout << color::grn << qwerty[c] << ' ' << color::wht;
			break;
		}
	}
	cout /* << "[]"*/ << endl << endl; // 'backspace key' option
}
bool filecontains(std::fstream& file, std::string s, int lines) {
	if (!file.good()) {
		cerr << color::red << "WARN: Filehelper Error!" << endl;
		return false;
	}
	string buf;
	for (int wdx = 1; wdx <= lines; wdx++) {
		std::getline(file, buf);
		// std::cout << "DEBUG: FileHelper got " << buf << endl;
		if (buf == s) {
			file.seekg(0, ios_base::beg); // reset stream buffer offset
			return true;
		}
	}
	file.seekg(0, ios_base::beg); // reset stream buffer offset
	return false;
}