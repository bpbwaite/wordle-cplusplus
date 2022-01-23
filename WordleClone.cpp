/********************************************************************************
 * File:     Homework 3
 * Author:   Bradyn Braithwaite
 * Purpose:	 Recreate the Wordle minigame in C++
 * Version:  1.2 Jan 22, 2022
 * Resources: copied Wordle's dictionary from their website lol
 ********************************************************************************/

#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iostream>
#include <string>

using namespace std;

// EXTERNAL FUNCTIONS

#include "splashfont.h"
#include "wordleHelper.h"

int main(void) {
	// configuration: set number of attempts allowed
	const int maxguesses = 6;

	// FILE SETUP
	cout << "Loading..." << endl;
	fstream allwords("WordleWords.txt", ios::in);
	fstream solutionwords("WordleSolutions.txt", ios::in);
	if (!allwords.is_open() || !solutionwords.is_open()) {
		cerr << color::red << "Unable to open dictionary files!" << color::wht << endl;
		return -2;
	}
	string buf;
	solutionwords >> buf; // read the indicator on line 1
	const int numSolutions = atoi(buf.c_str());
	allwords >> buf; // read the indicator on line 1
	const int numWords = atoi(buf.c_str());
	cout << "Loaded " << numSolutions + numWords << " words!" << endl;
	srand((unsigned int)time(NULL));

	// MAIN LOOP
	while (true) { // infinite so long as someone wants to play

		printSplash(); // cool intro

		// WORD GENERATION
		int wordleLine = rand() % numSolutions + 1; // wordle location (line num) in solution file
		string wordleOfTheDay;
		for (int wdx = 1; wdx != wordleLine; wdx++) {
			std::getline(solutionwords, wordleOfTheDay);
		}
		solutionwords.seekg(0, ios_base::beg); // reset stream buffer offset ( everyone is kind and does this )

		// GAME SETUP
		string* prevguesses = new string[maxguesses];
		int guess = 0;
		bool guessedCorrect = false;
		bool sneakyDebugging = false;

		// GAME LOOP
		while (guess < maxguesses && !guessedCorrect) {

			cout << clsr;
			if (sneakyDebugging)
				cout << color::red << "DEBUG: Wordle is '" << wordleOfTheDay << "'." << color::wht << endl;
			printGameBoard(wordleOfTheDay, prevguesses, maxguesses);

			string userInputRaw = "";
			string formattedInput = "";
			bool inputSuccess = false;

			// USER INPUT LOOP
			while (!inputSuccess) {
				// the prompt:
				cout << endl << txttab /* << "Guess #" << guess + 1 */ << "Enter a "
					<< wordleOfTheDay.length() << " letter word: " << endl << txttab << "> "; // default positive message.;
				bool containsInvalidChars = false;
				cin >> userInputRaw;

				// check for the worst errors first:
				for (int c = 0; c < userInputRaw.length(); c++) {
					// make lower case and check to make sure all chars are letters
					if (!islower(userInputRaw[c]))
						userInputRaw[c] = tolower(userInputRaw.c_str()[c]);
					if (userInputRaw[c] < int('a') || userInputRaw[c] > int('z')) {
						containsInvalidChars = true;
						break;
					}
				}
				if (!cin || userInputRaw.length() != 5 || containsInvalidChars) {
					cin.clear();
					cin.ignore(INT_MAX, '\n');
					cout << color::red << txttab;
					if (containsInvalidChars)
						cout << "You may only enter letters. ";
					else
						cout << "You may only enter 5 letter words. ";
					cout << color::wht;
				}
				// check against answer, then against previous inputs, then files
				else {
					formattedInput = userInputRaw; // we trust the input to be reasonable

					if (formattedInput == "debug") // cheat code to enable debug view
						sneakyDebugging = true;
					if (formattedInput == wordleOfTheDay) {
						prevguesses[guess] = formattedInput;
						guess++;
						guessedCorrect = true;
						inputSuccess = true; // input success case 1 of 2
					}
					else {
						cout << txttab;
						// cout << txttab << "DEBUG: Checking input... "; // plenty fast, no need
						// check against previous inputs first
						bool repeat = false;
						for (int wdx = 0; wdx < maxguesses; wdx++) {
							if (formattedInput == prevguesses[wdx]) {
								cout << color::red << "Already entered." << color::wht << endl;
								//cout << txttab << "Enter a new " << wordleOfTheDay.length() << " letter word: " << txttab; // superceded
								repeat = true;
								break;
							}
						}
						// if not a repeat, check against files.
						if (!repeat && (filecontains(solutionwords, formattedInput, numSolutions) || filecontains(allwords, formattedInput, numWords))) {
							prevguesses[guess] = formattedInput;
							guess++; // got something from the user!
							inputSuccess = true; // input success case 2 of 2

						}
						else if (!repeat) { // if it wasn't a repeat and didn't make it through the list it is not a word
							cout << color::red << "Not in word list." << color::wht;
						}
					}
				}
			}
		}

		// POST-GAME
		cout << clsr;
		printGameBoard(wordleOfTheDay, prevguesses, maxguesses);
		if (guessedCorrect)
			cout << endl << color::grn << txttab << "You got it!" << color::wht << endl;
		else
			cout << endl << color::ylw << txttab << "Better luck next time!" << color::wht << endl;
		cout << txttab << "The word was '" << wordleOfTheDay << "'." << endl;

		delete[]prevguesses;

		// PLAY AGAIN WITH Y
		cout << txttab << "Play again? (Y): ";
		cin >> buf;
		if (buf[0] == 'y' || buf[0] == 'Y')
			cout << clsr;
		else break;
	}

	cout << txttab << "Thanks for playing!" << endl;

	// FINAL CLEANUP
	allwords.close();
	solutionwords.close();
	return 0;
}
