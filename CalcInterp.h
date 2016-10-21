/*
Interpreter class, takes a list of symbols as input from the lexer class

Grammar definition:
Program -> List EOFSY
List -> State List | NULL
State -> id := E | read id | write E
E   -> T T_x
T   -> F F_x
T_x -> O_a T T_x | NULL
F   -> '(' E ')' | id | numconst
F_x -> O_m F F_x | NULL
O_a -> + | -
O_m -> * | /
*/

#pragma once
#include "CalcLex.h"
#include <map>

class CalcInterp
{
public:
	CalcInterp(CalcLex& lex); // a reference to the lexer so that it can tokenize the input for READSY
	~CalcInterp();
	void interp(list<Symbol>* symbolList);
private:
	list<Symbol>* symbols; // list of symbols being currently read, received from the lexer
	list<Symbol>::iterator i; // iterator for moving through the list of symbols
	map<string, double> symTable; // symbol table
	TOKEN tok; // current token being read
	CalcLex& lex; 
	// Consumes the current symbol and moves the iterator forward
	void match(TOKEN expected);
	// functions for interacting with the symbol table
	double getID(string id);
	void setID(string id, double val);
	// functions for the recursive descent parser
	void Program();
	void List();
	void State();
	double E();
	double T();
	double T_x(double firstTerm);
	double F();
	double F_x(double firstTerm);
	void O_a();
	void O_m();
};

