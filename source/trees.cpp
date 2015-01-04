/********************************************************************
 * Our main program
 * 
 * By Bastiaan Olij - 2014
********************************************************************/

#include "trees.h"

bool	wireframe = false;
bool	paused = true;
float	rotate = 0.0f;
float	distance = 400.0f;

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
			case GLFW_KEY_W: {
				wireframe = !wireframe;
			} break;
			case GLFW_KEY_ESCAPE: {
		        glfwSetWindowShouldClose(window, GL_TRUE);				
			} break;
			default: break;
		};
	};
};

static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
	distance += yoffset;
	if (distance < 10.0) {
		distance = 10.0;
	} else if (distance > 10000.0) {
		distance = 10000.0;
	};
	
	rotate += xoffset;
};

int main(void) {
	unsigned int stage = grow_tree_stage;
	
	glfwSetErrorCallback(error_callback);
	if (!glfwInit()) {
	    exit(EXIT_FAILURE);		
	};

#ifdef __APPLE__
			syslog(LOG_NOTICE, "Starting trees");
#else
			// need to implement for other platforms...
#endif
		
	// make sure we're using OpenGL 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		
	GLFWwindow* window = glfwCreateWindow(1024, 768, "Test tree logic", NULL, NULL);
	if (window) {
		glfwMakeContextCurrent(window);
		
		// init GLEW
		glewExperimental=true;
		GLenum err = glewInit();
		if (err != GLEW_OK) {
#ifdef __APPLE__
			syslog(LOG_ALERT, "Error: %s", glewGetErrorString(err));
#else
			// need to implement for other platforms...
#endif
		} else {
#ifdef __APPLE__
			syslog(LOG_NOTICE, "Using GLEW %s", glewGetString(GLEW_VERSION));
#else
			// need to implement for other platforms...
#endif
		};
		
		glfwSetKeyCallback(window, key_callback);
		glfwSetScrollCallback(window, scroll_callback);

		// our new tree
		treelogic * tree = new treelogic();
	
		// add just one branch to start of with, you could build the start of a tree here manually
		tree->growBranch(0, vec3(0.0, 30.0, 0.0));
	
		// and generate our attraction points, as a sample I've staged the points to get larger concentrations of points nearer to the center
		tree->generateAttractionPoints(800, 75.0, 50.0, 1.5, 50.0);
		tree->generateAttractionPoints(300, 90.0, 75.0, 1.5, 60.0, false);
		tree->generateAttractionPoints(50, 100.0, 90.0, 1.5, 70.0, false);

		// and an example with very few attraction points:
//		tree->generateAttractionPoints(50, 100.0, 40.0, 2.0, 50.0, false);
		
		tree->initShaders();
		
		while (!glfwWindowShouldClose(window)) {
	        int width, height;

			// get our width and height, may have been resized
	        glfwGetFramebufferSize(window, &width, &height);
			
			// clear our viewport
	        glViewport(0, 0, width, height);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

			// setup our projection matrix
			mat4 projection;
	        float ratio = width / (float) height;
			projection.perspective(45, ratio, 1.0, 10000.0);
			tree->setProjection(projection);

			// note, with just an identity matrix our "camera" is at 0.0 ,0.0 ,0.0 looking straight ahead (looking towards 0.0 ,0.0 , -1.0)..
			// but we do adjust our view
			mat4 view;			
			view += vec3(0.0f, -150.0f, -distance);
			view.rotate(rotate, 0.f, 1.f, 0.f);
			tree->setView(view);
			
			// we leave our model alone for now...
			
			// and render
			tree->setWireframe(wireframe);
			tree->render();
			
			if ((stage == grow_tree_stage) && !paused) {
				// This is just for testing, if we were generating for real we would call this until it returns false in our initialisation... 
				if (!tree->doIteration(100.0, 1.0, 10.0, vec3(0.1, 0.2, 0.0))) {
					stage = grow_roots_stage;
					
					// we also add a small point cloud for our roots to grow next
//					tree->generateAttractionPoints(150, 50.0, 20.0, 0.2, -3.0, false);					
				};
			} else if ((stage == grow_roots_stage) && !paused) {
				if (!tree->doIteration(30.0, 2.0, 10.0, vec3(0.0, 0.0, 0.0))) {
					stage = optimise_tree_stage;
					paused = true;
				};
			} else if ((stage == optimise_tree_stage) && !paused) {
				tree->optimiseNodes();
				stage = build_mesh_stage;
				paused = true;
			} else if ((stage == build_mesh_stage) && !paused) {
				tree->createModel();
				stage = add_leaves_stage;
				paused = true;
			};
						
			glfwSwapBuffers(window);
			glfwPollEvents();
		};
	
		glfwDestroyWindow(window);	

		delete tree;
	};	
	
	glfwTerminate();
};
