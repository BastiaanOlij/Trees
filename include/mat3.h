/********************************************************************
 * 3x3 matrix class
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifndef mat3h
#define mat3h

#define		PI				3.14159265358979323846264

#include <math.h>
#include "vec3.h"

class mat3 {
public:
	float	mat[3][3];
	
	mat3();
	mat3(const mat3& pCopy);
	
	// interface
	void identity();
	void rotate(float pAngle, float pX, float pY, float pZ);
	inline void rotate(float pAngle, const vec3& pAround) {
		rotate(pAngle, pAround.x, pAround.y, pAround.z);
	};
	
	// operators
	mat3& operator=(const mat3& pCopy);
	mat3& operator*=(const mat3& pMult);
	inline mat3 operator*(const mat3& pMult) {
		mat3 copy = *this;
		copy *= pMult;
		return copy;		
	};
	vec3 operator*(const vec3& pVec) const;
	
};

#endif