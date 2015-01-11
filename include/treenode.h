/********************************************************************
 * class for a node in our tree
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifndef treenodeh
#define treenodeh

class treenode {
public:
	unsigned long a;											// index to our vertex buffer where our node starts
	unsigned long b;											// index to our vertex buffer where our node ends
	long parent;												// index to our node buffer pointing to our parent (-1 if this is a root node)
	unsigned long childcount;									// number of children (including their children)
	
	treenode();
	treenode(unsigned long pA, unsigned long pB, long pParent = -1);
	treenode(const treenode& pCopy);
	
	treenode& operator=(const treenode& pCopy);
};

#endif
