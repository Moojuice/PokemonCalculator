/** 
PokemonCalculator.cpp 
author: Kyle Liu
purpose: defines our main function, which creates the calculator, initializes it, and then allows the user to type in enemy inputs 
**/

#include "stdafx.h" //need for visual studio 
#include "pokemoncalculator.h"
#include <iostream>
#include <fstream>

using namespace std;

//our lower case function to avoid case-sensitivity
string toLowerCase(string & input) {
	for (char &c:input) {
		c = tolower(c);
	}
	return input;
}

//standard main function
int main(int argc, char* argv[])
{
	Calculator calculator; 
	if (argc == 2) { //checks num args 
		ifstream ifs; int success_fail;
		ifs.open(argv[1]);
		if (ifs.is_open()) { 
			success_fail = calculator.implement(ifs);
			if (success_fail == SUCCESS) {
				return calculator.calculate();
			}
			else { 
				cout << "Proper text format per line is: <Pokemon name> <move1> <move2> <move3> <move4>" << endl;
				return success_fail;
			}
		}
		else {
			cout << "Error: could not open file, make sure name is spelled correctly and the file is accessible to the .exe (same folder)" << endl;
			return BAD_FILE_NAME;
		}
	}
	else { //checks to make sure input format is correct 
		cout << "Error: bad number of arguments, expecting 2 (including program name)" << endl;
		return BAD_ARGS;
	}
}

