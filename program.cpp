/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;

Program::Program() {
   // Replace this stub with your own code
	state = new EvalState();
}

Program::~Program() {
   // Replace this stub with your own code
	programLine.clear();
	if (state != nullptr) delete state;
}

void Program::clear() {
   // Replace this stub with your own code
	programLine.clear();
	if (state != nullptr) delete state;
	state = new EvalState();
}

void Program::addSourceLine(int lineNumber, string line) {
   // Replace this stub with your own code
	StatementType ty = analyzeStatement(line);
	shared_ptr<Statement> current;

	if (ty == SEQUENTIAL)
	{
		shared_ptr<SequentialStatement> temp(new SequentialStatement(line));
		current = dynamic_pointer_cast<Statement>(temp);
		programLine[lineNumber] = current;
	}
	if (ty == CONTROL)
	{
		shared_ptr<ControlStatement> temp(new ControlStatement(line));
		current = dynamic_pointer_cast<Statement>(temp);
		programLine[lineNumber] = current;
	}
	if (ty == DIRECTLY_EXECUTED)
	{
		auto cur = DirectlyExecutedStatement(line);
		cur.execute(*state);
	}
}

void Program::removeSourceLine(int lineNumber) {
   // Replace this stub with your own code
	programLine.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
	return programLine[lineNumber]->getLine();    // Replace this stub with your own code
}

void Program::setParsedStatement(int lineNumber, Statement *stmt) {
   // Replace this stub with your own code
}

shared_ptr<Statement> Program::getParsedStatement(int lineNumber) {
   return programLine[lineNumber];  // Replace this stub with your own code
}

int Program::getFirstLineNumber() {
	return programLine.begin()->first;     // Replace this stub with your own code
}

int Program::getNextLineNumber(int lineNumber) {
	auto t = programLine.upper_bound(lineNumber);
	if (t == programLine.end()) return -1;
	return t->first;// Replace this stub with your own code
}

void Program::execute()
{
	pair<int, shared_ptr<Statement>> cur = *programLine.begin();
	for (;cur.first >= 0;)
	{
		auto type = analyzeStatement(cur.second->getLine());
		if (type == SEQUENTIAL)
		{
			cur.second->execute(*state);
			if ((dynamic_pointer_cast<SequentialStatement>(cur.second))->hasEnd()) break;
			int t = getNextLineNumber(cur.first);
			cur = make_pair(t, programLine.count(t) ? programLine[t] : nullptr);
		}
		if (type == CONTROL)
		{
			int t = (dynamic_pointer_cast<ControlStatement>(cur.second))->getNextLine(*state);
			if (t == 0) t = getNextLineNumber(cur.first);
			else if (!programLine.count(t)) error("LINE NUMBER ERROR");
			cur = make_pair(t, programLine[t]);
		}
	}
}

ResultType Program::runCommand(string &line)
{
	if (line == "RUN") execute();
	else if (line == "LIST")
	{
		for (auto u : programLine)
		{
			cout << u.second->getLine() << endl;
		}
	}
	else if (line == "CLEAR") clear();

	if (line == "QUIT") return QUIT;
	else if (line == "HELP") return HELP;
	else return EXECUTED;
}