#include "AstNode.hpp"
#include <string>
#include <sstream>

AstNode::AstNode(Token *tokenVal)
{
	token = tokenVal;
	childrenNb = 0;
}

void AstNode::addNode(AstNode *child)
{
	children.push_back(child);
	childrenNb++;
}

void AstNode::addNodes(std::vector<AstNode*>* nodes)
{
	children.insert(children.end(), nodes->begin(), nodes->end());
}

std::vector<AstNode*> AstNode::getChildren()
{
	return children;
}

std::string AstNode::printTree()
{
	std::vector<AstNode*> children = getChildren();
	int nbChildren = children.size();
	
	std::string print = "";
	
	if(token == NULL)
	{
		print = "[";
		for(int i=0; i<nbChildren; ++i)
		{
			if(i != 0)
			{
				print += ", ";
			}
	
			print += children[i]->printTree();
		}
		print += "]";
		return print;
	}

	enum Token::Type type = token->getType();
	
	if(type == Token::Block && nbChildren > 1)
	{
		print = "[";
		for(int i=0; i<nbChildren; ++i)
		{
			if(i != 0)
			{
				print += ", ";
			}
	
			print += children[i]->printTree();
		}
		print += "]";
	}
	
	else if(type == Token::Class)
	{
		print += "Class(";
		
		for(int i=0; i<nbChildren; ++i)
		{
			if(i != 0)
			{
				print += ", ";
			}
	
			print += children[i]->printTree();
		}
		print += ")";
			
	}

	else if(type == Token::Type)
	{
		print += token->getStringValue();
	}

	else if(type == Token::Field)
	{
		print += "Field("
			+ children[0]->printTree() 
			+ ", "
			+ children[1]->printTree();

		if(nbChildren == 3)
		{
			print += ", " + children[2]->printTree(); 
		} 

		print += ")";
	}

	else if(type == Token::Method)
	{
		print += "Method(";
		
		for(int i=0; i<nbChildren; ++i)
		{
			if(i != 0)
			{
				print += ", ";
			}
	
			print += children[i]->printTree();
		}
		print += ")"; 
	}
	else if(type == Token::Object)
	{
		print += token->getStringValue();
	}	


	else if(type == Token::String)
	{
		print += "\"" + token->getStringValue(); + "\"";
	}
	
	else if(type == Token::Object_id)
	{
		print += token->getStringValue();
	}	

	else if(type == Token::Type_id)
	{
		print += token->getStringValue();
	}	

	else if(type == Token::Expr)
	{
		
	}
	
	else if(type == Token::Formal)
	{
		print += children[0]->printTree() + " : " + children[1]->printTree();
	}	

	else if(type == Token::Block)
	{
		print += children[0]->printTree();
	}
	
	else if(type == Token::If)
	{
		print += "If("
			+ children[0]->printTree()
			+ ", "
			+ children[1]->printTree();

		if(nbChildren == 3)
		{
			print += ", " + children[2]->printTree(); 
		} 

		print += ")";
	}
	
	else if(type == Token::While)
	{
		print += "While(" + children[0]->printTree() + ", " + children[1]->printTree() + ")";
	}	

	else if(type == Token::Let)
	{
		print += "Let("
			+ children[0]->printTree()
			+ ", "
			+ children[1]->printTree()
			+ ", "
			+ children[2]->printTree();

		if(nbChildren == 4)
		{
			print += ", " + children[3]->printTree(); 
		} 

		print += ")";
	}
	
	else if(type == Token::Assign)
	{
		print += "Assign(" + children[0]->printTree() + ", " + children[1]->printTree() + ")";
	}	

	else if(type == Token::UnOp)
	{
		print += "UnOp(" + token->getStringValue() + ", " + children[0]->printTree() + ")"; 
	}
	
	else if(type == Token::BinOp)
	{
		print += "BinOp(" 
			+ children[0]->printTree() + ", " 
			+ children[1]->printTree() + ", "
			+ children[2]->printTree() + ")";
	}
	
	else if(type == Token::Call)
	{
		print += "Call(" 
			+ children[0]->printTree() + ", " 
			+ children[1]->printTree() + ", " 
			+ children[2]->printTree() + ")";
	}
	
	else if(type == Token::New)
	{
		print += "New("
			+ children[0]->printTree() + ")";
	}
	
	else if(type == Token::Arg)
	{
		print += children[0]->printTree() + " : " + children[1]->printTree();
	}
	
	else if(type == Token::String_lit)
	{
		print += token->getStringValue(); 
	}

	else if(type == Token::Int_lit)
	{
		std::stringstream ss;
		ss << token->getIntValue();
		print += ss.str();
	}
	
	else if(type == Token::Bool_lit)
	{
		print += token->getStringValue(); 
	}

	return print;
}
