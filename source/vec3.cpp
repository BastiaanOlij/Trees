/********************************************************************
 * 3 dimensional vector class
 * 
 * Note that it can be accessed as x,y,z or r,g,b
 *
 * By Bastiaan Olij - 2014
********************************************************************/

#include "vec3.h"

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

vec3::vec3(const vec2& pCopy, float pZ) {
	x = pCopy.x;
	y = pCopy.y;
	z = pZ;
};

vec3::vec3(const vec3& pCopy) {
	x = pCopy.x;
	y = pCopy.y;
	z = pCopy.z;
};

float vec3::length() const {
	float l = (x * x) + (y * y) + (z * z);
	if (l==0.0f) {
		return 0.0f;
	} else {
		return sqrt(l);
	};
};

vec3 vec3::normalized() const {
	float l = length();
	if (l < 0.0001f) {
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

float vec3::operator%(const vec3 &pWith) const {
	return (this->x * pWith.x) + (this->y * pWith.y) + (this->z * pWith.z);
};

vec3 vec3::operator*(const vec3 &pCross) const {
	return vec3((y*pCross.z) - (z * pCross.y) , (z*pCross.x) - (x * pCross.z) , (x*pCross.y) - (y * pCross.x));
};
