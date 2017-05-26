/**@mainpage

# Compiler steps
## Parsing
The vsop source code is first parsed by flex to generate tokens. This is done in the file vsop.lex.
If the parameter -lex was provided during compilation, the tokens will be printed.
Those tokens are then passed to bison.
## Grammar
All grammar rules are written in vsop.y
### Nodes
A tree is built, with each node being a basic vsop statement. The root node is a ProgramNode, which contains a ClassNode for each class.
## %Semantic checking
%Semantic checking is performed by the Semantic class.
### Classes
First the Semantic::classesCheck method is called. The Types table is filled with the built in types provided, then with each defined class, by going through the class nodes. While doing so it checks for class redefinitions, if the parent class of each class is defined, inheritance cycles, and if the Main class and method are there with the right prototypes.
### Code %semantic checking
The rest of %semantic checking is performed by the Semantic::scopeCheck function, which will run %semantic checking on each class (ClassNode::semCheck). This is done by calling the %semantic checking functions on each method and each field.
The %semantic errors are represented by the SemErr structure, and carried in a vector all through %semantic checking, to be displayed in the end.
#### Methods
%Semantic checking on methods checks these:
	- The return type is valid (built in type or defined by a class definition).
	- The method has not already been defined (ClassNode::redifinedMethod)
	- Parameters checking:
		- The name is not 'self'
		- The type is valid
		- No other parameter with the same name
	- Method's body checking:
		- %Semantic checking on the body
		- The body type is the same as declared

#### Fields
	- The field has not already been defined with another type
	- The field has not been defined in the class or a parent
	- The field's type is valid
	- Initialization expression's checking:
		- %Semantic checking on the expression
		- The expression type is the same as the field's type, or compatible (by inheritance)

#### Expressions
	%Semantic checking for the expressions is performed by calling the ExprNode::getType function, which will run %semantic checking on all sub nodes, and compute the type of the expression. The ExprType structure that is returned contains the type and the %semantic errors that occured.
## LLVM code generation

*/
