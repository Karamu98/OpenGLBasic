#include "Shape.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>


Shape::~Shape()
{
	unsigned int buffers[3] = { m_vbo, m_vao, m_ibo }; 
	glDeleteBuffers(3, buffers);
}
