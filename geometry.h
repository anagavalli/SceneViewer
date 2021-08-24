/**
 * Defines helper methods to create and store the 3 types of supported shapes (teapots, cubes, and spheres).
 */
#ifndef _GLUT_SHAPES_REPLACEMENT
#define _GLUT_SHAPES_REPLACEMENT

// OSX systems require different headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
// Force glm to use radians since usage of degrees is deprecated
#ifndef GLM_FORCE_RADIANS
#define GLM_FORCE_RADIANS
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <fstream>
#include <sstream>
#include <iostream>

// These buffer objects should only be used for shapes defined in this file.
extern GLuint VAOs[3], VBOs[3], NBOs[3], EBOs[3];
enum {CUBE, SPHERE, TEAPOT};

extern GLuint vertexShader, fragmentShader, shaderProgram;
extern GLuint modelviewLoc;

void initBufferObjects();
void destroyBufferObjects();

// Functions used for rendering objects that would normally have been rendered by GLUT

/* TEAPOT */
// The teapot will be fed in as an OBJ file.
extern std::vector <glm::vec3> teapotVertices;
extern std::vector <glm::vec3> teapotNormals;
extern std::vector <unsigned int> teapotIndices;

void initTeapot();
void solidTeapot(float);

/* CUBE */
void initCube();
void solidCube(float);

/* SPHERE */
void initSphere();
void solidSphere(float, int, int);
extern std::vector <glm::vec3> sphereVertices;
extern std::vector <glm::vec3> sphereNormals;
extern int prevStacks, prevSlices;

#endif
