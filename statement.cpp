/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
#include "parser.h"
using namespace std;

/* Implementation of the Statement class */

StatementType analyzeStatement(string line)
{
	if (line == "RUN" || line == "LIST" || line == "CLEAR" || line == "QUIT" || line == "HELP") return COMMAND;
	if (line[0] >= 48 && line[0] <= 57) // LineNumber
	{
		string t = line.substr(line.find(" ") + 1, line.length() - line.find(" ") - 1);
		string p = t.substr(0, t.find(" "));
		if (p == "IF" || p == "GOTO") return CONTROL;
		else return SEQUENTIAL;
	}
	else return DIRECTLY_EXECUTED;
}
Statement::Statement(string& _line): line(_line),lineNumber(0) {
   /* Empty */
}

Statement::~Statement() {
   /* Empty */
}

string Statement::getLine()
{
	return line;
}

SequentialStatement::SequentialStatement(string &_line) : Statement(_line), terminated(false), exp(nullptr)
{
	string t = line.substr(line.find(" ") + 1, line.length() - line.find(" ") - 1);
	identifier = t.substr(0, t.find(" "));
	statement = t.substr(t.find(" ") + 1, t.length() - t.find(" ") - 1);

	if (identifier == "REM") ty = 1;
	else if (identifier == "LET") ty = 2;
	else if (identifier == "PRINT") ty = 3;
	else if (identifier == "INPUT") ty = 4;
	else if (identifier == "END") ty = 5;
	else error("SYNTAX ERROR");
}

/*
1 REM
2 LET
3 PRINT
4 INPUT
5 END
*/
SequentialStatement::~SequentialStatement()
{
	if (exp != nullptr) delete exp;
}

bool SequentialStatement::hasEnd()
{
	return terminated;
}

void SequentialStatement::execute(EvalState &state)
{
	if (ty == 1) return;
	if (ty == 5)
	{
		terminated = true;
		return;
	}
	string addStr = "";
	if (ty == 4)
	{
		string val = "";
		for (;;)
		{
			try
			{
				cout << " ? ";
				for (char ch = getchar();ch != '\n';ch = getchar()) val += ch;
				stringToInteger(val);
				break;
			}
			catch (...)
			{
				cout << "INVALID NUMBER" << endl;
				val = "";
			}
		}
		addStr = " = " + val;
	}

	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.setInput(statement + addStr);
	exp = parseExp(scanner);

	if (ty == 3) cout << (exp->eval(state)) << endl;
	else exp->eval(state);
}

ControlStatement::ControlStatement(string &_line) : Statement(_line), comp(0), gotoLine(0), lhs(nullptr), rhs(nullptr)
{
	string t = line.substr(line.find(" ") + 1, line.length() - line.find(" ") - 1);
	string identifier = t.substr(0, t.find(" "));
	string statement = t.substr(t.find(" ") + 1, t.length() - t.find(" ") - 1);

	if (identifier == "GOTO")
	{
		ty = 1;
		gotoLine = stringToInteger(statement);
	}
	else
	{
		ty = 2;
		auto p = statement.find("THEN");
		gotoLine = stringToInteger(statement.substr(p + 5, statement.length() - p - 5));
		statement = statement.substr(0, p);
		p = statement.find('<'), comp = 1;
		if (p == string::npos) p = statement.find('>'), comp = 2;
		if (p == string::npos) p = statement.find('='), comp = 3;
		if (p == string::npos) error("SYNTAX ERROR");

		TokenScanner scanner1, scanner2;
		scanner1.ignoreWhitespace(); scanner2.ignoreWhitespace();

		scanner1.setInput(statement.substr(0, p));
		lhs = parseExp(scanner1);

		scanner2.setInput(statement.substr(p + 1, statement.size() - p - 1));
		rhs = parseExp(scanner2);
	}
}

ControlStatement::~ControlStatement()
{
	if (lhs != nullptr) delete lhs;
	if (rhs != nullptr) delete rhs;
}

void ControlStatement::execute(EvalState &state) {}

int ControlStatement::getNextLine(EvalState &state)
{
	if (ty == 1) return gotoLine;

	int valLeft = lhs->eval(state);
	int valRight = rhs->eval(state);

	if (comp == 1) // <
	{
		if (valLeft >= valRight) return 0;
	}
	else if (comp == 2) // >
	{
		if (valLeft <= valRight) return 0;
	}
	else if (comp == 3)
	{
		if (valLeft != valRight) return 0;
	}
	return gotoLine;
}

DirectlyExecutedStatement::DirectlyExecutedStatement(string &_line) : Statement(_line), exp(nullptr)
{
	string identifier = line.substr(0, line.find(" ") + 1);
	statement = line.substr(line.find(" ") + 1, line.length() - line.find(" ") - 1);

	if (identifier == "LET") ty = 1;
	else if (identifier == "PRINT") ty = 2;
	else if (identifier == "INPUT") ty = 3;
}

/*
1 LET
2 PRINT
3 INPUT
*/

DirectlyExecutedStatement::~DirectlyExecutedStatement()
{
	if (exp != nullptr) delete exp;
}

void DirectlyExecutedStatement::execute(EvalState &state)
{
	string addStr = "";
	if (ty == 3)
	{
		string val = "";
		for (;;)
		{
			try
			{
				cout << " ? ";
				for (char ch = getchar(); ch != '\n'; ch = getchar()) val += ch;
				stringToInteger(val);
				break;
			}
			catch (...)
			{
				cout << "INVALID NUMBER" << endl;
				val = "";
			}
		}
		addStr = " = " + val;
	}

	TokenScanner scanner;
	scanner.ignoreWhitespace();
	scanner.setInput(statement + addStr);
	exp = parseExp(scanner);

	if (ty == 2) cout << exp->eval(state) << endl;
	else exp->eval(state);
}