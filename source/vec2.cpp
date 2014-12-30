/********************************************************************
 * 2 dimensional vector class
 * 
 * Note that it can be accessed as x,y or u,v or s,t
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#include "vec2.h"

vec2::vec2(){
	x = 0;
	y = 0;
};

vec2::vec2(float pX, float pY) {
	x = pX;
	y = pY;
};

vec2::vec2(const vec2& pCopy) {
	x = pCopy.x;
	y = pCopy.y;
};

float vec2::length() const {
	float l = (x * x) + (y * y);
	if (l==0.0f) {
		return 0.0f;
	} else {
		return sqrt(l);
	};
};

vec2 vec2::normalized() const {
	float l = length();
	if (l < 0.0001f) {
		return vec2(0.0f, 1.0f);
	} else {
		return vec2(x / l, y / l);
	};
};

vec2& vec2::operator=(const vec2 &pCopy) {
	x = pCopy.x;
	y = pCopy.y;
	return (*this);
};

vec2& vec2::operator+=(const vec2& pAdd) {
	x += pAdd.x;
	y += pAdd.y;
	return (*this);
};

vec2& vec2::operator-=(const vec2& pSub) {
	x -= pSub.x;
	y -= pSub.y;
	return (*this);	
};

vec2& vec2::operator*=(float pMult) {
	x *= pMult;
	y *= pMult;
	return (*this);
};

vec2& vec2::operator/=(float pDiv) {
	if (pDiv == 0.0) {
		// divide by 0, really should throw an exception error
		return (*this);			
	} else if (pDiv == 1.0) {
		// don't waste the time...
		return (*this);			
	};
	
	x /= pDiv;
	y /= pDiv;
	
	return (*this);	
};

float vec2::operator%(const vec2 &pWith) {
	return (this->x * pWith.x) + (this->y * pWith.y);
};

