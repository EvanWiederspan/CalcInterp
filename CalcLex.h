/** Functionality for CalcLex has not changed except that it now will only
*** read the first decimal point in numconsts (in response to the comment from
*** my last lab). Has been reorganized so that the lexer can be called multiple times
*** on multiple strings for reading input from the command line
*** 
**/
#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <list>
#include <fstream>
#include <sstream>

using namespace std;

enum TOKEN { ERROR = 0, EOFSY = 1, READSY, WRITESY, LPAREN, RPAREN, ASSIGNOP, ADDOP, SUBOP, MULTOP, DIVOP, ID, NUMCONST, COMMENT };


// Simple struct to store the tokens in the list
struct Symbol {
public:
	TOKEN token;
	string data;
	Symbol(TOKEN tok, string dat) : token(tok), data(dat) { }
};

class CalcLex
{
public:
	CalcLex();
	~CalcLex();
	// Can either return a list of symbols from a file or a string
	// Will return a null pointer if an unknown token was found
	list<Symbol>* tokenizeFile(string filePath);
	list<Symbol>* tokenizeString(string data);
private:
	// String that holds the current string being read, getNext() will add in one char at a time
	string currToken;
	istream* source;
	list<Symbol>* tokenize();
	char getNext();
	char peekNext();
	TOKEN slashPath();
	TOKEN numPath();
	TOKEN letterPath();
	TOKEN gettoken();
};