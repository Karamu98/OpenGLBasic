#version 330
layout (location = 0) in vec3 aPos;

uniform mat4 camProjView;
uniform mat4 objMatrix;

void main()
{
    gl_Position = camProjView * objMatrix * vec4(aPos, 1.0);
}