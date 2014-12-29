/********************************************************************
 * 3 dimensional vector class
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifndef vec3h
#define vec3h

#include <math.h>

// class for our vector
class vec3 {
public:
	float x;
	float y;
	float z;
	
	vec3();
	vec3(float pX, float pY, float pZ);
	vec3(const vec3& pCopy);

	float length();							// returns the lenght of our vector
	vec3 normalized();						// returns a unit vector for our vector (|N|)
	vec3& operator=(const vec3& pCopy);		// copy operator
	vec3& operator+=(const vec3& pAdd);		// adds a vector to our vector
	inline vec3 operator+(const vec3 &pAdd) {
		vec3 copy = *this;
		copy += pAdd;
		return copy;
	};
	vec3& operator-=(const vec3& pSub);		// substracts a vector from our vector
	inline vec3 operator-(const vec3 &pSub) {
		vec3 copy = *this;
		copy -= pSub;
		return copy;
	};
	vec3& operator*=(float pMult);			// multiply our vector with a scalar
	inline vec3 operator*(float pMult) {
		vec3 copy = *this;
		copy /= pMult;
		return copy;
	};
	vec3& operator/=(float pDiv);			// divide our vector with a scalar
	inline vec3 operator/(float pDiv) {
		vec3 copy = *this;
		copy /= pDiv;
		return copy;
	};

	float operator%(const vec3 &pWith);		// return dot product with a second vector
	vec3 operator*(const vec3 &pCross);		// cross product of two vectors
	
};

#endif
