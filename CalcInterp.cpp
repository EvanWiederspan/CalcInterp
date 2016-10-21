#include "CalcInterp.h"
#include "InterpException.h"

CalcInterp::CalcInterp(CalcLex& tokenizer) : lex(tokenizer)
{
}


CalcInterp::~CalcInterp()
{
}

void CalcInterp::interp(list<Symbol>* symbolList) {
	if (symbolList == NULL) {
		cout << "Error: null pointer passed to interpreter" << endl;
		return;
	}
	symbols = symbolList;
	i = symbols->begin();
	tok = i->token;
	Program();
}

double CalcInterp::getID(string id) {
	try {
		return symTable.at(id);
	}
	catch (out_of_range e) { // thrown when "id" is not found in the map
		cout << "Warning: Id \"" << id << "\" not yet set, using default value of 0.0" << endl;
		setID(id, 0.0);
		return 0.0;
	}
}

void CalcInterp::setID(string id, double val) {
	// will either update entry or create new one
	symTable[id] = val;
}

void CalcInterp::match(TOKEN expected) {
	// if its the expected token, consume it and move forward
	if (tok == expected) {
		if (tok != TOKEN::EOFSY)
			tok = (++i)->token;
	}
	else { // unexpected token found, will stop interpretation
		throw InterpException(tok);
	}
}

void CalcInterp::Program() {
	try {
		List();
		match(TOKEN::EOFSY);
	}
	catch (InterpException e) {
		e.printError();
	}
}

void CalcInterp::List() {
	if (tok != TOKEN::EOFSY) {
		State();
		List();
	}
}

void CalcInterp::State() {
	if (tok == TOKEN::ID) { // State -> id := E
		string id = i->data;
		match(TOKEN::ID);
		match(TOKEN::ASSIGNOP);
		double val = E();
		setID(id, val);
		cout << "Assigned the value " << val << " to variable " << id << endl;
	}
	else if (tok == TOKEN::READSY) { // State -> read id
		match(TOKEN::READSY);
		if (tok == TOKEN::ID) {
			string id = i->data;
			list<Symbol>* inputTokens = NULL;
			while (true) {
				cout << "Enter a value for " << id << ": ";
				string readVal;
				getline(cin, readVal);
				// tokenize the input and splice it into the current list of symbols, allows users to enter expressions for READs
				inputTokens = lex.tokenizeString(readVal);
				if (inputTokens != NULL) {
					if (inputTokens->begin()->token != TOKEN::EOFSY) {
						break;
					}
					else {
						inputTokens->clear();
						delete(inputTokens);
					}
				}
				cout << "Error: Invalid input, please enter a valid expression" << endl;
			}
			inputTokens->pop_back(); // remove the EOFSY symbol from the input token list
			list<Symbol>::iterator insertPoint = std::next(i);
			// merge the list of symbols from the user input into symbols
			symbols->splice(insertPoint, *inputTokens);
			inputTokens->clear();
			delete(inputTokens);
			match(TOKEN::ID);
			// Note: invalid input from a read (i.e. 2 * * 4) will stop interpretation of a file
			setID(id, E()); // read the user input as E()
		}
	}
	else if (tok == TOKEN::WRITESY) { // State -> write E
		match(TOKEN::WRITESY);
		cout << "Write: " << E() << endl;
	}
	else {
		throw InterpException(tok);
	}
}

double CalcInterp::E() { //  E -> T T_x
	return T_x(T());
}

double CalcInterp::T() { // T -> F F_x
	return F_x(F());
}

double CalcInterp::T_x(double firstTerm) {
	if (tok == TOKEN::ADDOP) { // T_x -> O_a T T_x
		match(TOKEN::ADDOP);
		return T_x(firstTerm + T());
	}
	else if (tok == TOKEN::SUBOP) {
		match(TOKEN::SUBOP);
		return T_x(firstTerm - T());
	}
	else { //  T_x -> NULL
		return firstTerm;
	}
}

double CalcInterp::F() {
	if (tok == TOKEN::LPAREN) { // F -> ( E )
		match(TOKEN::LPAREN);
		double val = E();
		match(TOKEN::RPAREN);
		return val;
	}
	else if (tok == TOKEN::ID) { // F -> id
		double val = getID(i->data);
		match(TOKEN::ID);
		return val;
	}
	else if (tok == TOKEN::NUMCONST) { // F -> numconst
		double val = stod(i->data);
		match(TOKEN::NUMCONST);
		return val;
	}
	else {
		throw InterpException(tok);
	}
}

double CalcInterp::F_x(double firstTerm) {
	if (tok == TOKEN::MULTOP) { // F_x -> O_m F F_x
		match(TOKEN::MULTOP);
		return F_x(firstTerm * F());
	}
	else if (tok == TOKEN::DIVOP) {
		match(TOKEN::DIVOP);
		double second = F();
		if (second == 0) {
			cout << "Warning: Attempted division by zero" << endl;
		}
		return F_x(firstTerm / second);
	}
	else { // F_x -> NULL
		return firstTerm;
	}
}