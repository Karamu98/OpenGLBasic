#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec2 aUV;

uniform mat4 camProjView;
uniform mat4 objMatrix;

out Vertex
{
    vec4 pos;
}vert;


void main()
{
    vert.pos = camProjView * objMatrix * vec4(aPos, 1.0);
    gl_Position = vert.pos;
}