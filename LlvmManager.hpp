#ifndef LLVMMANAGER_H
#define LLVMMANAGER_H

#include <string>
#include <unordered_map>
#include <ostream>
#include <vector>

typedef std::unordered_map<std::string,int> stringmap;
typedef std::unordered_map<std::string,stringmap> classmap;

/**
 * This class is used to do the common llvm operations, such as getting a pointer to a method, or writing llvm code and get a unique variable name.
 * When instantiating it, multiple output streams can be given, that way it can write the llvm output code to the screen and in a file at the same time. This manager is passed along each node during llvm code generation.
 */
class LlvmManager
{
	public:
		/**
		 * Constructor
		 * 
		 * @param outs A vector of all the output streams where the llvm code needs to be output
		 * @param moduleId The llvm module id for this code
		 */
		LlvmManager(std::vector<std::ostream*> outs, std::string moduleId = "main.vsop");

		/**
		 * Constructor
		 * 
		 * @param out The output stream where the llvm code needs to be output
		 * @param moduleId The llvm module id for this code
		 */
		LlvmManager(std::ostream *out, std::string moduleId = "main.vsop");

		/**
		 * Write llvm code to the output stream
		 * 
		 * @param toWrite The llvm code to write
		 * @param ret The name of the llvm variable to assign (if set to "", no variable will be assigned, if set to "." it will assigned to an unnamed variable)
		 * 
		 * @return The llvm name of the variable that was assigned
		 */
		std::string write(std::string toWrite, std::string ret = "", bool global=false);

		/**
		 * Write an llvm constant outside of a function
		 * 
		 * @param toWrite The llvm code to write
		 * @param ret The name of the llvm variable to assign
		 * 
		 * @return The llvm name of the variable that was assigned
		 */
		std::string addCst(std::string toWrite, std::string ret="");

		/**
		 * Write all the llvm constants
		 */
		void writeConstants();

		/**
		 * Get a new unique llvm variable name for a vsop variable
		 * 
		 * @param name The name of the variable for which to get a name
		 * 
		 * @return A unique llvm name for the variable
		 */
		std::string getNewVarName(std::string name);

		/**
		 * Write an llvm label to the output
		 * 
		 * @param llvmLabel The name of the label to output
		 */
		void writeLabel(std::string llvmLabel);

		/**
		 * Get a new unique llvm label
		 * 
		 * @param label The name for which to get a unique label
		 * 
		 * @return The unique label
		 */
		std::string getNewLabel(std::string label);

		/**
		 * Get a pointer to the method of an object
		 * 
		 * @param className The name of the class the method belongs to
		 * @param functionName The name of the method to get the pointer for
		 * @param object The vsop variable name of the object that the method belongs to
		 * 
		 * @return The llvm variable name of the pointer to the function
		 */
		std::string getFunction(std::string className, std::string methodName, std::string methodClassName, std::string object);
		/**
		 * Get a pointer to the field of an object
		 * 
		 * @param className The name of the class the field belongs to
		 * @param fieldName The name of the field to get
		 * @param object The vsop variable name of the object that the field belongs to
		 * 
		 * @return The llvm variable name of the pointer to the field
		 */
		std::string getField(std::string className, std::string fieldName, std::string object);

		/**
		 * Add the stringmap of a class
		 * 
		 * @param className The class name
		 * @param methods The stringmap of the methods of the class
		 * @param fields The stringmap of the fields of the class
		 */
		void addClass(std::string className, stringmap methods, stringmap fields);

		/**
		 * Write the code of the llvm main function
		 */
		void main();

		/**
		 * Get the corresponding llvm type of a vsop type
		 * 
		 * @param type The vsop type
		 * 
		 * @return The llvm type
		 */
		static std::string llvmType(std::string type);

		/** Increment the indendation of the output
		 */
		void incIndent();

		/** decrement the indendation of the output
		 */
		void decIndent();

	protected:
		int indent; /**< The current number of tabs to use to indent the output */
		classmap methodsMap; /**< The map between each class and its methods */
		classmap fieldsMap; /**< The map between each class and its fields */
		stringmap llvmVars; /**< The current number of each llvm variable */
		stringmap llvmLabels; /**< The current number of each llvm label */
		std::vector<std::ostream*> outputs; /**< The streams where to output the llvm code */
		std::vector<std::string> constants; /**< The constants to write at the end of the llvm file */
};
#endif
