/********************************************************************
 * class for our attraction point
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#include "attractionpoint.h"

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
