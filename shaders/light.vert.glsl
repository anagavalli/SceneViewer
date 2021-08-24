#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

uniform mat4 modelview;
uniform mat4 projection;

out vec3 vertNormal;
out vec4 vertPos;

void main() {
    gl_Position = projection * modelview * vec4(position, 1.0f); 
	vertNormal = normal;
	vertPos = vec4(position, 1.0f);
}

