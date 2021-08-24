/**
 * Main render loop logic.
 */

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <deque>
#include <stack>
// OSX systems require different headers
#ifdef __APPLE__
#include <OpenGL/gl3.h>
#include <OpenGL/glext.h>
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#include "transform.h"
#include "geometry.h"

using namespace std ; 
#include "variables.h"
#include "readfile.h"

// tranforms the given input vector by the current modelview matrix, storing result in output.
void transformvec (const GLfloat input[4], GLfloat output[4]) 
{
  glm::vec4 inputvec(input[0], input[1], input[2], input[3]);
  glm::vec4 outputvec = modelview * inputvec;
  output[0] = outputvec[0];
  output[1] = outputvec[1];
  output[2] = outputvec[2];
  output[3] = outputvec[3];
}

void display() 
{
  glClearColor(0, 0, 1, 0);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // initialize modelview matrix based on camera position
  modelview = transform::lookAt(eye,center,up);
  glUniformMatrix4fv(modelviewLoc, 1, GL_FALSE, &modelview[0][0]);

  // Lights are transformed by current modelview matrix. 
  // The shader can't do this globally. 
  // So we need to do so manually.
  if (numUsed) {
    glUniform1i(enableLightingLoc,true);

    for (int i = 0; i < numUsed; ++i)
      transformvec(&lightPosn[4 * i], &lightTransf[4 * i]);

    glUniform1i(numUsedLoc, numUsed);
    glUniform4fv(lightPosnLoc, numLights, lightTransf);
    glUniform4fv(lightColorPos, numLights, lightColor);
  } else {
    glUniform1i(enableLightingLoc,false);
  }

  // Transformations for objects, involving translation and scaling 
  mat4 sc(1.0) , tr(1.0), transf(1.0); 
  sc = transform::scale(sx,sy,1.0);
  tr = transform::translate(tx,ty,0.0);

  // YOUR CODE FOR HW 2 HERE.  
  // You need to use scale, translate and modelview to 
  // set up the net transformation matrix for the objects.  
  // Account for GLM issues, matrix order, etc.
  transf = modelview * tr * sc;

  
  // The object draw functions will need to further modify the top of the stack,
  // so assign whatever transformation matrix you intend to work with to modelview
  // rather than use a uniform variable for that.
  modelview = transf;

  for (int i = 0 ; i < numObjects ; i++) {
    object* obj = &(objects[i]); // Grabs an object struct.

    // YOUR CODE FOR HW 2 HERE. 
    // Set up the object transformations 
    // And pass in the appropriate material properties
    // Again glUniform() related functions will be useful
    modelview = transf * obj->transform;

    glUniform4fv(ambientLoc, 1, obj->ambient);
    glUniform4fv(diffuseLoc, 1, obj->diffuse);
    glUniform4fv(specularLoc, 1, obj->specular);
    glUniform4fv(emissionLoc, 1, obj->emission);
    glUniform1f(shininessLoc, obj->shininess);


    // Actually draw the object
    // We provide the actual drawing functions for you.  
    // Remember that obj->type is notation for accessing struct fields
    if (obj->type == cube) {
      solidCube(obj->size); 
    }
    else if (obj->type == sphere) {
      const int tessel = 20; 
      solidSphere(obj->size, tessel, tessel); 
    }
    else if (obj->type == teapot) {
      solidTeapot(obj->size); 
    }
  }

  glutSwapBuffers();
}
