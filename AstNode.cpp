#include "AstNode.hpp"
#include <string>
#include <sstream>

#include <iostream>

AstNode::AstNode(Token *tokenVal)
{
	children = std::vector<AstNode*>();
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
	childrenNb += nodes->size();
}

std::vector<AstNode*> AstNode::getChildren()
{
	return children;
}

std::string AstNode::printTree(int tabs)
{
	std::string print = "";
	std::string tabsStr(tabs*2, ' ');
	std::string tab = "  ";
	
	if(token == NULL)
	{
		print += "[\n";
		for(int i=0; i<childrenNb; ++i)
		{
			print += tabsStr + tab + children[i]->printTree(tabs+1);
			if(i != childrenNb-1)
				print += ",";
			print += "\n";
		}
		print += tabsStr + "]";
		return print;
	}

	enum Token::Type type = token->getType();
	
	if(type == Token::Block && childrenNb > 1)
	{
		print += "[\n";
		for(int i=0; i<childrenNb; ++i)
		{
			print += tabsStr + tab + children[i]->printTree(tabs+1);
			if(i != childrenNb-1)
				print += ",";
			print += "\n";
		}
		print += tabsStr + "]";
	}
	
	else if(type == Token::Class)
	{
		print += "Class(" + children[0]->printTree() + "," + children[1]->printTree() + ",\n"
			+ tabsStr + tab + children[2]->printTree(tabs+1) + ",\n"
			+ tabsStr + tab + children[3]->printTree(tabs+1) + "\n"
			+ tabsStr + ")";
	}

	else if(type == Token::Type)
	{
		print += token->getStringValue();
	}

	else if(type == Token::Field)
	{
		print += "Field(" + children[0]->printTree() + "," + children[1]->printTree();

		if(childrenNb == 3)
		{
			print += ",\n"
				+ tabsStr + tab + children[2]->printTree(tabs+1); 
		} 

		print += "\n"
			+ tabsStr + ")";
	}

	else if(type == Token::Method)
	{
		print += "Method(" + children[0]->printTree() + ",\n"
			+ tabsStr + tab + children[1]->printTree(tabs+1) + ",\n"
			+ tabsStr + tab + children[2]->printTree() + ",\n"
			+ tabsStr + tab + children[3]->printTree(tabs+1) + "\n"
			+ tabsStr + ")";
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

	else if(type == Token::Formal)
	{
		print += children[0]->printTree() + " : " + children[1]->printTree();
	}	

	else if(type == Token::Block)
	{
		print += children[0]->printTree(tabs);
	}
	
	else if(type == Token::If)
	{
		print += "If(\n"
			+ tabsStr + tab + children[0]->printTree(tabs+1) + ",\n"
			+ tabsStr + tab + children[1]->printTree(tabs+1);

		if(childrenNb == 3)
		{
			print += ",\n"
				+ tabsStr + tab + children[2]->printTree(tabs+1); 
		} 

		print += "\n"
			+ tabsStr + ")";
	}
	
	else if(type == Token::While)
	{
		print += "While(\n"
			+ tabsStr + tab + children[0]->printTree(tabs+1) + ",\n"
			+ tabsStr + tab + children[1]->printTree(tabs+1) + "\n"
			+ tabsStr + ")";
	}	

	else if(type == Token::Let)
	{
		print += "Let(" + children[0]->printTree() + children[1]->printTree() + ",\n"
			+ tabsStr + tab + children[2]->printTree(tabs+1);

		if(childrenNb == 4)
		{
			print += ",\n"
				+ tabsStr + tab + children[3]->printTree(tabs+1); 
		} 

		print += "\n"
			+ tabsStr + ")";
	}
	
	else if(type == Token::Assign)
	{
		print += "Assign(" + children[0]->printTree() + ",\n"
			+ tabsStr + tab + children[1]->printTree(tabs+1) + "\n"
			+ tabsStr + ")";
	}	

	else if(type == Token::UnOp)
	{
		print += "UnOp(" + token->getStringValue() + ",\n"
			+ tabsStr + tab + children[0]->printTree(tabs+1) + "\n"
			+ tabsStr + ")"; 
	}
	
	else if(type == Token::BinOp)
	{
		print += "BinOp(" + token->getStringValue() + ",\n" 
			+ tabsStr + tab + children[0]->printTree(tabs+1) + ",\n"
			+ tabsStr + tab + children[1]->printTree(tabs+1) + "\n"
			+ tabsStr + ")";
	}
	
	else if(type == Token::Call)
	{
		print += "Call(\n" 
			+ tabsStr + tab + children[0]->printTree(tabs+1) + ",\n" 
			+ tabsStr + tab + children[1]->printTree(tabs+1) + ",\n" 
			+ tabsStr + tab + children[2]->printTree(tabs+1) + "\n"
			+ tabsStr + ")";
	}
	
	else if(type == Token::New)
	{
		print += "New(" + children[0]->printTree() + ")";
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
