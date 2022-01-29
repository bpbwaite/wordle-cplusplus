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
	// DEFINITIONS & PRECONFIGURATION
	// unique solutions for 3/4/5/6/7/8. these values work well
	const int SOLUTIONS_PER_GAME[] = { 120, 900, 1400, 2000, 3200, 2400 };
	const bool REBUILD_SOLUTIONS = false; // set to true for one run if changing solutions

	const int MODE_NORMAL = 0;

	const int MODE_BABY = 1;
	const int MODE_EASY = 2;
	const int MODE_HARD = 3;
	const int MODE_EXPERT = 4;
	const int MODE_INSANE = 5;

	int diffdefs[] = { 6, 5, 6, 3, 7, 4, 7, 6, 8, 7, 9, 8 }; // Guesses,Wordlength: for 3-8 letters
	int mode = MODE_NORMAL;
	int streak = 0;
	bool colorblind = false;

	if (REBUILD_SOLUTIONS) {
		for (int wordLengths = 3; wordLengths <= 8; wordLengths++) {
			scrapeEnglishWordsToFile(wordLengths);
			scrapeCommonWordsToFile(wordLengths, SOLUTIONS_PER_GAME[wordLengths - 3]);
		}
	}

	// MAIN LOOP
	while (!REBUILD_SOLUTIONS) {
		bool modechange = false;
		int maxGuesses = diffdefs[mode * 2];
		int wordleLength = diffdefs[mode * 2 + 1];

		// FILE SETUP & WORD COUNTER
		string wordFileName = "words" + to_string(wordleLength) + ".txt";
		string solutionFileName = "words" + to_string(wordleLength) + "sol.txt";
		fstream AllWords(wordFileName, ios::in);
		fstream SolutionWords(solutionFileName, ios::in);

		if (!AllWords.is_open() || !SolutionWords.is_open()) {
			cerr << color::red << "Unable to open local dictionary files!" << color::white << endl;
			return -20;
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
			int inputErrorType = -1; // -1: NoRead, 0: OK, 1: InvalChar, 2: WrongLen, 3: AlreadyExist, 4: NotWord, 5: CinErr, 6: Debug, 7: Color

			// USER INPUT LOOP
			while (!modechange && inputErrorType != 0) {

				cout << endl << blocktab << " > ";
				cin >> userInput;
				inputErrorType = 0;
				// initially assume the input is good, cascadingly check each scenario

				for (unsigned c = 0; c < userInput.length(); c++) {
					if (!islower(userInput[c]))
						userInput[c] = tolower(userInput.c_str()[c]);
					if (userInput[c] < int('a') || userInput[c] > int('z')) {
						inputErrorType = 1;
						break;
					}
				}

				if (!inputErrorType && userInput.length() != wordleOfTheDay.length())
					inputErrorType = 2;

				// check against previous inputs
				for (int wdx = 0; !inputErrorType && wdx < maxGuesses; wdx++)
					if (userInput == prevInputs[wdx])
						inputErrorType = 3;
				if (!cin) {
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					inputErrorType = 5;
				}
				else if (!inputErrorType && !fileContains(SolutionWords, userInput, numSolutions) && !fileContains(AllWords, userInput, numWords))
					inputErrorType = 4;

				// check special cases
				if (userInput == wordleOfTheDay)
					guessedCorrect = true;
				else if (userInput == "debugmode")
					inputErrorType = 6;
				else if (userInput == "colorblindmode") 
					inputErrorType = 7;
				
				else if (userInput == "babymode") {
					modechange = true;
					mode = MODE_BABY;
				}
				else if (userInput == "easymode") {
					modechange = true;
					mode = MODE_EASY;
				}
				else if (userInput == "normalmode") {
					modechange = true;
					mode = MODE_NORMAL;
				}
				else if (userInput == "hardmode") {
					modechange = true;
					mode = MODE_HARD;
				}
				else if (userInput == "expertmode") {
					modechange = true;
					mode = MODE_EXPERT;
				}
				else if (userInput == "insanemode") {
					modechange = true;
					mode = MODE_INSANE;
				}
				if (modechange)
					inputErrorType = -1;
				switch (inputErrorType) {
				case 0:
					prevInputs[guess] = userInput;
					guess++; // got a good input
					break;
				case 1:
					cout << color::red << blocktab;
					cout << "You may only enter letters." << color::white;
					break;
				case 2:
					cout << color::red << blocktab;
					cout << "You may only enter " << wordleOfTheDay.length() << " letter words." << color::white;
					break;
				case 3:
					cout << color::red << blocktab;
					cout << "Already entered." << color::white;
					break;
				case 4:
					cout << color::red << blocktab;
					cout << "Not in word list." << color::white;
					break;
				case 6:
					cout << color::red << blocktab;
					cout << "Debug: '" << wordleOfTheDay << "'" << color::white;
					break;
				case 7:
					colorblind = !colorblind;
					cout << blocktab << "ColorBlind: " << (colorblind ? "on" : "off");
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
	}

	return 0;
}
