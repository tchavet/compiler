%{
#include <iostream>
#include <string>
#include <vector>
#include <utility>

#include "nodes/AstNode.hpp"
#include "nodes/AssignNode.hpp"
#include "nodes/BinOpNode.hpp"
#include "nodes/BlockNode.hpp"
#include "nodes/BoolLitNode.hpp"
#include "nodes/CallNode.hpp"
#include "nodes/ClassNode.hpp"
#include "nodes/FieldNode.hpp"
#include "nodes/FormalNode.hpp"
#include "nodes/IfNode.hpp"
#include "nodes/IntLitNode.hpp"
#include "nodes/LetNode.hpp"
#include "nodes/MethodNode.hpp"
#include "nodes/NewNode.hpp"
#include "nodes/ObjectIdNode.hpp"
#include "nodes/ProgramNode.hpp"
#include "nodes/StringLitNode.hpp"
#include "nodes/UnOpNode.hpp"
#include "nodes/WhileNode.hpp"

#include "vsop.tab.h"

extern "C" int yylex();

extern ProgramNode *root;

struct YYLTYPE;
static void yyerror(const char*);
void syntax_error(int line, int col, std::string message="");

%}


%union
{
	int intval;
	std::string* strval;
	AstNode* node;
	std::vector<AstNode*>* nodeVector;
	ClassNode* classNode;
	std::vector<ClassNode*>* classVect;
	std::pair<std::vector<FieldNode*>*, std::vector<MethodNode*>*>* fieldMethod;
	FieldNode* fieldNode;
	MethodNode* methodNode;
	FormalNode* formalNode;
	std::vector<FormalNode*>* formalVect;
	BlockNode* blockNode;
}

%token <intval> INT_LIT
%token AND
%token BOOL
%token CLASS
%token DO
%token ELSE
%token EXTENDS
%token FALSE
%token IF
%token IN
%token INT32
%token ISNULL
%token LET
%token NEW
%token NOT
%token STRING
%token THEN
%token TRUE
%token UNIT
%token WHILE
%token <strval> TYPE_ID
%token <strval> OBJECT_ID
%token <strval> STRING_LIT
%token LBRACE
%token RBRACE
%token LPAR
%token RPAR
%token COLON
%token SEMICOLON
%token COMMA
%token PLUS
%token MINUS
%token TIMES
%token DIV
%token POW
%token DOT
%token EQUAL
%token LOWER
%token LOWER_EQ
%token ASSIGN

/*Precedence options*/
%right ASSIGN
%left AND
%right NOT
%nonassoc LOWER LOWER_EQ EQUAL 
%left PLUS MINUS
%left TIMES DIV
%right ISNULL 
%right POW
%left DOT

%type <strval> type
%type <classNode> class
%type <classVect> classopt
%type <fieldNode> field
%type <methodNode> method
%type <formalNode> formal
%type <formalVect> formals formalopt
%type <blockNode> block

%type <node> expr arg literal boolean_literal
%type <nodeVector> expropt args argopt
%type <fieldMethod> field_method class_body

%start program

%%

program:
	   class classopt
	   {
		 root->addClass($1);
		 root->addClasses($2);
	   }

classopt:
		classopt class
		{
			$1->push_back($2);
			$$ = $1;
		}
|		%empty
		{
			std::vector<ClassNode*> *classes = new std::vector<ClassNode*>();
			$$ = classes;
		}

class:
	 CLASS TYPE_ID class_body
	 {
		ClassNode* classNode = new ClassNode(@1.first_line, @1.first_column, *$2, "Object");
		classNode->addFields($3->first);
		classNode->addMethods($3->second);
		$$ = classNode;
	 }
|	 CLASS TYPE_ID EXTENDS TYPE_ID class_body
	 {
		ClassNode* classNode = new ClassNode(@1.first_line, @1.first_column, *$2, *$4);
		classNode->addFields($5->first);
		classNode->addMethods($5->second);
		$$ = classNode;
	 }

class_body:
		  LBRACE field_method RBRACE
		  {
			$$ = $2;
		  }

field_method:
			%empty
			{
			  std::pair<std::vector<FieldNode*>*, std::vector<MethodNode*>*> *fields_methods = new std::pair<std::vector<FieldNode*>*, std::vector<MethodNode*>*>();
			  fields_methods->first = new std::vector<FieldNode*>();
			  fields_methods->second = new std::vector<MethodNode*>();
			  $$ = fields_methods;
			}
|			field_method field
			{
			  $1->first->push_back($2);
			  $$ = $1;
			}
|			field_method method
			{
			  $1->second->push_back($2);
			  $$ = $1;
			}

field:
	OBJECT_ID COLON type SEMICOLON
	{
		$$ = new FieldNode(@1.first_line, @1.first_column, *$1, *$3);
	}

|	OBJECT_ID COLON type ASSIGN expr SEMICOLON
	{
		$$ = new FieldNode(@1.first_line, @1.first_column, *$1, *$3, $5);
	}

method:
	 OBJECT_ID LPAR formals RPAR COLON type block
	 {
		$$ = new MethodNode(@1.first_line, @1.first_column, *$1, *$3, *$6, $7);
	 }

type:
	TYPE_ID
	{
		$$ = $1;
	}
|	INT32
	{
		$$ = new std::string("int32");
	}
|	BOOL
	{
		$$ = new std::string("bool");
	}
