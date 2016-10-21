#include "CalcLex.h"


CalcLex::CalcLex() : source(NULL), currToken("")
{
}

CalcLex::~CalcLex()
{
}

list<Symbol>* CalcLex::tokenizeString(string data) {
	source = new istringstream(data);
	list<Symbol>* tokens = tokenize();
	delete(source);
	source = NULL;
	return tokens;
}

list<Symbol>* CalcLex::tokenizeFile(string filePath) {
	ifstream* file = new ifstream(filePath);
	if (file->fail()) {
		cout << "Error: Could not open file " << filePath << endl;
		file->close();
		delete(file);
		return NULL;
	}
	source = file;
	list<Symbol>* tokens = tokenize();
	file->close();
	delete(file);
	source = NULL;
	return tokens;
}

list<Symbol>* CalcLex::tokenize() {
	currToken = "";
	list<Symbol>* tokens = new list<Symbol>; // Holds all of the tokens returned from gettoken();
	for (;; currToken = "") {
		TOKEN tok = gettoken();
		if (tok == TOKEN::COMMENT) // Comments are not added to the list
			continue;
		if (tok == TOKEN::ERROR) {
			cout << "Lex Error: Unrecognized symbol \"" << currToken << "\"" << endl;
			return NULL;
		}
		tokens->push_back(Symbol(tok, currToken));
		if (tok == TOKEN::EOFSY) // When we encounter this, we're done
			break;
	}
	return tokens;
}

// return next character, move read pointer forward, store character in currToken
char CalcLex::getNext() {
	char next = source->get();
	if (source->eof() == true)
		return 0;
	if (next >= 'A' && next <= 'Z') // make letters lowercase
		next += ('a' - 'A');
	currToken += next;
	return next;
}

// return next character but does not add to currToken or move read pointer forward
char CalcLex::peekNext() {
	char next = source->peek();
	if (next >= 'A' && next <= 'Z')
		next -= ('a' - 'A');
	return next;
}

// Logic path for when encountering a '/'
TOKEN CalcLex::slashPath() {
	if (peekNext() == '*') {// its a comment
		getNext();
		while (true) {
			char next;
			//while (next != '*' || next != '\n') { int a = 1; }
			while ((next = getNext()) != '*' && next != '\n') {}
			if (peekNext() == '/') { // end of comment or line
				getNext();
				break;
			}
			else if (next == '\n')
				break;
		}
		return TOKEN::COMMENT;
	}
	else { // its a division symbol
		return TOKEN::DIVOP;
	}

}

// Logic path for encountering a number as the first character in the token
TOKEN CalcLex::numPath() {
	char next;
	bool decimal = false; // added in response to comments from Lab 4, set to true once the first decimal is reached
	while (((next = peekNext()) >= '0' && next <= '9') || (!decimal && (decimal = (next == '.')))) { getNext(); }
	return TOKEN::NUMCONST;
}

// Logic path for encountering a letter as the first character in the token
TOKEN CalcLex::letterPath() {
	char next;
	while (((next = peekNext()) >= 'a' && next <= 'z') || (next >= '0' && next <= '9') || next == '_') { getNext(); }
	if (currToken == "read")
		return TOKEN::READSY;
	else if (currToken == "write")
		return TOKEN::WRITESY;
	else
		return TOKEN::ID;
}

// Returns the next token in the file, returns the type and puts the text data in currToken
TOKEN CalcLex::gettoken() {
	char start = getNext();

	if (start == ' ' || start == '\n' || start == '\t')
		return TOKEN::COMMENT;
	else if (start == '(')
		return TOKEN::LPAREN;
	else if (start == ')')
		return TOKEN::RPAREN;
	else if (start == '+')
		return TOKEN::ADDOP;
	else if (start == '-')
		return TOKEN::SUBOP;
	else if (start == '*')
		return TOKEN::MULTOP;
	else if (start == '/')
		return slashPath();
	else if (start >= '0' && start <= '9')
		return numPath();
	else if (start >= 'a' && start <= 'z')
		return letterPath();
	else if (start == ':') {
		if (peekNext() == '=') {
			getNext();
			return TOKEN::ASSIGNOP;
		}
		return TOKEN::ERROR;
	}
	else if (start <= 0) // end of file
		return TOKEN::EOFSY;
	else // for any unrecognized symbols. This wil cause the tokenize() to return a null pointer
		return TOKEN::ERROR;
}