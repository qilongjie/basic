/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "statement.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;

/* Function prototypes */

ResultType processLine(string line, Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   while (true) {
      try {
		  ResultType t = processLine(getLine(), program, state);
		  if (t == QUIT) break;
		  if (t == HELP)
		  {
			  cout << "input RUN for running program" << endl;
			  cout << "input LIST for listing out the whole program" << endl;
			  cout << "input CLEAR for deleting all program and variables" << endl;
			  cout << "input QUIT for quiting the program" << endl;
		  }
		  if (t == EXECUTED) continue;
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version,
 * the implementation does exactly what the interpreter program
 * does in Chapter 19: read a line, parse it as an expression,
 * and then print the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

ResultType processLine(string line, Program & program, EvalState & state) {
	for (;line.length() && line[line.length() - 1] == ' ';) line = line.substr(0, line.length() - 2);
	if (line.length() == 0) return EXECUTED;
	StatementType current = analyzeStatement(line);
	if (current == COMMAND) return program.runCommand(line);
	else
	{
		if (line[0] >= 48 && line[0] <= 57) //linenumber
		{
			int linenumber = stringToInteger(line.substr(0, line.find(" ")));
			if (line.find(" ") == string::npos) program.removeSourceLine(linenumber);
			program.addSourceLine(linenumber, line);
		}
		else //Direct
			program.addSourceLine(0, line);
		return EXECUTED;
	}
}
