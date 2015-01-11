/********************************************************************
 * shader is a container class for a shader program
 * 
 * By Bastiaan Olij - 2015
********************************************************************/

#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "STB/stb_image.h"

/////////////////////////////////////////////////////////////////////
// constructors/destructors
/////////////////////////////////////////////////////////////////////

shader::shader() {
	mShaderProgram = 0;
};

shader::~shader() {
	if (mShaderProgram != 0) {
		glDeleteProgram(mShaderProgram);
		mShaderProgram = 0;		
	};
	freeShaders();
};

void shader::freeShaders() {
	while (!mShaders.empty()) {
		GLuint shader = mShaders.back();
		mShaders.pop_back();
		glDeleteShader(shader);
	};
};

bool shader::getline(std::string &s, std::string &line) {
	// should move this somewhere more sensible..
	
	if (s.length() == 0) {
		line = "";
		return false;
	};
	
	std::size_t pos = s.find_first_of("\r\n");
	if (pos == std::string::npos) {
		line = s;
		s = "";
	} else {
		line = s.substr(0,pos);
		if ((s[pos] == '\r') && (s[pos+1] == '\n')) {
			s = s.substr(pos+2);
		} else {
			s = s.substr(pos+1);			
		};
	};
	
	return true;
};

std::string shader::addLineNos(const char *pText) {
	long lineno = 1;
	std::string input = pText;
	std::string line;
	std::string output;
	
	while (getline(input, line)) {
		// add our line number
		char prefix[50];
		sprintf(prefix, "%li: ",lineno);
		output += prefix;
		output += line;
		output += "\r\n";
		
		lineno++;
	};
	
	return output;
};


/////////////////////////////////////////////////////////////////////
// properties
/////////////////////////////////////////////////////////////////////

/**
 * shaderProgram()
 *
 * Returns our shaderprogram ID that we need to use our shader
 **/
GLuint shader::shaderProgram() {
	return mShaderProgram;
};

/////////////////////////////////////////////////////////////////////
// uniforms
//
// Uniforms are variable definitions inside of the shader that allow
// us to parse information between our application and the shader 
// program.
//
// Information is passed using an ID that represents a uniform, not
// its name. We get get this ID using glGetUniformLocation after
// we've compiled our shader. 
//
// It is important to know that uniforms defined but unused in the 
// shader program are removed during the linking stage. As a result
// no value is returned. Especially if you have many shaders and
// have defined a couple of uniforms that may or may not be used
// in all shaders not having such an ID may not actually be a
// problem. 
//
// Finally, it is smart to cache any uniform IDs instead of calling
// glGetUniformLocation each time (which we'll be doing here for
// simplicities sake)
/////////////////////////////////////////////////////////////////////

GLint shader::uniform(const GLchar * pName) {
	if (mShaderProgram) {
		GLint uniform = glGetUniformLocation(mShaderProgram, pName);
		if (uniform < 0) {
#ifdef __APPLE__
			syslog(LOG_WARNING, "Unknown uniform %s (%i)", pName, uniform);
#else
			// need to implement for other platforms...
#endif
		};
		return uniform;	
	} else {
		return -1;
	};	
};

void shader::setIntUniform(GLint pUniform, GLint pValue) {
	if (pUniform >= 0) {
		glUniform1i(pUniform, pValue);
	};
};

void shader::setFloatUniform(GLint pUniform, GLfloat pValue) {
	if (pUniform >= 0) {
		glUniform1f(pUniform, pValue);
	};	
};

void shader::setVec2Uniform(GLint pUniform, const vec2& pValue) {
	if (pUniform >= 0) {
		glUniform2f(pUniform, pValue.x, pValue.y);
	};		
};

void shader::setVec3Uniform(GLint pUniform, const vec3& pValue) {
	if (pUniform >= 0) {
		glUniform3f(pUniform, pValue.x, pValue.y, pValue.z);
	};			
};

void shader::setVec4Uniform(GLint pUniform, const vec4& pValue) {
	if (pUniform >= 0) {
		glUniform4f(pUniform, pValue.x, pValue.y, pValue.z, pValue.w);
	};	
};

void shader::setMat3Uniform(GLint pUniform, const mat3& pValue) {
	if (pUniform >= 0) {
		glUniformMatrix3fv(pUniform, 1, false, (const GLfloat *) pValue.mat);
	};		
};

void shader::setMat4Uniform(GLint pUniform, const mat4& pValue) {
	if (pUniform >= 0) {
		glUniformMatrix4fv(pUniform, 1, false, (const GLfloat *) pValue.mat);
	};		
};
	
/////////////////////////////////////////////////////////////////////
// interface
/////////////////////////////////////////////////////////////////////

