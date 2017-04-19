#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <vector>

struct SemErr
{
	SemErr(int line=0, int column=0, std::string errStr=""){this->errStr = errStr; this->line=line; this->column=column;};
	std::string errStr;
	int line;
	int column;
};

struct ExprType
{
	ExprType(std::string type=""){error = false; this->type = type; errors=std::vector<SemErr*>();};
	bool error;
	std::string type;
	std::vector<SemErr*> errors;
	void addError(SemErr* err)
	{
		errors.push_back(err);
		error=true;
	};
	void addErrors(std::vector<SemErr*> errors)
	{
		this->errors.insert(this->errors.end(), errors.begin(), errors.end());
		error = true;
	};
};
#endif
