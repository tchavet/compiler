#ifndef IOClass_H
#define IOClass_H

#include "../nodes/ClassNode.hpp"

class IOClass
{
public:
	IOClass();
	ClassNode* getClassNode();

protected:
	ClassNode* classNode;
};
#endif
