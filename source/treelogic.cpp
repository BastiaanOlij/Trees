/********************************************************************
 * treelogic contains the logic to generate our tree
 * 
 * Implementation based on the "Space Colonization Algorithm" by
 * Adam Runions, Brendan Lane, and Przemyslaw Prusinkiewicz
 * 
 * By Bastiaan Olij - 2014
********************************************************************/


#include "treelogic.h"

/////////////////////////////////////////////////////////////////////
// class for our attraction points
/////////////////////////////////////////////////////////////////////

attractionPoint::attractionPoint() {
	position.x = 0;
	position.y = 0;
	position.z = 0;
	closestVertice = 0;
};

attractionPoint::attractionPoint(float pX, float pY, float pZ) {
	position.x = pX;
	position.y = pY;
	position.z = pZ;
	closestVertice = 0;
};

attractionPoint::attractionPoint(vec3 pPosition) {
	position = pPosition;
	closestVertice = 0;
};

attractionPoint::attractionPoint(const attractionPoint& pCopy) {
	position = pCopy.position;
	closestVertice = pCopy.closestVertice;
};

attractionPoint& attractionPoint::operator=(const attractionPoint& pCopy) {
	position = pCopy.position;
	closestVertice = pCopy.closestVertice;
	return (*this);
};

/////////////////////////////////////////////////////////////////////
// class for a node
/////////////////////////////////////////////////////////////////////

treenode::treenode() {
	a = 0;
	b = 0;
	parent = -1;
	childcount = 0;
};

