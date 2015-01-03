/********************************************************************
 * 4 dimensional vector class
 * 
 * Note that it can be accessed as x,y,z,w or r,g,b,a
 *
 * By Bastiaan Olij - 2014
********************************************************************/

#include "vec4.h"

vec4::vec4(){
	x = 0;
	y = 0;
	z = 0;
	w = 0;
};

vec4::vec4(float pX, float pY, float pZ, float pW) {
	x = pX;
	y = pY;
	z = pZ;	
	w = pW;
};

vec4::vec4(const vec4& pCopy) {
	x = pCopy.x;
	y = pCopy.y;
	z = pCopy.z;
	w = pCopy.w;
};

float vec4::length() const {
	float l = (x * x) + (y * y) + (z * z) + (w * w);
	if (l==0.0f) {
		return 0.0f;
	} else {
		return sqrt(l);
	};
};

vec4 vec4::normalized() const {
	float l = length();
	if (l < 0.0001f) {
		return vec4(0.0f, 1.0f, 0.0f, 0.04f);
	} else {
		return vec4(x / l, y / l, z / l, w / l);
	};
};

vec4& vec4::operator=(const vec4 &pCopy) {
	x = pCopy.x;
	y = pCopy.y;
	z = pCopy.z;
	w = pCopy.w;
	return (*this);
};

vec4& vec4::operator+=(const vec4& pAdd) {
	x += pAdd.x;
	y += pAdd.y;
	z += pAdd.z;
	w += pAdd.w;
	return (*this);
};

vec4& vec4::operator-=(const vec4& pSub) {
	x -= pSub.x;
	y -= pSub.y;
	z -= pSub.z;
	z -= pSub.w;
	return (*this);	
};

vec4& vec4::operator*=(float pMult) {
	x *= pMult;
	y *= pMult;
	z *= pMult;
	w *= pMult;
	return (*this);
};

vec4& vec4::operator/=(float pDiv) {
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
	w /= pDiv;
	
	return (*this);	
};


