/**
 * Implementations of functions defined in shaders.h.
 */
#include <iostream>
#include <fstream>
#include <string.h>
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

using namespace std ; 

// This is a basic program to initiate a shader
// The textFileRead function reads in a filename into a string
// programErrors and shaderErrors output compilation errors
// initShaders initiates a vertex or fragment shader
// initProgram initiates a program with vertex and fragment shaders

string textFileRead (const char * filename) {
  string str, ret = "" ; 
  ifstream in ; 
  in.open(filename) ; 
  if (in.is_open()) {
    getline (in, str) ; 
    while (in) {
      ret += str + "\n" ; 
      getline (in, str) ; 
    }
    return ret ; 
  }
  else {
    cerr << "Unable to Open File " << filename << "\n" ; 
    throw 2 ; 
  }
}

void programErrors (const GLint program) {
  GLint length ; 
  GLchar * log ; 
  glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length) ; 
  log = new GLchar[length+1] ;
  glGetProgramInfoLog(program, length, &length, log) ; 
  cout << "Compile Error, Log Below\n" << log << "\n" ; 
  delete [] log ; 
}
void shaderErrors (const GLint shader) {
  GLint length ; 
  GLchar * log ; 
  glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length) ; 
  log = new GLchar[length+1] ;
  glGetShaderInfoLog(shader, length, &length, log) ; 
  cout << "Compile Error, Log Below\n" << log << "\n" ; 
  delete [] log ; 
}

GLuint initShaders (GLenum type, const char *filename)
{
  GLuint shader = glCreateShader(type) ; 
  GLint compiled ; 
  string str = textFileRead (filename) ; 
  const char * cstr = str.c_str();
  glShaderSource (shader, 1, &cstr, NULL) ; 
  glCompileShader (shader) ; 
  glGetShaderiv (shader, GL_COMPILE_STATUS, &compiled) ; 
  if (!compiled) {
    shaderErrors(shader) ;
    throw 3 ; 
  }
  return shader ; 
}

GLuint initProgram (GLuint vertexShader, GLuint fragmentShader)
{
  GLuint program = glCreateProgram() ; 
  GLint linked ; 
  glAttachShader(program, vertexShader) ;
  glAttachShader(program, fragmentShader) ;
  glLinkProgram(program) ; 
  glGetProgramiv(program, GL_LINK_STATUS, &linked) ; 
  if (linked) glUseProgram(program) ; 
  else {
    programErrors(program) ;
    throw 4 ; 
  }
  return program ; 
}