treenode::treenode(unsigned int pA, unsigned int pB, int pParent) {
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


/////////////////////////////////////////////////////////////////////
// TreeLogic
//
// constructors/destructors
/////////////////////////////////////////////////////////////////////

/**
 * treelogic(pTrunkNode)
 * 
 * constructor for our treelogic, requires the initial vector for our first trunk node
 **/
treelogic::treelogic() {
	// set some defaults
	mLastNumOfVerts	= 1;
	
	// add our root vertex
	mVertices.push_back(vec3(0.0, 0.0, 0.0)); // our tree "root"
};

treelogic::~treelogic() {
	
};

/////////////////////////////////////////////////////////////////////
// helpers
/////////////////////////////////////////////////////////////////////

/**
 * randf(pMin, pMax)
 *
 * This method generates a random floating point number between min
 * and max
 **/
float treelogic::randf(float pMin, float pMax) {
	float randomNumber;
	
	randomNumber = rand() % 100000; // random number between 0 and 100000
	randomNumber /= 100000; // scale to 0.0 - 1.0
	randomNumber *= (pMax - pMin); // scale to our delta
	
	return randomNumber + pMin; // and return a number between min and max
};

/////////////////////////////////////////////////////////////////////
// tree generation code
/////////////////////////////////////////////////////////////////////

/**
 * growBrach(pFromVertex, pTo)
 *
 * This method will grow a branch from a given vertex
 *
 * pFromVertex - index of vertex to grow from
 * pTo         - location of new vertex to branch to
 *
 **/
unsigned int treelogic::growBranch(unsigned int pFromVertex, vec3 pTo) {
	int	parent = -1;
	
	// Find our parent
	for (unsigned int p = 0; p < mNodes.size() && parent == -1; p++) {
		if (mNodes[p].b == pFromVertex) {
			parent = p;
		};
	};
	
	if (parent != -1) {
		// check our vector from our parent
		vec3 parentVector = mVertices[mNodes[parent].b] - mVertices[mNodes[parent].a];
		parentVector = parentVector.normalized();
		
		vec3 toVector = pTo - mVertices[mNodes[parent].b];
		toVector = toVector.normalized();
		
		// check if we're backtracking, this can happen if we're "trapped" between two equal distanced but opposite attraction points
		if ((parentVector % toVector) < -0.8) {
			// use a cross product of the two vectors 
			pTo = mVertices[mNodes[parent].b] + (parentVector * toVector);
		};		
	};
	
	// add our new vertice
	mVertices.push_back(pTo);
	
	// add our node
	mNodes.push_back(treenode(pFromVertex, mVertices.size()-1, parent));
	
	// now update our count
	while (parent != -1) {
		mNodes[parent].childcount++;
		parent = mNodes[parent].parent;
	};
	
	return mVertices.size()-1;
};

/**
 * generateAttractionPoints(pCount, pNumOfPoints, pOuterRadius, pInnerRadius, pAspect, pOffsetY)
 * 
 * This method generates the attaction points for our tree. 
 * At this moment we've only got a single very simple generation of points based on a stretched hemisphere filled with random points.
 * The shape of our point cloud very much determines the look of our tree. 
 * Adding more complexity to this algorithm to steer the shape of the point cloud will become a target later on.
 *
 * pNumOfPoints - Number of attraction points (N)
 * pOuterRadius - Outer size of our point cloud
 * pInnerRadius - Inner size of our point cloud
 * pAspect      - Aspect ratio between height and width
 * pOffsetY     - Y offset
 * pClear    	- Clears our attraction points first
 **/
void treelogic::generateAttractionPoints(unsigned int pNumOfPoints, float pOuterRadius, float pInnerRadius, float pAspect, float pOffsetY, bool pClear) {
	// Seed our randomiser
	srand (time(NULL));
	
	if (pClear) {
		// Clear any existing points (shouldn't be any..)
		mAttractionPoints.clear();		
	};
	
	// Add random attraction points until we reached our goal
	for (unsigned int i = 0; i < pNumOfPoints; i++) {
		vec3 point;
		
		// random normalized vector for half a hemisphere
		point.x = randf();
		point.y = randf(0.0f, 1.0f);
		point.z = randf();		
		point = point.normalized();
		
		// Scale it up to a random radius and stretch if needed
		point *= ((pOuterRadius - pInnerRadius) * randf(0.0f, 1.0f)) + pInnerRadius;
		point.y *= pAspect;
		point.y += pOffsetY;
		
		// and add it to our buffer
		mAttractionPoints.push_back(attractionPoint(point));
	};
};

/**
 * doIteration(pCutOffDistance, pBranchSize)
 * 
 * This method performs one iteration of our tree generation logic and returns true if changes have been made.
 * Basically you should repeatidly call this method until it returns false
 * 
 * pMaxDistance    - maximum distance between attraction point and vertice for it to be considered (must be > pInnerRadius) (dk)
 * pBranchSize     - size with which we grow a branch (D)
 * pCutOffDistance - once the closest distance to an attraction point and a vertice becomes less then this we remove the attraction point (di, must be a multiple of pBranchSize)
 * pBias           - vector to add to simulate the effect the direction of growth
 **/
bool treelogic::doIteration(float pMaxDistance, float pBranchSize, float pCutOffDistance, vec3 pBias) {
	unsigned int numVerts = mVertices.size(); // need to know the number of vertices at the start of our process
	unsigned int i, v;
	std::vector<float> numOfAPoints;
	std::vector<vec3> directions;
	
	// init our temporary buffers
	for (v = 0; v < numVerts; v++) {
		numOfAPoints.push_back(0.0);
		directions.push_back(vec3(0.0f, 0.0f, 0.0f));
	};
	
	// find out what our closest vertice to each attraction points is:
	i = 0;
	while (i < mAttractionPoints.size()) { // use a while loop, we'll be removing points..
		attractionPoint point = mAttractionPoints[i];
		
		// start with our current distance for our attraction point
		vec3 delta = mVertices[point.closestVertice] - point.position;
		float currentDistance = delta.length();
		
		// as our vertices haven't moved we only need to check any new vertices
		for (v = mLastNumOfVerts; v < mVertices.size(); v++) {
			delta = mVertices[v] - point.position;
			float distance = delta.length();
			if (distance < currentDistance) {
				// this one is now our closest
				point.closestVertice = v;
				currentDistance = distance;
			};
		};
		
		if (currentDistance < pCutOffDistance) {
			// we're done with this one...
			mAttractionPoints.erase(mAttractionPoints.begin() + i);
		} else {
			// copy back our new closest vertice and advance...
			mAttractionPoints[i].closestVertice = point.closestVertice;
			
			if (currentDistance < pMaxDistance) {
				// count our vertice
				numOfAPoints[point.closestVertice] += 1.0;
				vec3 norm = mAttractionPoints[i].position - mVertices[point.closestVertice];
				directions[point.closestVertice] += norm.normalized();				
			};
			
			// and advance
			i++;
		};
	};
	
	// Update our last number of vertices
	mLastNumOfVerts = numVerts;
	
	// Now check which vertices need to branch out...
	for (v = 0; v < numVerts; v++) {		
		if (numOfAPoints[v] > 0.0) {
			vec3	vert = mVertices[v];
			directions[v] /= numOfAPoints[v];
			directions[v] = directions[v].normalized() * pBranchSize;				
			vert += directions[v] + pBias;
			
			growBranch(v, vert);			
		};
	};
	
	// as long as we still have attraction points left we must still be growing our tree
	return mAttractionPoints.size() > 0; 
};

/**
 * optimiseNodes()
 *
 * This method will optimise nodes by joining nodes with small angles between them 
 * Note that this will invalidate our childcount, we won't update this rather leave
 * it up to the implementation whether to recount it or use the original counts
 *
 * 
 **/
void treelogic::optimiseNodes() {
	unsigned int node = 0;
	std::vector<unsigned int> children;
	
	while (node < mNodes.size()) {
		unsigned int mergeWith = 0;
		
		// first we need to find out how many children we have, we can only optimise if just one is found
		children.clear();
		for (unsigned int n = node+1; n < mNodes.size(); n++) {
			if (mNodes[n].parent == node) {
				children.push_back(n);
			};
		};
		
		// only one child? check if we need to merge
		if (children.size() == 1) {
			vec3	parentVector = mVertices[mNodes[node].b] - mVertices[mNodes[node].a];
			vec3	childVector = mVertices[mNodes[children[0]].b] - mVertices[mNodes[children[0]].a];
			
			// normalize our vectors
			parentVector = parentVector.normalized();
			childVector = childVector.normalized();
			
			// use dot product, this gives our cosine, the closer to 1.0 the more the vectors match direction
			if ((parentVector % childVector) > 0.97) {
				mergeWith = children[0];
			};
		};
		
		// and merge
		if (mergeWith!=0) {
			unsigned int eraseVertice = mNodes[node].b; // should be same as mNodes[mergeWith].a, this we'll erase..
			
			// merge our nodes...
			mNodes[mergeWith].a = mNodes[node].a;
			mNodes[mergeWith].childcount = mNodes[node].childcount;
			mNodes[mergeWith].parent = mNodes[node].parent;
			mNodes.erase(mNodes.begin() + node);
			
			// erase our vertice
			mVertices.erase(mVertices.begin() + eraseVertice);
			
			// adjust our nodes
			for (unsigned int n = 0; n < mNodes.size(); n++) {
				if (mNodes[n].parent > node)mNodes[n].parent--;
				if (mNodes[n].a > eraseVertice) mNodes[n].a--;
				if (mNodes[n].b > eraseVertice) mNodes[n].b--;
			};
		} else {
			node++;
		};
	};
};


/////////////////////////////////////////////////////////////////////
// rendering
/////////////////////////////////////////////////////////////////////

void treelogic::render() {
	unsigned int i;
	// for now, we go slowpoke OpenGl 1.0....
	
	// first our (remaining) attraction points
	glPointSize(2.0f);
	glBegin(GL_POINTS);
    glColor3f(0.0f, 1.0f, 0.0f); // green
	for (i = 0; i < mAttractionPoints.size(); i++) {
		vec3 position = mAttractionPoints[i].position;
        glVertex3f(position.x, position.y, position.z);
	};
	glEnd();

	// also draw points for our vertices to highlight them
	glPointSize(3.0f);
	glBegin(GL_POINTS);
    glColor3f(170.0f / 256.0f, 80.0f / 256.0f, 0.0f); // brown
	for (i = 0; i < mVertices.size(); i++) {
		vec3 position = mVertices[i];
        glVertex3f(position.x, position.y, position.z);		
	};
	glEnd();
	
	// now draw the tree as far as we've build it...
	glBegin(GL_LINES);
    glColor3f(204.0f / 256.0f, 102.0f / 256.0f, 0.0f); // brown
	for (i = 0; i < mNodes.size(); i++) {
		treenode node = mNodes[i];
		vec3 positionA = mVertices[node.a];
		vec3 positionB = mVertices[node.b];
		
        glVertex3f(positionA.x, positionA.y, positionA.z);
        glVertex3f(positionB.x, positionB.y, positionB.z);
	};
	glEnd();
		
	// reset our point size
	glPointSize(1.0f);	
};
