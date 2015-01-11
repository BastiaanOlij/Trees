/********************************************************************
 * class for our attraction point
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifndef attractionpointh
#define attractionpointh

#include "vec3.h"

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

#endif
