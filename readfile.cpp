/**
 * Implementations of functions defined in readfile.h
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
#include "transform.h"

using namespace std;
#include "variables.h"
#include "readfile.h"

void rightmultiply(const mat4 & M, stack<mat4> &transfstack)
{
  mat4 &T = transfstack.top();
  T = T * M;
}

bool readvals(stringstream &s, const int numvals, GLfloat* values)
{
  for (int i = 0; i < numvals; i++) {
    s >> values[i];
    if (s.fail()) {
      cout << "Failed reading value " << i << " will skip\n";
      return false;
    }
  }
  return true;
}

void readfile(const char* filename)
{
  string str, cmd;
  ifstream in;
  in.open(filename);
  if (in.is_open()) {

    // I need to implement a matrix stack to store transforms.
    stack <mat4> transfstack;
    transfstack.push(mat4(1.0));  // identity

    getline (in, str);
    while (in) {
      // Skip comments and blank lines
      if ((str.find_first_not_of(" \t\r\n") != string::npos) && (str[0] != '#')) {

        stringstream s(str);
        s >> cmd;
        int i;
        GLfloat values[10]; // Position and color for light, colors for others
                            // Up to 10 params for cameras.
        bool validinput; // Validity of input

        // Process the light, add it to database.
        // Lighting Command
        if (cmd == "light") {
          if (numUsed == numLights) { // No more Lights
            cerr << "Reached Maximum Number of Lights " << numUsed << " Will ignore further lights\n";
          } else {
            validinput = readvals(s, 8, values); // Position/color for lts.
            if (validinput) {
              GLfloat* curLightPos = lightPosn + (numUsed * 4);
              GLfloat* curLightColor = lightColor + (numUsed * 4);

              memcpy(curLightPos, values, 4*sizeof(GLfloat));
              memcpy(curLightColor, values + 4, 4*sizeof(GLfloat));

              ++numUsed;
            }
          }
        }

          // Material Commands
          // Ambient, diffuse, specular, shininess properties for each object.
        else if (cmd == "ambient") {
          validinput = readvals(s, 4, values); // colors
          if (validinput) {
            memcpy(ambient, values, 4*sizeof(GLfloat));
          }
        } else if (cmd == "diffuse") {
          validinput = readvals(s, 4, values);
          if (validinput) {
            memcpy(diffuse, values, 4*sizeof(GLfloat));
          }
        } else if (cmd == "specular") {
          validinput = readvals(s, 4, values);
          if (validinput) {
            memcpy(specular, values, 4*sizeof(GLfloat));
          }
        } else if (cmd == "emission") {
          validinput = readvals(s, 4, values);
          if (validinput) {
            memcpy(emission, values, 4*sizeof(GLfloat));
          }
        } else if (cmd == "shininess") {
          validinput = readvals(s, 1, values);
          if (validinput) {
            shininess = values[0];
          }
        } else if (cmd == "size") {
          validinput = readvals(s,2,values);
          if (validinput) {
            w = (int) values[0]; h = (int) values[1];
          }
        } else if (cmd == "camera") {
          validinput = readvals(s,10,values); // 10 values eye center up fov
          if (validinput) {
            eyeinit = vec3(values[0], values[1], values[2]);
            center = vec3(values[3], values[4], values[5]);
            vec3 cameraUp = vec3(values[6], values[7], values[8]);
            upinit = transform::upvector(cameraUp, center - eyeinit);
            fovy = values[9];
          }
        }
        else if (cmd == "sphere" || cmd == "cube" || cmd == "teapot") {
          if (numObjects == maxObjects) { // No more objects
            cerr << "Reached Maximum Number of Objects " << numObjects << " Will ignore further objects\n";
          } else {
            validinput = readvals(s, 1, values);
            if (validinput) {
              object * obj = &(objects[numObjects]);
              obj->size = values[0];

              // Set the object's light properties
              memcpy(obj->ambient, ambient, 4*sizeof(GLfloat));
              memcpy(obj->diffuse, diffuse, 4*sizeof(GLfloat));
              memcpy(obj->specular, specular, 4*sizeof(GLfloat));
              memcpy(obj->emission, emission, 4*sizeof(GLfloat));
              obj->shininess = shininess;

              // Set the object's transform
              obj->transform = transfstack.top();

              // Set the object's type
              if (cmd == "sphere") {
                obj->type = sphere;
              } else if (cmd == "cube") {
                obj->type = cube;
              } else if (cmd == "teapot") {
                obj->type = teapot;
              }
            }
            ++numObjects;
          }
        }

        else if (cmd == "translate") {
          validinput = readvals(s,3,values);
          if (validinput) {
            rightmultiply(transform::translate(values[0], values[1], values[2]), transfstack);
          }
        }
        else if (cmd == "scale") {
          validinput = readvals(s,3,values);
          if (validinput) {
            rightmultiply(transform::scale(values[0], values[1], values[2]), transfstack);
          }
        }
        else if (cmd == "rotate") {
          validinput = readvals(s,4,values);
          if (validinput) {
            vec3 rotAxis = glm::normalize(vec3(values[0], values[1], values[2]));
            mat3 rotMatrix = transform::rotate(values[3], rotAxis);
            rightmultiply(mat4(rotMatrix), transfstack);
          }
        }
        else if (cmd == "pushTransform") {
          transfstack.push(transfstack.top());
        } else if (cmd == "popTransform") {
          if (transfstack.size() <= 1) {
            cerr << "Stack has no elements.  Cannot Pop\n";
          } else {
            transfstack.pop();
          }
        }
        else {
          cerr << "Unknown Command: " << cmd << " Skipping \n";
        }
      }
      getline (in, str);
    }

    // Set up initial position for eye, up and amount
    // As well as booleans

    eye = eyeinit;
    up = upinit;
    amount = amountinit;
    sx = sy = 1.0;  // keyboard controlled scales in x and y
    tx = ty = 0.0;  // keyboard controlled translation in x and y

    glEnable(GL_DEPTH_TEST);
  } else {
    cerr << "Unable to Open Input Data File " << filename << "\n";
    throw 2;
  }
}
