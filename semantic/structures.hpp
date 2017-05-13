#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <string>
#include <vector>

/**
 * Structure representing a semantic error
 */
struct SemErr
{
	SemErr(int line=0, int column=0, std::string errStr=""){this->errStr = errStr; this->line=line; this->column=column;};
	std::string errStr; /**< The error message */
	int line; /**< The line where the error occured */
	int column; /**< The column where the error occured */
};

/**
 * Structure carrying the needed information for expression semantic checking
 */
struct ExprType
{
	ExprType(std::string type=""){error = false; this->type = type; errors=std::vector<SemErr*>();};
	bool error; /**< If a semantic error occured somewhere in this expression */
	std::string type; /**< The vsop type of the expression */
	std::vector<SemErr*> errors; /**< A vector of the semantic errors that occured in this expression */
	/**
	 * Add a semantic error
	 * 
	 * @param err The semantic error to add
	 */
	void addError(SemErr* err)
	{
		errors.push_back(err);
		error=true;
	};

	/**
	 * Add semantic errors
	 * 
	 * @param errors A vector containing the semantic errors to add
	 */
	void addErrors(std::vector<SemErr*> errors)
	{
		this->errors.insert(this->errors.end(), errors.begin(), errors.end());
		error = true;
	};
};
#endif
