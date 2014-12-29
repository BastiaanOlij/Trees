/********************************************************************
 * treelogic contains the logic to generate our tree
 * 
 * Implementation based on the "Space Colonization Algorithm" by
 * Adam Runions, Brendan Lane, and Przemyslaw Prusinkiewicz
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifndef treelogich
#define treelogich

#define		GLFW_INCLUDE_GL_3
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

// standard libraries we need...
#include <stdlib.h>
#include <math.h>
#include <time.h> 
#include <vector>

#include "vec3.h"

// class for our attraction points
class attractionPoint {
public:
	vec3 position;
	unsigned int closestVertice;

	attractionPoint();
	attractionPoint(float pX, float pY, float pZ);
	attractionPoint(vec3 pPosition);
	attractionPoint(const attractionPoint& pCopy);
	
	attractionPoint& operator=(const attractionPoint& pCopy);
};

// class for a node
class treenode {
public:
	unsigned int a;												// index to our vertex buffer where our node starts
	unsigned int b;												// index to our vertex buffer where our node ends
	int parent;													// index to our node buffer pointing to our parent (-1 if this is a root node)
	unsigned int childcount;									// number of children (including their children)
	
	treenode();
	treenode(unsigned int pA, unsigned int pB, int pParent = -1);
	treenode(const treenode& pCopy);
	
	treenode& operator=(const treenode& pCopy);
};

class treelogic {
private:
	std::vector<attractionPoint>		mAttractionPoints;
	std::vector<vec3>					mVertices;
	std::vector<treenode>				mNodes;
	unsigned int						mLastNumOfVerts;		// number of vertices before we added our last round of nodes
	
	float randf(float pMin = -1.0f, float pMax = 1.0f);

protected:
public:	
	// constructors/destructors
	treelogic();
	~treelogic();
	
	// tree generation code
	unsigned int growBranch(unsigned int pFromVertex, vec3 pTo);
	void generateAttractionPoints(unsigned int pNumOfPoints = 5000, float pOuterRadius = 100.0f, float pInnerRadius = 50.0f, float pAspect = 3.0f, float pOffsetY = 20.0f, bool pClear = true);
	bool doIteration(float pMaxDistance = 75.0f, float pBranchSize = 5.0f, float pCutOffDistance = 10.0f, vec3 pBias = vec3(0.0, 0.0, 0.0));
	
	void optimiseNodes();
	
	// rendering
	void render();
};

#endif