/**
 * addShader(pShaderType, pText)
 *
 * Creates a shader, compiles the shader text and adds it to our shaders
 * pShaderType   - our shader type: GL_VERTEX_SHADER, GL_TESS_CONTROL_SHADER, GL_TESS_EVALUATION_SHADER, GL_GEOMETRY_SHADER or GL_FRAGMENT_SHADER
 * pText         - our shader text
 *
 * Note that you shouldn't add the same shader type more then once or things will get tricky..
 **/
bool shader::addShader(GLenum pShaderType, const GLchar * pText) {
	GLint compiled = 0;

	// create our shader
	GLuint shader = glCreateShader(pShaderType);

	// compile our shader
	const GLchar *stringptrs[1];
	stringptrs[0] = pText;
	glShaderSource(shader, 1, stringptrs, NULL);
	glCompileShader(shader);
	
	// check our status
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (compiled) { 
		mShaders.push_back(shader);
	} else {
		GLint len = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &len); 
		if (len > 1) {
			char type[50];
			
			GLchar* compiler_log = new GLchar[len];
			glGetShaderInfoLog(shader, len, 0, compiler_log);
			
			switch (pShaderType) {
				case GL_VERTEX_SHADER: {
					strcpy(type, "vertex");
				} break;
				case GL_TESS_CONTROL_SHADER: {
					strcpy(type, "tessellation control");
				} break;
				case GL_TESS_EVALUATION_SHADER: {
					strcpy(type, "tessellation evaluation");
				} break;
				case GL_GEOMETRY_SHADER: {
					strcpy(type, "geometry");
				} break;
				case GL_FRAGMENT_SHADER: {
					strcpy(type, "fragment");
				} break;
				default: {
					strcpy(type, "unknown");
				} break;
			};
			
			std::string ShaderWithLineNos = addLineNos(pText);
			
#ifdef __APPLE__
			syslog(LOG_ALERT, "Can't compile %s shader: %s\r\n%s", type, compiler_log, ShaderWithLineNos.c_str());
#else
			// need to implement for other platforms...
#endif
			
			delete compiler_log;
		};
		
		glDeleteShader(shader);
	};

	return compiled;
};

/**
 * link()
 * 
 * Links all the shaders together into a shader program
 **/
bool shader::link() {
	GLint	linked = 0;

	// shouldn't be needed but just in case
	if (mShaderProgram != 0) {
		glDeleteProgram(mShaderProgram);
		mShaderProgram = 0;		
	};
	
	// create our shader program
	mShaderProgram = glCreateProgram();
	
	// attach our shaders to our program
	for (int s = 0; s < mShaders.size(); s++) {
		glAttachShader(mShaderProgram, mShaders[s]);
	};
	
	// link it...
	glLinkProgram(mShaderProgram);

	// and check whether it all went OK..
	glGetProgramiv(mShaderProgram, GL_LINK_STATUS, &linked);		
	if (!linked) {
		GLint len = 0;

		glGetProgramiv(mShaderProgram, GL_INFO_LOG_LENGTH , &len); 
		if (len > 1) {
			GLchar* compiler_log = new GLchar[len];
			glGetProgramInfoLog(mShaderProgram, len, 0, compiler_log);
			
#ifdef __APPLE__
			// for now just log it into the system log
			syslog(LOG_ALERT, "Can't link shader: %s", compiler_log);
#else
			// need to implement for other platforms...
#endif
			
			delete compiler_log;
		};
		
		glDeleteProgram(mShaderProgram);
		mShaderProgram = 0;
	};
	
	// we no longer need our shaders now that our program has been linked
	freeShaders();
	
	return linked;
};

/////////////////////////////////////////////////////////////////////
// helper
/////////////////////////////////////////////////////////////////////

/**
 * loadShader(pFileName)
 *
 * Loads a text file (assumed shader) and returns this as a string.
 * Note that we assume, as is the default with GLFW, that our path 
 * is correctly set to our resources folder
 **/
std::string shader::loadShaderText(const char *pFileName) {
	std::string program;
	std::ifstream file(pFileName);
	
	if (file.is_open()) {
		file.seekg(0, std::ios::end);   
		program.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		program.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	
		file.close();
	};

	return program;
};

/**
 * loadTexture(pFilename)
 *
 * Loads a texture into an OpenGL texture object, note that this texture will be bound to GL_TEXTURE_2D
 **/
GLuint shader::loadTexture(const char *pFileName) {
	GLuint texture = 0;
	
	// create our textures
	glGenTextures(1, &texture);
	
	// bind our texture
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// load our texture
	int width, height, channels;
	unsigned char* image = stbi_load(pFileName, &width, &height, &channels, STBI_rgb);
	
	syslog(LOG_ALERT, "Loaded %s, size = %i, %i, channels = %i ", pFileName, width, height, STBI_rgb_alpha);
	
	if(channels == 3) {
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);	
	} else if(channels == 4) {
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);				
	};
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	stbi_image_free(image);	
	
	return texture;
};
