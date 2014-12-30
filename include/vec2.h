/********************************************************************
 * 2 dimensional vector class
 * 
 * Note that it can be accessed as x,y or u,v or s,t
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifndef vec2h
#define vec2h

#include <math.h>

// class for our vector
class vec2 {
public:
	union { float x; float u; float s; };
	union { float y; float v; float t; };
	
	vec2();
	vec2(float pX, float pY);
	vec2(const vec2& pCopy);

	float length() const;					// returns the lenght of our vector
	vec2 normalized() const;				// returns a unit vector for our vector (|N|)
	vec2& operator=(const vec2& pCopy);		// copy operator
	vec2& operator+=(const vec2& pAdd);		// adds a vector to our vector
	inline vec2 operator+(const vec2 &pAdd) {
		vec2 copy = *this;
		copy += pAdd;
		return copy;
	};
	vec2& operator-=(const vec2& pSub);		// substracts a vector from our vector
	inline vec2 operator-(const vec2 &pSub) {
		vec2 copy = *this;
		copy -= pSub;
		return copy;
	};
	vec2& operator*=(float pMult);			// multiply our vector with a scalar
	inline vec2 operator*(float pMult) {
		vec2 copy = *this;
		copy /= pMult;
		return copy;
	};
	vec2& operator/=(float pDiv);			// divide our vector with a scalar
	inline vec2 operator/(float pDiv) {
		vec2 copy = *this;
		copy /= pDiv;
		return copy;
	};

	float operator%(const vec2 &pWith);		// return dot product with a second vector	
};

#endif
