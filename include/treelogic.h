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
#include "mat4.h"
#include "vec4.h"
#include "shader.h"

// class for our attraction point
class attractionPoint {
public:
	vec3 position;
	unsigned long closestVertice;

	attractionPoint();
	attractionPoint(float pX, float pY, float pZ);
	attractionPoint(vec3 pPosition);
	attractionPoint(const attractionPoint& pCopy);
	
	attractionPoint& operator=(const attractionPoint& pCopy);
};

// class for a node in our tree
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

// class for a slice
class slice {
public:
	unsigned long p[5];											// 5 vertices to a slice
	
	slice();
	slice(const slice& pCopy);

	slice& operator=(const slice& pCopy);
};

// class for a quad
class quad {
public:
	GLuint v[4];												// index to our vertex buffer
	
	quad();
	quad(const quad& pCopy);
	
	quad& operator=(const quad& pCopy); 
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
	std::vector<quad>					mElements;				// our elements
	
	unsigned long						mLastNumOfVerts;		// number of vertices before we added our last round of nodes
	
	mat4								mProjection;			// our projection matrix
	mat4								mView;					// our view matrix
	mat4								mModel;					// our model matrix
	shader*								mSimpleShader;			// simple shader to render our points
	shader*								mTreeShader;			// shader we use to render our tree
	
	GLuint								mVAO_APoints;			// Vertex array for our attraction points
	GLuint								mVBO_APoints;			// Vertex buffer for our attraction points
	
	bool								mUpdateBuffers;			// Do we need to update our vertex buffer?
	GLuint								mVAO_Tree;				// Our vertex array buffer for our tree
	GLuint								mVBO_Verts;				// Vertex buffer for our vertexs
	GLuint								mVBO_Elements;			// Vertex buffer for our elements
	
	GLuint								mTextureID;				// ID of our texture map

	float randf(float pMin = -1.0f, float pMax = 1.0f);
	unsigned long addVertex(const vec3& pVertex);
	void remVertex(unsigned long pIndex);
	
	slice createSlice(const vec3 pCenter, const vec3 pDir, float pSize, float pDistance);
	void joinTwoSlices(const slice& pA, const slice& pB);
	void joinMultiSlices(long pSliceCount, slice* pSlices);
	void expandChildren(unsigned long pParentNode, const slice& pParentSlice, vec3 pOffset, float pDistance);

protected:
public:	
	// constructors/destructors
	treelogic();
	~treelogic();
	
	// matrixes
	mat4 projection();
	void setProjection(const mat4& pProjection);
	mat4 view();
	void setView(const mat4& pView);
	mat4 model();
	void setModel(const mat4& pModel);
	
	// tree generation code
	unsigned long growBranch(unsigned long pFromVertex, vec3 pTo);
	void generateAttractionPoints(unsigned long pNumOfPoints = 5000, float pOuterRadius = 100.0f, float pInnerRadius = 50.0f, float pAspect = 3.0f, float pOffsetY = 20.0f, bool pClear = true);
	bool doIteration(float pMaxDistance = 75.0f, float pBranchSize = 5.0f, float pCutOffDistance = 10.0f, vec3 pBias = vec3(0.0, 0.0, 0.0));
	void optimiseNodes();
	void createModel();
	
	// shaders
	void initShaders();
	void makeSimpleShader();
	void makeTreeShader();
	
	// rendering
	void render();
};

#endif
