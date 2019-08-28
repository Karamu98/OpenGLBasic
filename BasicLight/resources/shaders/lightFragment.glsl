#version 330

out vec4 FragColour;

in Vertex
{
    vec4 pos;
}vert;



void main()
{
    FragColour = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}