/********************************************************************
 * class for a node in our tree
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#include "treenode.h"

treenode::treenode() {
	a = 0;
	b = 0;
	parent = -1;
	childcount = 0;
};

treenode::treenode(unsigned long pA, unsigned long pB, long pParent) {
	a = pA;
	b = pB;
	parent = pParent;
	childcount = 0;
};

treenode::treenode(const treenode& pCopy) {
	a = pCopy.a;
	b = pCopy.b;
	parent = pCopy.parent;
	childcount = pCopy.childcount;
};

treenode& treenode::operator=(const treenode& pCopy) {
	a = pCopy.a;
	b = pCopy.b;
	parent = pCopy.parent;
	childcount = pCopy.childcount;
	return (*this);
};
