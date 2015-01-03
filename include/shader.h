/********************************************************************
 * shader is a container class for a shader program
 * 
 * By Bastiaan Olij - 2015
********************************************************************/

#ifndef shaderh
#define shaderh

#define		GLFW_INCLUDE_GL_3
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <vector>

#ifdef __APPLE__
#include <syslog.h>
#endif

#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "mat4.h"

class shader {
private:
	std::vector<GLuint>		mShaders;
	GLuint					mShaderProgram;
	
	void freeShaders();
	bool getline(std::string &s, std::string &line);
	std::string addLineNos(const char *pText);
	
public:
	shader();
	~shader();
	
	// properties
	GLuint shaderProgram();
	
	// shaders
	GLint uniform(const GLchar * pName);
	void setIntUniform(GLint pUniform, GLint pValue);
	void setFloatUniform(GLint pUniform, GLfloat pValue);
	void setVec2Uniform(GLint pUniform, const vec2& pValue);
	void setVec3Uniform(GLint pUniform, const vec3& pValue);
	void setVec4Uniform(GLint pUniform, const vec4& pValue);
	void setMat3Uniform(GLint pUniform, const mat3& pValue);
	void setMat4Uniform(GLint pUniform, const mat4& pValue);
	
	// interface
	bool addShader(GLenum pShaderType, const GLchar * pText);
	bool link();
};

#endif