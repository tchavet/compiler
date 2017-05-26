#ifndef CLASSNODE_H
#define CLASSNODE_H

#include "FieldNode.hpp"
#include "MethodNode.hpp"
#include "unordered_map"

typedef std::unordered_map<std::string,int> stringmap;

/**
 * A node representing a vsop class
 */
class ClassNode : public AstNode
{
public:
	/**
	 * Constructor
	 *
	 * @see AstNode
	 * @param name The name of the class
	 * @param parent The name of the class' parent
	 */
	ClassNode(int line, int column, std::string name, std::string parent = "Object");

	std::string printTree(int tabsNb=0, bool types=false);

	std::string getTypeInScope(std::string id);

	/**
	 * Add a field node
	 *
	 * @param field The field node to add
	 */
	void addField(FieldNode* field);

	/**
	 * Add field nodes
	 *
	 * @param fields A vector of the field nodes to add
	 */
	void addFields(std::vector<FieldNode*>* fields);

	/**
	 * Add a method node
	 *
	 * @param method The method node to add
	 */
	void addMethod(MethodNode* method);

	/**
	 * Add method nodes
	 *
	 * @param methods A vector of the method nodes to add
	 */
	void addMethods(std::vector<MethodNode*>* methods);

	/**
	 * Set the class node of the class' parent
	 *
	 * @param parent The class parent's node
	 */
	void setParentNode(ClassNode* parent);

	/**
	 * Get the name of the class
	 *
	 * @return The name of the class
	 */
	std::string getName();

	/**
	 * Get the name of the parent class
	 *
	 * @return The parent class' name
	 */
	std::string getParentName();

	/**
	 * Get the class node of the parent class
	 *
	 * @return The parent class' node
	 */
	ClassNode* getParentNode();

	/**
	 * Check if the class inherits from another class
	 *
	 * @param className The name of the class to check inheritance to
	 *
	 * @return true if this class is a <className>
	 */
	bool isA(std::string className);

	/**
	 * Get the closest common ancestor of this class and another one
	 *
	 * @param class2 The node of the other class
	 *
	 * @return The name of the closest common ancestor's class
	 */
	std::string commonAncestor(ClassNode* class2);

	/**
	 * Get the method node of one of this class' methods (also works if the method is inherited)
	 *
	 * @param methodName The name of the method to get
	 *
	 * @return The method's node
	 */
	MethodNode* getMethod(std::string methodName);

	/**
	 * Get the field node of one of this class' fields (also works if the field is inherited)
	 *
	 * @param fieldName The name of the field to get
	 *
	 * @return The field's node
	 */
	FieldNode* getField(std::string fieldName);

	/**
	 * Get all the fields of this class (without the inherited ones, except if redefined)
	 *
	 * @return A vector of the field nodes
	 */
	std::vector<FieldNode*> getFields();

	/**
	 * Get all the methods of this class (without the inherited ones, except if redefined)
	 *
	 * @return A vector of the method nodes
	 */
	std::vector<MethodNode*> getMethods();

	/**
	 * Run semantic checking on everything this class contains
	 *
	 * @return A vector containing all the semantic errors that occured
	 */
	std::vector<SemErr*> semCheck();

	/**
	 * Check if a field has already been defined (in this class or a parent's)
	 *
	 * @param field The field node of the field to check
	 *
	 * @return true if the field has already been defined before
	 */
	bool redefinedField(FieldNode* field);

	/**
	 * Check if a method has already been defined in this class, or with another prototype in a parent's class
	 *
	 * @param method The method node of the method to check
	 * 
	 * @return true if the method has already been defined in this class, or differently in an ancestor class
	 */
	bool redefinedMethod(MethodNode* method);

	/**
	 * Get a hashmap of all the methods of the class, including inherited ones.
	 * The methods are in the order they appear in the code, the second element is the position
	 * 
	 * @return A hashmap where the first element is the method's name, the second its position in the code
	 */
	stringmap getAllMethods();

	/**
	 * Get a hashmap of all the fields of the class, including inherited ones.
	 * The fields are in the order they appear in the code, the second element is the position
	 * 
	 * @return A hashmap where the first element is the field's name, the second its position in the code
	 */
	stringmap getAllFields();

	/**
	 * Write the llvm header for the class.
	 *  - create methods structure
	 *  - create class structure
	 *  - send the methods and fields maps to the llvm manager
	 *  
	 * @param llvmManager The llvm manager
	 */
	void llvmHeader(LlvmManager *manager);

	std::string llvm(LlvmManager *manager);

	/**
	 * Write the llvm code to allocate and initialize this class
	 * 
	 * @param manager The llvm manager
	 * 
	 * @return The llvm variable pointing to the allocated class
	 */
	std::string llvmAllocate(LlvmManager *manager);

	/**
	 * Write the llvm code to declare the methods of this class
	 * 
	 * @param manager The LlvmManager
	 */
	void declareMethods(LlvmManager *manager);

protected:
	std::string name; /**< The name of the class */
	std::string parentName; /**< The name of the parent of the class */
	ClassNode* parentNode; /**< The class node of the parent of the class */
	std::vector<FieldNode*> fields; /**< The field nodes of this class */
	std::vector<MethodNode*> methods; /**< The method nodes of this class */
	stringmap allMethods;
	stringmap allFields;
	bool allMethodsSet;
	bool allFieldsSet;
};
#endif
