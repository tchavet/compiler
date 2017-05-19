#ifndef LLVMMANAGER_H
#define LLVMMANAGER_H

#include <string>
#include <unordered_map>
#include <ostream>
#include <vector>

typedef std::unordered_map<std::string,int> stringmap;
typedef std::unordered_map<std::string,stringmap> classmap;

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
		 * Write the code to begin the llvm main function
		 */
		void beginMain();

		/**
		 * Write the code to end the llvm main function
		 */
		void endMain();

		/**
		 * Get the corresponding llvm type of a vsop type
		 * 
		 * @param type The vsop type
		 * 
		 * @return The llvm type
		 */
		static std::string llvmType(std::string type);

		void incIndent();
		void decIndent();

	protected:
		int indent;
		classmap methodsMap; //for the functions
		classmap fieldsMap; //for the fields//
		stringmap llvmVars; 
		stringmap llvmLabels; 
		std::vector<std::ostream*> outputs;
};
#endif
