#version 330

uniform vec3 gLightColour;
out vec4 FragColour;

in Vertex
{
    vec4 pos;
}vert;



void main()
{
    FragColour = vec4(gLightColour, 1.0);
}