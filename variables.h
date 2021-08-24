/**
 * This is the basic include file for the global variables in the program.
 *
 * Since all files need access to it, define EXTERN as either blank or
 * extern, depending on if in the main program or not.
 */

#ifdef MAINPROGRAM 
#define EXTERN 
#else 
#define EXTERN extern 
#endif

EXTERN int amount; // The amount of rotation for each arrow press
EXTERN vec3 eye; // The (regularly updated) vector coordinates of the eye 
EXTERN vec3 up;  // The (regularly updated) vector coordinates of the up 

#ifdef MAINPROGRAM 
vec3 eyeinit(0.0,0.0,5.0); // Initial eye position, also for resets
vec3 upinit(0.0,1.0,0.0); // Initial up position, also for resets
vec3 center(0.0,0.0,0.0); // Center look at point
int amountinit = 5;
int w = 500, h = 500; // width and height
float fovy = 90.0; // For field of view
#else 
EXTERN vec3 eyeinit;
EXTERN vec3 upinit;
EXTERN vec3 center;
EXTERN int amountinit;
EXTERN int w, h;
EXTERN float fovy;
#endif 

EXTERN GLuint vertexShader, fragmentShader, shaderProgram; // shaders
EXTERN mat4 projection, modelview; // The mvp matrices
EXTERN GLuint projectionLoc, modelviewLoc; // Uniform locations of the above matrices
static enum {view, translate, scale} transformOp; // which operation to transform
enum shape {cube, sphere, teapot};
EXTERN float sx, sy; // the scale in x and y
EXTERN float tx, ty; // the translation in x and y

// Lighting parameter array, to send to the fragment shader
const int numLights = 10;
EXTERN GLfloat lightPosn[4*numLights];   // Light Positions
EXTERN GLfloat lightColor[4*numLights];  // Light Colors
EXTERN GLfloat lightTransf[4*numLights]; // Lights transformed by modelview
EXTERN int numUsed;                      // How many lights are used

// Materials (read from file)
// One color of each type is active at a time while
// creating objects.
EXTERN GLfloat ambient[4];
EXTERN GLfloat diffuse[4];
EXTERN GLfloat specular[4];
EXTERN GLfloat emission[4];
EXTERN GLfloat shininess;

// For multiple objects, read from a file.  
const int maxObjects = 10;
EXTERN int numObjects;
EXTERN struct object {
  shape type;
  GLfloat size;
  GLfloat ambient[4];
  GLfloat diffuse[4];
  GLfloat specular[4];
  GLfloat emission[4];
  GLfloat shininess;
  mat4 transform;
} objects[maxObjects];

// Uniform variable locations in OpenGL program
EXTERN GLuint lightColorPos;
EXTERN GLuint lightPosnLoc;
EXTERN GLuint numUsedLoc;
EXTERN GLuint enableLightingLoc;
EXTERN GLuint ambientLoc;
EXTERN GLuint diffuseLoc;
EXTERN GLuint specularLoc;
EXTERN GLuint emissionLoc;
EXTERN GLuint shininessLoc;

