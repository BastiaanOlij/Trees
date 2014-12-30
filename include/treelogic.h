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

#include "vec2.h"
#include "vec3.h"

// class for our attraction point
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

// class for a node in our tree
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

// class for a slice
class slice {
public:
	unsigned int	p[5];										// 5 vertices to a slice
	
	slice();
	slice(const slice& pCopy);
		
};

// Our treelogic class, note that after we are finished only mVertices, mNormals, mTexCoords and mElements are relevant
class treelogic {
private:
	std::vector<attractionPoint>		mAttractionPoints;		// our attraction points
	std::vector<vec3>					mVertices;				// vertices that make up our tree
	std::vector<vec3>					mNormals;				// normals for our vertice
	std::vector<vec2>					mTexCoords;				// texture coordinates
	std::vector<treenode>				mNodes;					// nodes used to construct our tree skeleton
	std::vector<slice>					mSlices;				// slices that form the basis of
	
	unsigned int						mLastNumOfVerts;		// number of vertices before we added our last round of nodes

	float randf(float pMin = -1.0f, float pMax = 1.0f);
	unsigned int addVertex(const vec3& pVertex);
	void remVertex(unsigned int pIndex);

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
	void createModel();
	
	// rendering
	void render();
};

#endif
