/********************************************************************
 * Our main program
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#define		GLFW_INCLUDE_GL_3
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

#define		PI				3.14159265358979323846264

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef __APPLE__
#include <syslog.h>
#endif

#include "treelogic.h"

enum stages {
	grow_tree_stage,
	grow_roots_stage,
	optimise_tree_stage,
	build_mesh_stage,
	add_leaves_stage
};
