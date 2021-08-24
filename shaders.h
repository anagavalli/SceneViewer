/**
 * Utilities to read GLSL shader files and create OpenGL program.
 */
#include <iostream>
#include <string>

#ifndef __INCLUDESHADERS 
#define __INCLUDESHADERS 

std::string textFileRead (const char * filename) ;
void programErrors (const GLint program) ;
void shaderErrors (const GLint shader) ;
GLuint initShaders (GLenum type, const char * filename) ;
GLuint initProgram (GLuint vertexShader, GLuint fragmentShader) ;

#endif 
