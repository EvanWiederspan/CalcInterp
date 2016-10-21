#include "InterpException.h"


InterpException::InterpException(TOKEN found) : _found(found)
{
}

void InterpException::printError() {
	const string TOKEN_NAME[14] = { "ERROR", "EOFSY", "READSY", "WRITESY", "LPAREN", "RPAREN", "ASSIGNOP", "ADDOP", "SUBOP", "MULTOP", "DIVOP", "ID", "NUMCONST", "COMMENT" };
	cout << "Error: Unexpected token of type " << TOKEN_NAME[_found] << " found" << endl;
}
