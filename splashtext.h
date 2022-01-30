/********************************************************************************
 * File:      splashtext.h
 * Author:    Bradyn Braithwaite
 * Purpose:	  contains strings and defines used by game
 ********************************************************************************/
#pragma once

#include <string>
#include <vector>

using namespace std;
// DEFINITIONS & PRECONFIGURATION
#define MODE_L3  3
#define MODE_L4  4
#define MODE_L5  5
#define MODE_L6  6
#define MODE_L7  7
#define MODE_L8  8
// difficulty definitions (max guesses) for [n] letters
const int difficulties[] = {0,0,0, 6, 7, 6, 7, 8, 9 };
// unique solutions for 3/4/5/6/7/8 letter words
const int SOLUTIONS_PER_GAME[] = { 120, 900, 1400, 2000, 3200, 2400 };
const bool REBUILD_SOLUTIONS = false;

#define WER_NOREAD -2
#define WER_MODECHANGE -1
#define WER_OK 0
#define WER_INVCHAR 1
#define WER_WLEN 2
#define WER_EXIST 3
#define WER_DICT 4
#define WER_CIN 5
#define WER_DEBUG 6
#define WER_COLOR 7
#define WER_HMODE 8
#define WER_MUSTUSE 9

#define EXIT_DICTIONARY_OK 5

/*
static const std::vector<std::string> Splash = {
	
"\t\t\t\t\t                          | | |     	     ",
"\t\t\t\t\t                   _ __ __| | | ___ 	     ",
"\t\t\t\t\t    \\ \\ /\\ / / _ \\|  __/ _  | |/   \\ ",
"\t\t\t\t\t     \\ V  V / (_) | | | (_| | |  __/	 ",
"\t\t\t\t\t      \\_/\\_/ \\___/| |  \\____|_|\\___| "
};
*/

string watermark = "C++ clone for CS225 - 2022 by B.P. Braithwaite";
namespace color {
	string white = "\033[0m";
	string black = "\033[30m";
	string grey = "\033[90m";
	string red = "\033[91m";
	string yellow = "\033[93m";
	string green = "\033[92m";
	string blue = "\033[34m";
	string lightblue = "\033[36m";
}
string clearscreen = true ? "\033c" : ""; // make false to see scrollback
string blocktab = "\t\t\t\t\t\t";
string keytab = "\t\t\t\t\t\t   "; // centers keyboard