|	STRING
	{
		$$ = new std::string("string");
	}
|	UNIT
	{
		$$ = new std::string("unit");
	}


formals:
	formalopt formal
	{
		$1->push_back($2);
		$$ = $1;
	}
|	%empty
	{
		$$ = new std::vector<FormalNode*>();
	}

formalopt:
	formalopt formal COMMA
	{
		$1->push_back($2);
		$$ = $1;
	}
|	%empty
	{
		$$ = new std::vector<FormalNode*>();
	}

formal:
	OBJECT_ID COLON type
	{
		$$ = new FormalNode(@1.first_line, @1.first_column, *$1, *$3);
	}

block:
	LBRACE expropt expr RBRACE
	{
		$$ = new BlockNode(@1.first_line, @1.first_column);
		$$->addExpressions($2);
		$$->addExpression($3);
	}

expropt:
	expropt expr SEMICOLON
	{
		$1->push_back($2);
		$$ = $1;
	}
|	%empty
	{
		$$ = new std::vector<AstNode*>();
	}

expr:
	IF expr THEN expr
	{
		$$ = new IfNode(@1.first_line, @1.first_column, $2, $4);
	}
|	IF expr THEN expr ELSE expr
	{
		$$ = new IfNode(@1.first_line, @1.first_column, $2, $4, $6);
	}
|	WHILE expr DO expr
	{
		$$ = new WhileNode(@1.first_line, @1.first_column, $2, $4);
	}
|	LET OBJECT_ID COLON type IN expr
	{
		$$ = new LetNode(@1.first_line, @1.first_column, *$2, *$4, $6);
	}
|	LET OBJECT_ID COLON type ASSIGN expr IN expr
	{
		$$ = new LetNode(@1.first_line, @1.first_column, *$2, *$4, $8, $6);
	}
|	OBJECT_ID ASSIGN expr
	{
		$$ = new AssignNode(@1.first_line, @1.first_column, *$1, $3);
	}
|	NOT expr
	{
		$$ = new UnOpNode(@1.first_line, @1.first_column, "not", $2);
	}
|	MINUS expr
	{
		$$ = new UnOpNode(@1.first_line, @1.first_column, "-", $2);
	}
|	ISNULL expr
	{
		$$ = new UnOpNode(@1.first_line, @1.first_column, "isnull", $2);
	}
|	expr AND expr 
	{
		$$ = new BinOpNode(@1.first_line, @1.first_column, "and", $1, $3);
	}
|	expr EQUAL expr 
	{
		$$ = new BinOpNode(@1.first_line, @1.first_column, "=", $1, $3);
	}
|	expr LOWER expr
	{
		$$ = new BinOpNode(@1.first_line, @1.first_column, "<", $1, $3);
	}
|   expr LOWER_EQ expr 
    {
		$$ = new BinOpNode(@1.first_line, @1.first_column, "<=", $1, $3);
	}
|	expr PLUS expr
	{
		$$ = new BinOpNode(@1.first_line, @1.first_column, "+", $1, $3);
	}
|	expr MINUS expr
	{
		$$ = new BinOpNode(@1.first_line, @1.first_column, "-", $1, $3);
	}
|	expr TIMES expr
	{
		$$ = new BinOpNode(@1.first_line, @1.first_column, "*", $1, $3);
	}
|	expr DIV expr
	{
		$$ = new BinOpNode(@1.first_line, @1.first_column, "/", $1, $3);
	}
|	expr POW expr
	{
		$$ = new BinOpNode(@1.first_line, @1.first_column, "^", $1, $3);
	}
|	OBJECT_ID LPAR args RPAR
	{
		$$ = new CallNode(@1.first_line, @1.first_column, new ObjectIdNode(@1.first_line, @1.first_column, "self"), *$1, *$3);
	}
|	expr DOT OBJECT_ID LPAR args RPAR
	{
		$$ = new CallNode(@1.first_line, @1.first_column, $1, *$3, *$5);
	}
|	NEW TYPE_ID
	{
		$$ = new NewNode(@1.first_line, @1.first_column, *$2);
	}
|	OBJECT_ID
	{
		$$ = new ObjectIdNode(@1.first_line, @1.first_column, *$1);
	}
|	literal
	{
		$$ = $1;
	}
|	LPAR expr RPAR
	{
		$$ = $2;
	}
|	block
	{
		$$ = $1;
	}

args:
	argopt arg
	{
		$1->push_back($2);
		$$ = $1;
	}
| %empty
	{
		$$ = new std::vector<AstNode*>();
	}

argopt:
	  argopt arg COMMA
	  {
		$1->push_back($2);
		$$ = $1;
	  }
| %empty
	{
		$$ = new std::vector<AstNode*>();
	}
arg:
   expr
	{
		$$ = $1;
	}

literal:
	STRING_LIT
	{
		$$ = new StringLitNode(@1.first_line, @1.first_column, *$1);
	}
|	INT_LIT
	{
		$$ = new IntLitNode(@1.first_line, @1.first_column, $1);
	}
|	boolean_literal
	{
		$$ = $1;
	}

boolean_literal:
	FALSE
	{
		$$ = new BoolLitNode(@1.first_line, @1.first_column, "false");
	}
|	TRUE
	{
		$$ = new BoolLitNode(@1.first_line, @1.first_column, "true");
	}
%%

static void yyerror(const char *s)
{
	syntax_error(yylloc.first_line, yylloc.first_column);
}

