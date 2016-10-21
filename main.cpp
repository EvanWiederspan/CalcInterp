/*
Evan Wiederspan
Lab 5 CalcInterp
Concepts in Programming
2 December 2015
*/

#include "CalcLex.h"
#include "CalcInterp.h"

int main(int argc, char** argv) {
	cout << "Welcome to the CalcInterpreter!" << endl;
	CalcLex lex = CalcLex();
	CalcInterp _interp = CalcInterp(lex);
	if (argc > 1) {
		cout << "Opening file " << argv[1] << endl;
		list<Symbol>* symList = lex.tokenizeFile(argv[1]);
		if (symList != NULL) {
			_interp.interp(symList);
			symList->clear();
			delete(symList);
		}
	}
	cout << "Interpreter ready for input..." << endl;
	while (true) { // now run a command line interpreter
		cout << ">";
		string input;
		getline(cin, input);
		list<Symbol>* symList = lex.tokenizeString(input);
		if (symList != NULL) {
			_interp.interp(symList);
			symList->clear();
			delete(symList);
		}
	}
	return 0;
}