/**
 * Simple scene viewer made using C++, OpenGL, GLUT, and GLM.
 *
 * Usage is "./sceneview <file.txt>". See demo.txt and teapot.txt for examples.
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "shaders.h"
#include "transform.h"
#include <FreeImage.h>
#include "geometry.h"

using namespace std; 

// Main variables in the program.  
#define MAINPROGRAM 
#include "variables.h" 
#include "readfile.h" // prototypes for readfile.cpp  
void display(void);  // prototype for display function.  

// Reshapes the window
void reshape(int width, int height){
  w = width;
  h = height;

  glViewport(0, 0, w, h);

  // Create projection matrix
  float aspect = (float) w / (float) h, zNear = 0.1, zFar = 99.0 ;
  projection = transform::perspective(fovy,aspect,zNear,zFar) ;
  glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, &projection[0][0]);
}

void printHelp() {
  std::cout << "\npress 'h' to print this message again.\n" 
    << "press '+' or '-' to change the amount of rotation that\noccurs with each arrow press.\n" 
    << "press 'r' to reset the transformations.\n"
    << "press 'v' 't' 's' to do view [default], translate, scale.\n"
    << "press ESC to quit.\n" ;      
}


void keyboard(unsigned char key, int x, int y) {
  switch(key) {
    case '+':
      amount++;
      std::cout << "amount set to " << amount << "\n" ;
      break;
    case '-':
      amount--;
      std::cout << "amount set to " << amount << "\n" ; 
      break;
    case 'h':
      printHelp();
      break;
    case 27:  // Escape to quit
      exit(0);
    case 'r': // reset eye and up vectors, scale and translate. 
      eye = eyeinit;
      up = upinit;
      amount = amountinit;
      transformOp = view;
      sx = sy = 1.0;
      tx = ty = 0.0;
      break ;   
    case 'v': 
      transformOp = view ;
      std::cout << "Operation is set to View\n" ; 
      break ; 
    case 't':
      transformOp = translate ;
      std::cout << "Operation is set to Translate\n" ; 
      break ; 
    case 's':
      transformOp = scale ;
      std::cout << "Operation is set to Scale\n" ; 
      break ; 
  }
  glutPostRedisplay();
}

void arrowKey(int key, int x, int y) {
  switch(key) {
    case 100: //left
      if (transformOp == view) transform::left(amount, eye,  up);
      else if (transformOp == scale) sx -= amount * 0.01 ;
      else if (transformOp == translate) tx -= amount * 0.01 ;
      break;
    case 101: //up
      if (transformOp == view) transform::up(amount,  eye,  up);
      else if (transformOp == scale) sy += amount * 0.01 ;
      else if (transformOp == translate) ty += amount * 0.01 ;
      break;
    case 102: //right
      if (transformOp == view) transform::left(-amount, eye,  up);
      else if (transformOp == scale) sx += amount * 0.01 ;
      else if (transformOp == translate) tx += amount * 0.01 ;
      break;
    case 103: //down
      if (transformOp == view) transform::up(-amount,  eye,  up);
      else if (transformOp == scale) sy -= amount * 0.01 ;
      else if (transformOp == translate) ty -= amount * 0.01 ;
      break;
  }
  glutPostRedisplay();
}

void init() {
  // Initialize shaders
  vertexShader = initShaders(GL_VERTEX_SHADER, "shaders/light.vert.glsl") ;
  fragmentShader = initShaders(GL_FRAGMENT_SHADER, "shaders/light.frag.glsl") ;
  shaderProgram = initProgram(vertexShader, fragmentShader) ;
  // Get locations of all uniform variables.
  enableLightingLoc = glGetUniformLocation(shaderProgram,"enableLighting") ;
  lightPosnLoc = glGetUniformLocation(shaderProgram,"lightPosn") ;
  lightColorPos = glGetUniformLocation(shaderProgram,"lightColor") ;
  numUsedLoc = glGetUniformLocation(shaderProgram,"numUsed") ;
  ambientLoc = glGetUniformLocation(shaderProgram,"ambient") ;
  diffuseLoc = glGetUniformLocation(shaderProgram,"diffuse") ;
  specularLoc = glGetUniformLocation(shaderProgram,"specular") ;
  emissionLoc = glGetUniformLocation(shaderProgram,"emission") ;
  shininessLoc = glGetUniformLocation(shaderProgram,"shininess") ;
  projectionLoc = glGetUniformLocation(shaderProgram, "projection");
  modelviewLoc = glGetUniformLocation(shaderProgram, "modelview");
  // Initialize geometric shapes
  initBufferObjects();
  initTeapot(); initCube(); initSphere();
}

int main(int argc, char* argv[]) {

  if (argc != 2) {
    cerr << "Usage: sceneview <scenefile>\n";
    exit(-1); 
  }

  FreeImage_Initialise();
  glutInit(&argc, argv);
// OSX systems require an extra window init flag
#ifdef __APPLE__
  glutInitDisplayMode(GLUT_3_2_CORE_PROFILE | GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#else
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
#endif
  //glutInitWindowSize(w,h);
  glutCreateWindow("Scene Viewer");

#ifndef __APPLE__ // GLew not needed on OSX systems
  GLenum err = glewInit() ; 
  if (GLEW_OK != err) { 
    std::cerr << "Error: " << glewGetString(err) << std::endl; 
  } 
#endif

  init();
  readfile(argv[1]) ; 
  glutDisplayFunc(display);
  glutSpecialFunc(arrowKey);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutReshapeWindow(w, h);

  printHelp();
  glutMainLoop();
  FreeImage_DeInitialise();
  destroyBufferObjects();
  return 0;
}
