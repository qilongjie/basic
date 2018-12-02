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

void EvalState::setValue(string var, int value) {
   if (isDefined(var)) symbolTable.put(var, value);
   else error("SYNTAX ERROR");
}

int EvalState::getValue(string var) {
   return symbolTable.get(var);
}


