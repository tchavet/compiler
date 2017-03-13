%{
#include <string>

static AstNode *root;

struct YYLTYPE;

%union
{
	int intval;
	std::string strval;
}
%}

%token INT_LIT
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
%token TYPE_ID
%token OBJECT_ID
%token STRING_LIT
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

%start program

%%

program:
	   class classopt
	   {
		 Token* token = new Token(@1.first_line, @1.first_column, Token::Class);
		 root = new AstNode(token);
		 root->addNode($2);
	   }

classopt:
		class | class classopt | %empty

class:
	 CLASS TYPE_ID class-body
	 {
		Token *token = new Token(@1.first_line, @1.first_column, Token::Class);
		AstNode *node = new AstNode(token);
		token = new Token(@2.first_line, @2.first_column, Token::Type, $2.strval);
		node->addNode(new AstNode(token));
		node->addNode($3);
		$$ = node;
	 }
|	 CLASS TYPE_ID EXTENDS TYPE_ID class-body
	 {
		Token *token = new Token(@1.first_line, @1.first_column, Token::Class);
		AstNode *node = new AstNode(token);
		token = new Token(@2.first_line, @2.first_column, Token::Type, $2.strval);
		node->addNode(new AstNode(token));
		token = new Token(@4.first_line, @4.first_column, Token::Type, $4.strval);
		node->addNode(new AstNode(token));
		node->addNodes($5);
		$$ = node;
	 }

class-body:
		  LBRACE field_method RBRACE
		  {
			$$ = $2;
		  }

field_method:
			%empty
			{
			  std::vector<AstNode*> fields_methods(2);
			  $$ = fields_methods;
			}
|			field field_method
			{
			  AstNode *node = new AstNode($1);
			  $2.push_back(node);
			  $$ = $2;
			}
|			method field_method
			{
			  AstNode *node = new AstNode($1);
			  $2.push_back(node);
			  $$ = $2;
			}

field:
	 object-identifier LPAR formals RPAR COLON TYPE_ID block
	 {
		Token *token = new Token(@1.first_line, @1.first_column, Token::Field);
	 }
