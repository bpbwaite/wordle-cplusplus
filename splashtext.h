/********************************************************************************
 * File:      splashtext.h
 * Author:    Bradyn Braithwaite
 * Purpose:	  contains strings used by game
 ********************************************************************************/
#pragma once

#include <string>
#include <vector>

using namespace std;

static const std::vector<std::string> Splash = {
"\t\t\t\t\t                          | | |     	     ",
"\t\t\t\t\t                   _ __ __| | | ___ 	     ",
"\t\t\t\t\t    \\ \\ /\\ / / _ \\|  __/ _  | |/   \\ ",
"\t\t\t\t\t     \\ V  V / (_) | | | (_| | |  __/	 ",
"\t\t\t\t\t      \\_/\\_/ \\___/| |  \\____|_|\\___| "
};                                             
string watermark = "C++ clone for CS225 - 2022 by B.P. Braithwaite";
namespace color {
	string white = "\033[0m";
	string blue = "\033[34m";
	string lightblue = "\033[36m";
	string green = "\033[92m";
	string yellow = "\033[93m";
	string red = "\033[91m";
	string grey = "\033[90m";
	string black = "\033[30m";
}
string clearscreen = true ? "\033c" : ""; // make false to see scrollback
string blocktab = "\t\t\t\t\t\t";
string keytab = "\t\t\t\t\t\t   "; // centers keyboard