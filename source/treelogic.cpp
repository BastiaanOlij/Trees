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
// class for our vector
/////////////////////////////////////////////////////////////////////

vec3::vec3(){
	x = 0;
	y = 0;
	z = 0;	
};

vec3::vec3(float pX, float pY, float pZ) {
	x = pX;
	y = pY;
	z = pZ;	
};

vec3::vec3(const vec3& pCopy) {
	x = pCopy.x;
	y = pCopy.y;
	z = pCopy.z;
};

float vec3::length() {
	float l = (x * x) + (y * y) + (z * z);
	if (l==0.0f) {
		return 0.0f;
	} else {
		return sqrt(l);
	};
};

vec3 vec3::normalize() {
	float l = length();
	if (l == 0.0f) {
		return vec3(0.0f, 1.0f, 0.0f);
	} else {
		return vec3(x / l, y / l, z / l);
	};
};

vec3& vec3::operator=(const vec3 &pCopy) {
	x = pCopy.x;
	y = pCopy.y;
	z = pCopy.z;
	return (*this);
};

vec3& vec3::operator+=(const vec3& pAdd) {
	x += pAdd.x;
	y += pAdd.y;
	z += pAdd.z;
	return (*this);
};

vec3& vec3::operator-=(const vec3& pSub) {
	x -= pSub.x;
	y -= pSub.y;
	z -= pSub.z;
	return (*this);	
};

vec3& vec3::operator*=(float pMult) {
	x *= pMult;
	y *= pMult;
	z *= pMult;
	return (*this);
};

vec3& vec3::operator/=(float pDiv) {
	if (pDiv == 0.0) {
		// divide by 0, really should throw an exception error
		return (*this);			
	} else if (pDiv == 1.0) {
		// don't waste the time...
		return (*this);			
	};
	
	x /= pDiv;
	y /= pDiv;
	z /= pDiv;
	
	return (*this);	
};

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
};

treenode::treenode(unsigned int pA, unsigned int pB) {
	a = pA;
	b = pB;
};

treenode::treenode(const treenode& pCopy) {
	a = pCopy.a;
	b = pCopy.b;
};

treenode& treenode::operator=(const treenode& pCopy) {
	a = pCopy.a;
	b = pCopy.b;
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
	// add our new vertice
	mVertices.push_back(pTo);
	
	// add our node
	mNodes.push_back(treenode(pFromVertex, mVertices.size()-1));
	
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
		point = point.normalize();
		
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
		for (v = point.closestVertice + 1; v < mVertices.size(); v++) {
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
				directions[point.closestVertice] += norm.normalize();				
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
			directions[v] = directions[v].normalize() * pBranchSize;				
			vert += directions[v] + pBias;
			
			growBranch(v, vert);			
		};
	};
	
	// as long as we still have attraction points left we must still be growing our tree
	return mAttractionPoints.size() > 0; 
};

/////////////////////////////////////////////////////////////////////
// rendering
/////////////////////////////////////////////////////////////////////

void treelogic::render() {
	unsigned int i;
	// for now, we go slowpoke OpenGl 1.0....
	
	// first our (remaining) attraction points
	glBegin(GL_POINTS);
	glPointSize(2.0f);
    glColor3f(0.0f, 1.0f, 0.0f); // green
	for (i = 0; i < mAttractionPoints.size(); i++) {
		vec3 position = mAttractionPoints[i].position;
        glVertex3f(position.x, position.y, position.z);
	};
	glPointSize(1.0f);
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
};
