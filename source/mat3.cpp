/********************************************************************
 * 3x3 matrix class
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#include "mat3.h"

mat3::mat3() {
	identity();
};

mat3::mat3(const mat3& pCopy) {
	for (int j = 0 ; j < 3; j++) {
		for (int i = 0 ; i < 3; i++) {
			mat[j][i] = pCopy.mat[j][i];
		};		
	};
};

void mat3::identity() {
	for (int j = 0 ; j < 3; j++) {
		for (int i = 0 ; i < 3; i++) {
			mat[j][i] = i == j ? 1 : 0;
		};		
	};
};


void mat3::rotate(float pAngle, float pX, float pY, float pZ) {
	mat3 R;
	vec3 axis(pX, pY, pZ);
	axis = axis.normalized();

	float sin = sinf(pAngle * PI / 180.0f);
	float cos = cosf(pAngle * PI / 180.0f);
	
	float xx = axis.x * axis.x;
	float yy = axis.y * axis.y;
	float zz = axis.z * axis.z;
	float xy = axis.x * axis.y;
	float yz = axis.y * axis.z;
	float zx = axis.z * axis.x;
	float xs = axis.x * sin;
	float ys = axis.y * sin;
	float zs = axis.z * sin;
	float oneMinCos = 1.0f - cos;
	
	R.mat[0][0] = (oneMinCos * xx) + cos;
	R.mat[0][1] = (oneMinCos * xy) - zs;
	R.mat[0][2] = (oneMinCos * zx) + ys;
 
	R.mat[1][0] = (oneMinCos * xy) + zs;
	R.mat[1][1] = (oneMinCos * yy) + cos;
	R.mat[1][2] = (oneMinCos * yz) - xs;
 
	R.mat[2][0] = (oneMinCos * zx) - ys;
	R.mat[2][1] = (oneMinCos * yz) + xs;
	R.mat[2][2] = (oneMinCos * zz) + cos;
 		  
	*this *= R;
};

mat3& mat3::operator=(const mat3& pCopy) {
	for (int j = 0 ; j < 3; j++) {
		for (int i = 0 ; i < 3; i++) {
			mat[j][i] = pCopy.mat[j][i];
		};		
	};

	return (*this);
};

mat3& mat3::operator*=(const mat3& pMult) {
	int		i, j, k;
	mat3	Copy;

	Copy=*this;

	for (j = 0; j < 3; j++) {
		for (i = 0; i < 3; i++) {
			mat[j][i] = 0.0;

			for (k = 0; k < 3; k++) {
				mat[j][i] += pMult.mat[j][k] * Copy.mat[k][i];				
			};
		};
	};
  
	return (*this);
};
