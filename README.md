# Scene Viewer
Simple scene viewer made using C++, OpenGL, GLUT, and GLM.

Usage is "sceneview <scene.txt>".

Note that included executable and build target is OSX 10.15, not guaranteed to work elsewhere.

Included demo scenes are teapot.txt and demo.txt.

### Scene file format:
- Blank line: Any whitespace only lines are ignored.
- \# comment line: Any line with # as its first character is a comment, and will be ignored
- command parameter1 parameter2 ...: The first part of the line is always the command (may be preceded by whitespace, for formatting). Depending on the command, it takes in some number of parameters. If the number of parameters is not appropriate for that command (or the command is not recognized) an error message is printed and the line is skipped.

### Scene file commands:
Scene descriptors
- **size width height** specifies the width and height of the scene.
- **camera lookfromx lookfromy lookfromz lookatx lookaty lookatz upx upy upz fovy** specifies the camera in the standard way. zmin and zmax are currently hardcoded.

Lighting
- **light x y z w r g b a** has 8 parameters, the first 4 of which specify the homogeneous coordinates of the light. If w = 0, it is a directional light and is a point light in homogeneous coordinates otherwise. The colors are specified next. There is a maximum of 10 lights in the scene, and if no lights are specified lighting is disabled.

Material properties - Note that material properties persist for all objects until overwritten (see demo.txt).
- **ambient r g b a** specifies the ambient color. 
- **diffuse r g b a** specifies the diffuse color of the surface
- **specular r g b a** specifies the specular color of the surface
- **emission r g b a** gives the emissive color of the surface
- **shininess s** specifies the shininess of the surface

Geometries - only the following geometry primitives are supported.
- **teapot size** makes a teapot of given size
- **sphere size** makes a sphere of given size
- **cube size** makes a cube of given size

Transforms - arbitrary transformations are supported per object, with the last defined being applied first (matrix stack)
- **translate x y z** translation 3-vector
- **rotate x y z deg** rotation of deg degrees about the axis x y z
- **scale x y z** non-uniform scaling

Matrix stack - see demo.txt for examples.
- **pushTransform** pushes the current transformation onto the stack.
- **popTransform** pops the transform from the stack.