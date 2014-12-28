/********************************************************************
 * Our main program
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#include "trees.h"

bool	paused = true;

void error_callback(int error, const char* description) {
#ifdef __APPLE__
	// output to syslog
	syslog(LOG_ALERT, "%i: %s", error, description);
#endif
};

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) {
		switch (key) {
			case GLFW_KEY_SPACE: {
				paused = !paused;
			} break;
			case GLFW_KEY_ESCAPE: {
		        glfwSetWindowShouldClose(window, GL_TRUE);				
			} break;
			default: break;
		};
	};
};

void perspective(float fov, float aspect, float znear, float zfar) {
	float ymax, xmax;
	
	ymax = znear * tan(fov * PI / 360.0f);
	xmax = ymax * aspect;
	glFrustum(-xmax, xmax, -ymax, ymax, znear, zfar);
};

int main(void) {
	unsigned int stage = grow_tree_stage;
	
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
	    exit(EXIT_FAILURE);		
	};
	
	// our new tree
	treelogic * tree = new treelogic();
	
	// add just one branch to start of with, you could build the start of a tree here manually
	tree->growBranch(0, vec3(0.0, 30.0, 0.0));
	
	// and generate our attraction points, as a sample I've staged the points to get larger concentrations of points nearer to the center
	tree->generateAttractionPoints(800, 75.0, 50.0, 2.0, 30.0);
	tree->generateAttractionPoints(300, 90.0, 75.0, 2.0, 30.0, false);
	tree->generateAttractionPoints(50, 100.0, 90.0, 2.0, 30.0, false);
		
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Test tree logic", NULL, NULL);
	if (window) {
		glfwMakeContextCurrent(window);
		glfwSetKeyCallback(window, key_callback);
		
		while (!glfwWindowShouldClose(window)) {
	        float ratio;
	        int width, height;
			
	        glfwGetFramebufferSize(window, &width, &height);
	        ratio = width / (float) height;
			
			// for the time being we stick to good old, SLOOOW OpenGL 1.0 commands.. 
	        glViewport(0, 0, width, height);
	        glClear(GL_COLOR_BUFFER_BIT);
	        glMatrixMode(GL_PROJECTION);
	        glLoadIdentity();
			perspective(45, width/height, 1.0, 1000.0);
	        glMatrixMode(GL_MODELVIEW);
	        glLoadIdentity();
			
			// note, with just an identity matrix our "camera" is at 0.0 ,0.0 ,0.0 looking straight ahead (looking towards 0.0 ,0.0 , -1.0)..
			
			// as whatever we render will be centered at 0.0, 0.0, 0.0 move it to a better location..
			glTranslatef(0.0f, -150.0f, -400.0f);
			
			// and rotate it so we can see what is going on...
	        glRotatef((float) glfwGetTime() * 50.f, 0.f, 1.f, 0.f);
			
			tree->render();
			
			if ((stage == grow_tree_stage) && !paused) {
				// This is just for testing, if we were generating for real we would call this until it returns false in our initialisation... 
				if (!tree->doIteration(100.0, 2.0, 10.0, vec3(0.1, 0.2, 0.0))) {
					stage = grow_roots_stage;
					
					// we also add a small point cloud for our roots to grow next
					tree->generateAttractionPoints(100, 50.0, 10.0, 0.2, -3.0, false);					
				};
			} else if ((stage == grow_roots_stage) && !paused) {
				if (!tree->doIteration(25.0, 2.0, 5.0, vec3(0.0, 0.0, 0.0))) {
					stage = optimise_tree_stage;
					paused = true;
				};
			};
						
			glfwSwapBuffers(window);
			glfwPollEvents();
		};
	
		glfwDestroyWindow(window);	
	};
	
	delete tree;
	
	glfwTerminate();
};
