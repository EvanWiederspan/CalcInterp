/* Exception that is thrown when an unexpected token is encountered 
** during interpretation. Will stop interpretation of current state() once it is encountered
*/
#pragma once
#include "CalcLex.h"

class InterpException {
public:
	InterpException(TOKEN found);
	void printError();
private:
	TOKEN _found;
};
