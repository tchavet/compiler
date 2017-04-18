#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <vector>

struct SemErr
{
	std::string errStr;
	int line;
	int column;
};

struct ExprType
{
	bool error;
	std::string type;
	std::vector<SemErr> errors;
};
#endif
