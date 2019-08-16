#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 camProjView;
uniform mat4 objMatrix;

out Vertex
{
    vec4 pos;
    vec3 normal;
    vec2 uv;
}vert;


void main()
{
    vert.pos = camProjView * objMatrix * vec4(aPos, 1.0);
    vert.uv = aUV;
    gl_Position = vert.pos;
}