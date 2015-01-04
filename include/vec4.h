/********************************************************************
 * 4 dimensional vector class
 *
 * Note that it can be accessed as x,y,z,w or r,g,b,a
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifndef vec4h
#define vec4h

#include <math.h>
#include "vec3.h"

// class for our vector
class vec4 {
public:
	union { float x; float r; };
	union { float y; float g; };
	union { float z; float b; };
	union { float w; float a; };
	
	vec4();
	vec4(float pX, float pY, float pZ, float pW);
	vec4(const vec3& pCopy, float pW = 1.0f);
	vec4(const vec4& pCopy);
	
	inline vec3 xyz() const {
		return vec3(x, y, z);			
	};
	
	float length() const;					// returns the lenght of our vector
	vec4 normalized() const;				// returns a unit vector for our vector (|N|)
	vec4& operator=(const vec4& pCopy);		// copy operator
	vec4& operator+=(const vec4& pAdd);		// adds a vector to our vector
	inline vec4 operator+(const vec4 &pAdd) const {
		vec4 copy = *this;
		copy += pAdd;
		return copy;
	};
	vec4& operator-=(const vec4& pSub);		// substracts a vector from our vector
	inline vec4 operator-(const vec4 &pSub) const {
		vec4 copy = *this;
		copy -= pSub;
		return copy;
	};
	vec4& operator*=(float pMult);			// multiply our vector with a scalar
	inline vec4 operator*(float pMult) const {
		vec4 copy = *this;
		copy *= pMult;
		return copy;
	};
	vec4& operator/=(float pDiv);			// divide our vector with a scalar
	inline vec4 operator/(float pDiv) const {
		vec4 copy = *this;
		copy /= pDiv;
		return copy;
	};
};

#endif
