/********************************************************************
 * 3 dimensional vector class
 *
 * Note that it can be accessed as x,y,z or r,g,b
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifndef vec3h
#define vec3h

#include <math.h>
#include "vec2.h"

// class for our vector
class vec3 {
public:
	union { float x; float r; };
	union { float y; float g; };
	union { float z; float b; };
	
	vec3();
	vec3(float pX, float pY, float pZ);
	vec3(const vec3& pCopy);
	
	inline vec2 xy() {
		return vec2(x, y);
	};
	inline vec2 xz() {
		return vec2(x, z);
	};
	inline vec2 yz() {
		return vec2(y, z);
	};

	float length() const;					// returns the lenght of our vector
	vec3 normalized() const;				// returns a unit vector for our vector (|N|)
	vec3& operator=(const vec3& pCopy);		// copy operator
	vec3& operator+=(const vec3& pAdd);		// adds a vector to our vector
	inline vec3 operator+(const vec3 &pAdd) const {
		vec3 copy = *this;
		copy += pAdd;
		return copy;
	};
	vec3& operator-=(const vec3& pSub);		// substracts a vector from our vector
	inline vec3 operator-(const vec3 &pSub) const {
		vec3 copy = *this;
		copy -= pSub;
		return copy;
	};
	vec3& operator*=(float pMult);			// multiply our vector with a scalar
	inline vec3 operator*(float pMult) const {
		vec3 copy = *this;
		copy *= pMult;
		return copy;
	};
	vec3& operator/=(float pDiv);			// divide our vector with a scalar
	inline vec3 operator/(float pDiv) const {
		vec3 copy = *this;
		copy /= pDiv;
		return copy;
	};

	float operator%(const vec3 &pWith) const;	// return dot product with a second vector
	vec3 operator*(const vec3 &pCross) const;	// cross product of two vectors
	
};

#endif
