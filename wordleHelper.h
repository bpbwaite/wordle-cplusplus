/********************************************************************************
 * File:      wordleHelper.h
 * Author:    Bradyn Braithwaite
 * Purpose:	  supply supplemental functions to the game
 ********************************************************************************/
#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include "splashtext.h"

using namespace std;

constexpr int displayCenter = 60;

void printSplash()
{
	cout << clearscreen;
	for (unsigned ln = 0; ln < Splash.size(); ln++) {
		cout << color::lightblue << Splash.at(ln).c_str() << endl;
	}
}
void offsetDisplayToCenter(int linelength, int charwidth = 1, bool newline = true)
{
	if(newline)
	cout << endl;
	for (int spx = 0; spx < (displayCenter - charwidth * linelength / 2); spx++) {
		cout << " ";
	}
}
bool stringContains(string s, char c)
{
	for (unsigned j = 0; j < s.length(); j++) {
		if (s[j] == c) {
			return true;
		}
	}
	return false;
}
bool fileContains(std::fstream& file, string s, int lines)
{
	// reset stream offset
	file.seekg(0, ios_base::beg);
	if (!file.good()) {
		cerr << color::red << "Filehelper Error!" << endl;
		return false;
	}

	string buf;
	bool found = false;
	for (int wdx = 1; !found && wdx <= lines; wdx++) {
		std::getline(file, buf);
		found = buf == s;
	}
	return found;
}
void outputGame(string answer, std::vector<std::string> Entries)
{
	int nth_word;
	int max = Entries.size();

	// ARRAY PORTION
	string posedCorrectly;
	for (nth_word = 0; nth_word < max; nth_word++) {
		if (Entries.at(nth_word).empty())
			break; // leave to go print the empty cells

		offsetDisplayToCenter(answer.length(), 5);
		string uniqueCharsInRow;
		// character relatability section
		for (unsigned chdx = 0; chdx < answer.length(); chdx++) {
			int relatability = 0;
			char curchar = Entries.at(nth_word)[chdx];

			if (stringContains(answer, curchar) && !stringContains(uniqueCharsInRow, curchar)) {
				relatability = 3;
				uniqueCharsInRow += curchar;
			}
			if (curchar == answer[chdx]) {
				relatability = 2;
				posedCorrectly += curchar;
			}
			if (relatability == 3) {
				relatability = 1;
				// look ahead through the word, see if our character is in place in a different position
				for (unsigned acdx = 0; acdx < answer.length(); acdx++) {
					if (acdx != chdx && Entries.at(nth_word)[acdx] == answer[acdx] && answer[acdx] == curchar) {
						relatability = 0;
						break;
					}
				}
			}

			curchar = (char)toupper(curchar);
			cout << color::grey << "[ ";
			switch (relatability) {
			case 0: // touched
				cout << color::white;
				break;
			case 1: // exists in word
				cout << color::yellow;
				break;
			case 2: // perfect
				cout << color::green;
				break;
			default: break;
			}
			cout << curchar << color::grey << " ]";
		}
	}

	while (nth_word < max) {
		offsetDisplayToCenter(answer.length(), 5);
		for (unsigned gdx = 0; gdx < answer.length(); gdx++) {
			cout << color::grey << "[   ]";
		}
		nth_word++;
	}

	// KEYBOARD PORTION
	cout << color::white << endl << endl << keytab;
	constexpr int keyboard_size = 26;
	const string keyboard_lowercase = { "qwertyuiopasdfghjklzxcvbnm" };
	const string keyboard_uppercase = { "QWERTYUIOPASDFGHJKLZXCVBNM" };
	int qwerty_colors[keyboard_size] = {};

	nth_word = 0;
	while (nth_word < max) {
		if (Entries.at(nth_word).empty())
			break; // no need to compare with empty strings

		for (int acdx = 0; acdx < keyboard_size; acdx++) {
			if (stringContains(Entries.at(nth_word), keyboard_lowercase[acdx]))
				qwerty_colors[acdx] = 1;
			if (stringContains(answer, keyboard_lowercase[acdx]) && stringContains(Entries.at(nth_word), keyboard_lowercase[acdx]))
				qwerty_colors[acdx] = 2;
			if (stringContains(posedCorrectly, keyboard_lowercase[acdx]))
				qwerty_colors[acdx] = 3;
		}
		nth_word++;
	}

	for (int acdx = 0; acdx < keyboard_size; acdx++) {
		if (acdx == 10)
			cout << endl << keytab << ' ';
		else if (acdx == 19)
			cout << endl << keytab << "   ";

		switch (qwerty_colors[acdx]) {
		case 0: // unknown
			cout << color::white << keyboard_uppercase[acdx];
			break;
		case 1: // touched
			cout << color::black << keyboard_uppercase[acdx];
			break;
		case 2: // exists in word
			cout << color::yellow << keyboard_uppercase[acdx];
			break;
		case 3: // perfect
			cout << color::green << keyboard_uppercase[acdx];
			break;
		default: break;
		}
		cout << ' ' << color::white;
	}
	cout << endl;
}
void scrapeEnglishWordsToFile(int length)
{
	string buf;
	string ofilename = "words" + to_string(length) + ".txt";
	cerr << color::red << "Running Dictionary Setup... ";
	fstream English("English.txt");
	fstream wordsfile(ofilename);
	if (!English.is_open() || !wordsfile.is_open())
		cerr << color::red << "Unable to open dictionary file!" << color::white << endl;
	while (!English.eof()) {
		std::getline(English, buf);
		if (buf.length() == length) {
			wordsfile << endl; // easily removable line
			wordsfile << buf;
		}
	}
	English.close();
	wordsfile.close();
	cout << "Done." << endl;
}
void scrapeCommonWordsToFile(int targetlength, int amount = 2300, bool solutionfile = true)
{
	string ofilename = "words";
	if (!solutionfile) {
		ofilename += to_string(targetlength) + ".txt";
	}
	else {
		ofilename += to_string(targetlength) + "sol.txt";
	}
	cerr << color::red << "Scraping... ";
	fstream Wiki("Wiki.txt");
	fstream solutionFile(ofilename, ios::out);
	if (!Wiki.is_open() || !solutionFile.is_open())
		cerr << color::red << "Unable to open dictionary file!" << color::white << endl;

	string linebuf;
	int ldx = 1;
	while (ldx <= amount && !Wiki.eof()) {
		string wordbuf("", targetlength);
		// temporary reserve
		char tempWordbuf[128];

		std::getline(Wiki, linebuf);
		unsigned chx;
		for (chx = 0; chx < linebuf.length(); chx++) {
			if (linebuf[chx] == ' ')
				break;
			else tempWordbuf[chx] = linebuf[chx];
		}
		tempWordbuf[chx] = '\0'; // nullify
		// copy per-character
		if (strlen(tempWordbuf) == targetlength) {
			// mirror behavior of not using plurals in solutions (for long words)
			if (targetlength < 5 || tempWordbuf[targetlength - 1] != 's') {
				for (chx = 0; chx < targetlength; chx++) {
					wordbuf[chx] = tempWordbuf[chx];
				}
				solutionFile << endl << wordbuf;
				ldx++;
			}
			
		}
	}
	Wiki.close();
	solutionFile.close();
	cout << color::red << "Done." << color::white << endl;
}