#ifndef headers_H
#define headers_H
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <vector>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <GL/glew.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glu.h>
#include <SDL/SDL.h>
#include <GLFW/glfw3.h>

#include <random>
#include <stdexcept>

struct part {
	double x;
	double y;
	double z;
	double mass;
};

// For OpenGL rendering
struct col {
	float r;
	float g;
	float b;
};

void initGL();
void Initiate_Velocity();
void initGLFW();
void initPointSpriteExt();
static void error_callback(int error, const char* description);
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
extern "C" void Pre_Render();
extern "C" void Render(float* coord, int num);
extern "C" void Post_Render();

extern GLuint m_texStar;
extern GLFWwindow* window;
#endif
