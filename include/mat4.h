/********************************************************************
 * 4x4 matrix class
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#ifndef mat4h
#define mat4h

#define		PI				3.14159265358979323846264

#include <math.h>
#include "vec3.h"
#include "vec4.h"
#include "mat3.h"

class mat4 {
public:
	float	mat[4][4];
	
	mat4();
	mat4(const mat3& pCopy);
	mat4(const mat4& pCopy);
	
	// conversions
	mat3 mat3x3();
	
	// interface
	void identity();
	void perspective(float fov, float aspect, float znear, float zfar);
	void frustum(float pLeft, float pRight, float pBottom, float pTop, float pZNear, float pZFar);
	void rotate(float pAngle, float pX, float pY, float pZ);
	inline void rotate(float pAngle, const vec3& pAround) {
		rotate(pAngle, pAround.x, pAround.y, pAround.z);
	};
	
	// operators
	mat4& operator=(const mat4& pCopy);
	mat4& operator*=(const mat4& pMult);
	inline mat4 operator*(const mat4& pMult) {
		mat4 copy = *this;
		copy *= pMult;
		return copy;		
	};
	mat4& operator+=(const vec3& pTranslate);
	vec3 operator*(const vec3& pVec) const;
	vec4 operator*(const vec4& pVec) const;
};

#endif