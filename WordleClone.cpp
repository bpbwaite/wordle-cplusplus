/********************************************************************************
 * File:     Homework 3
 * Author:   Bradyn Braithwaite
 * Purpose:	 Recreate the Wordle minigame in C++
 * Version:  2.3 Jan 24, 2022
 * Resources: frequency lists, occasionally documentation for file io
 ********************************************************************************/

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <ctime>
#include <climits>

#include "wordleHelper.h"

using namespace std;

int main(void)
{
	int wordleLength = MODE_L5;
	int streak = 0;
	bool colorblind = false;
	bool hardmode = false;

	if (REBUILD_SOLUTIONS) {
		rebuildSolutions();
		return EXIT_DICTIONARY_OK;
	}

	// MAIN LOOP
	do {
		bool modechange = false;
		int maxGuesses = difficulties[wordleLength];

		// FILE SETUP & WORD COUNTER
		string wordFileName = "words" + to_string(wordleLength) + ".txt";
		string solutionFileName = "words" + to_string(wordleLength) + "sol.txt";
		fstream AllWords(wordFileName, ios::in);
		fstream SolutionWords(solutionFileName, ios::in);

		if (!AllWords.is_open() || !SolutionWords.is_open()) {
			cerr << color::red << "Unable to open local dictionary files!" << color::white << endl;
			return EXIT_FAILURE;
		}
		string buf;
		int numSolutions = 0;
		int numWords = 0;
		while (!AllWords.eof()) {
			std::getline(AllWords, buf);
			numWords++;
		}
		while (!SolutionWords.eof()) {
			std::getline(SolutionWords, buf);
			numSolutions++;
		}
		srand((unsigned int)time(NULL));

		// WORD GENERATION
		int wordleLine = rand() % numSolutions + 2; // line num in solution file
		string wordleOfTheDay = {};
		SolutionWords.seekg(0, ios_base::beg);
		for (int wdx = 1; wdx != wordleLine; wdx++) {
			std::getline(SolutionWords, wordleOfTheDay);
		}
		removeNewLineChars(wordleOfTheDay);
		// GAME SETUP
		std::vector<std::string> prevInputs(maxGuesses);
		int guess = 0;
		bool guessedCorrect = false;

		// GAME LOOP
		while (!modechange && guess < maxGuesses && !guessedCorrect) {

			printSplash(); // cool intro
			outputGame(wordleOfTheDay, prevInputs, colorblind);

			offsetDisplayToCenter(32);
			cout << "Guess #" << guess + 1 << ": Enter a" << ((wordleOfTheDay.length() == 8) ? "n " : " ")
				<< wordleOfTheDay.length() << " letter word:";

			string userInput = "";
			int inputErrorType = WER_NOREAD; 

			// USER INPUT LOOP
			while (!modechange && inputErrorType != WER_OK) {

				cout << endl << blocktab << " > ";
				cin >> userInput;
				inputErrorType = WER_OK;
				// initially assume the input is good, cascadingly check each scenario

				for (unsigned c = 0; c < userInput.length(); c++) {
					if (!islower(userInput[c]))
						userInput[c] = tolower(userInput.c_str()[c]);
					if (userInput[c] < int('a') || userInput[c] > int('z')) {
						inputErrorType = WER_INVCHAR;
						break;
					}
				}

				if (!inputErrorType && userInput.length() != wordleOfTheDay.length())
					inputErrorType = WER_WLEN;

				// check against previous inputs
				for (int wdx = 0; !inputErrorType && wdx < maxGuesses; wdx++)
					if (userInput == prevInputs[wdx]) {
						inputErrorType = WER_EXIST;
						break;
					}
				if (!cin) {
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					inputErrorType = WER_CIN;
				}
				else if (hardmode && !validInHardMode(prevInputs, userInput, wordleOfTheDay))
					inputErrorType = WER_MUSTUSE;
				else if (!inputErrorType && !fileContains(SolutionWords, userInput, numSolutions) && !fileContains(AllWords, userInput, numWords))
					inputErrorType = WER_DICT;

				// check special cases
				if (userInput == wordleOfTheDay)
					guessedCorrect = true;
				if (userInput == "debugme")
					inputErrorType = WER_DEBUG;
				if (userInput == "colorblindmode")
					inputErrorType = WER_COLOR;
				if (userInput == "hardmode")
					inputErrorType = WER_HMODE;
				
				// mode changing logic
				short usermodenum = atoi(userInput.c_str());
				if (usermodenum >= MODE_L3 && usermodenum <= MODE_L8) {
					modechange = true;
					wordleLength = usermodenum;
				}
				if (modechange)
					inputErrorType = WER_MODECHANGE;
				
				switch (inputErrorType) {
				case WER_OK:
					prevInputs[guess] = userInput;
					guess++; // got a good input
					break;
				case WER_INVCHAR:
					cout << color::red << blocktab;
					cout << "You may only enter letters." << color::white;
					break;
				case WER_WLEN:
					cout << color::red << blocktab;
					cout << "You may only enter " << wordleOfTheDay.length() << " letter words." << color::white;
					break;
				case WER_EXIST:
					cout << color::red << blocktab;
					cout << "Already entered." << color::white;
					break;
				case WER_DICT:
					cout << color::red << blocktab;
					cout << "Not in word list." << color::white;
					break;
				case WER_DEBUG:
					cout << color::red << blocktab;
					cout << "Debug: '" << wordleOfTheDay << "'" << color::white;
					break;
				case WER_COLOR:
					colorblind = !colorblind;
					cout << blocktab << "ColorBlind: " << (colorblind ? "on" : "off");
					break;
				case WER_HMODE:
					hardmode = !hardmode;
					cout << blocktab << "Hard Mode: " << (hardmode ? "on" : "off");
					break;
				case WER_MUSTUSE:
					cout << color::red << blocktab;
					cout << "Hard Mode: Must reuse all revealed letters!" << color::white;
					break;
				default: break;
				}
			}
		}

		// POST-GAME
		if (!modechange) {
			printSplash();
			offsetDisplayToCenter(watermark.length(), 1, false);
			cout << watermark;
			outputGame(wordleOfTheDay, prevInputs, colorblind);
			if (guessedCorrect) {
				streak++;
				cout << endl << color::green << blocktab << "You got it!" << color::white << endl;
				cout << blocktab << "Streak: " << streak << endl;
			}
			else {
				cout << endl << color::yellow << blocktab << "Better luck next time!" << endl;
				if (streak > 0) {
					cout << blocktab << "You had a streak of " << streak << "!" << color::white << endl;
				}
				streak = 0;
			}
			cout << blocktab << "The word was '" << wordleOfTheDay << "'." << endl;

			// PLAY AGAIN WITH Y
			cout << blocktab << color::white << "Play again? (y): ";
			cin >> buf;
			if (!(buf[0] == 'y' || buf[0] == 'Y')) {
				cout << blocktab << "Thanks for playing!" << endl;
				break;
			}
		}

		// CLEANUP FOR NEXT
		AllWords.close();
		SolutionWords.close();

	} while (true);
	return EXIT_SUCCESS;
}
