/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */

#include <string>
#include "evalstate.h"

#include "../StanfordCPPLib/map.h"
using namespace std;

/* Implementation of the EvalState class */

EvalState::EvalState() {
   /* Empty */
}

EvalState::~EvalState() {
   /* Empty */
}

bool EvalState::isDefined(string var) {
   return symbolTable.containsKey(var);
}
const int RK_SIZE = 12;
const string RESERVED_KEYWORD[RK_SIZE] = { "REM", "LET", "PRINT", "END", "GOTO",
									"IF", "THEN", "RUN", "LIST", "CLEAR",
									"QUIT", "HELP" };
bool checkValid(string var)
{
	for (int i = 0; i < RK_SIZE; ++i)
		if (var == RESERVED_KEYWORD[i]) return 0;
	if (var[0] >= 48 && var[0] <= 57) return 0;
	return 1;
}
void EvalState::setValue(string var, int value) {
   if (checkValid(var)) symbolTable.put(var, value);
   else error("SYNTAX ERROR");
}

int EvalState::getValue(string var) {
   return symbolTable.get(var);
}


