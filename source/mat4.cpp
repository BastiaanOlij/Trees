/********************************************************************
 * 4x4 matrix class
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#include "mat4.h"

mat4::mat4() {
	identity();
};

mat4::mat4(const mat3& pCopy) {
	identity();
	
	for (int j = 0 ; j < 3; j++) {
		for (int i = 0 ; i < 3; i++) {
			mat[j][i] = pCopy.mat[j][i];
		};		
	};
};

mat4::mat4(const mat4& pCopy) {
	for (int j = 0 ; j < 4; j++) {
		for (int i = 0 ; i < 4; i++) {
			mat[j][i] = pCopy.mat[j][i];
		};		
	};
};

mat3 mat4::mat3x3() {
	mat3 result;
	for (int j = 0 ; j < 3; j++) {
		for (int i = 0 ; i < 3; i++) {
			result.mat[j][i] = mat[j][i];
		};		
	};
	return result;
};

void mat4::identity() {
	for (int j = 0 ; j < 4; j++) {
		for (int i = 0 ; i < 4; i++) {
			mat[j][i] = i == j ? 1 : 0;
		};		
	};
};

void mat4::perspective(float fov, float aspect, float znear, float zfar) {
	float ymax, xmax;
	
	ymax = znear * tan(fov * PI / 360.0f);
	xmax = ymax * aspect;
	
	frustum(-xmax, xmax, -ymax, ymax, znear, zfar);
};

// add frustrum matrix
void mat4::frustum(float pLeft, float pRight, float pBottom, float pTop, float pZNear, float pZFar) {
	mat4 M;

	M.mat[0][0] = (2.0f * pZNear) / (pRight - pLeft);
	M.mat[0][1] = 0.0;
	M.mat[0][2] = 0.0;
	M.mat[0][3] = 0.0f;

	M.mat[1][0] = 0.0;
	M.mat[1][1] = (2 * pZNear) / (pTop - pBottom);
	M.mat[1][2] = 0.0;
	M.mat[1][3] = 0.0f;
	
	M.mat[2][0] = (pRight + pLeft) / (pRight - pLeft);
	M.mat[2][1] = (pTop + pBottom) / (pTop - pBottom);
	M.mat[2][2] = -(pZFar + pZNear) / (pZFar - pZNear);
	M.mat[2][3] = -1.0f;

	M.mat[3][0] = 0.0;
	M.mat[3][1] = 0.0;
	M.mat[3][2] = -(2.0f * pZFar * pZNear) / (pZFar - pZNear);
	M.mat[3][3] = 0.0f;	
	
	*this *= M;	
}; 

void mat4::rotate(float pAngle, float pX, float pY, float pZ) {
	mat4 R;
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
	R.mat[0][3] = 0.0f; 
 
	R.mat[1][0] = (oneMinCos * xy) + zs;
	R.mat[1][1] = (oneMinCos * yy) + cos;
	R.mat[1][2] = (oneMinCos * yz) - xs;
	R.mat[1][3] = 0.0f;
 
	R.mat[2][0] = (oneMinCos * zx) - ys;
	R.mat[2][1] = (oneMinCos * yz) + xs;
	R.mat[2][2] = (oneMinCos * zz) + cos;
	R.mat[2][3] = 0.0f; 
 
	R.mat[3][0] = 0.0f;
	R.mat[3][1] = 0.0f;
	R.mat[3][2] = 0.0f;
	R.mat[3][3] = 1.0f;
		  
	*this *= R;
};

mat4& mat4::operator=(const mat4& pCopy) {
	for (int j = 0 ; j < 4; j++) {
		for (int i = 0 ; i < 4; i++) {
			mat[j][i] = pCopy.mat[j][i];
		};		
	};

	return (*this);
};

mat4& mat4::operator*=(const mat4& pMult) {
	int		i, j, k;
	mat4	Copy;

	Copy=*this;

	for (j = 0; j < 4; j++) {
		for (i = 0; i < 4; i++) {
			mat[j][i] = 0.0;

			for (k = 0; k < 4; k++) {
				mat[j][i] += pMult.mat[j][k] * Copy.mat[k][i];				
			};
		};
	};
  
	return (*this);
};

mat4& mat4::operator+=(const vec3& pTranslate) {
	mat4 translate;
	
	translate.mat[3][0]+=pTranslate.x;
	translate.mat[3][1]+=pTranslate.y;
	translate.mat[3][2]+=pTranslate.z;
	
	*this *= translate;

	return (*this);
};

