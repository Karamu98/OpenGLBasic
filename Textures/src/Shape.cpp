#include "Shape.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>


Shape::Shape() : m_ibo(-1), m_vbo(-1), m_vao(-1)
{
	m_objMatrix = glm::inverse(glm::lookAt(glm::vec3(0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0)));
}

Shape::~Shape()
{
	unsigned int buffers[3] = { m_vbo, m_vao, m_ibo }; 
	glDeleteBuffers(3, buffers);
}

void Shape::Draw(unsigned int a_program)
{
	unsigned int loc = glGetUniformLocation(a_program, "objMatrix");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m_objMatrix[0][0]);

	DrawShape();
}

void Shape::SetPosition(glm::vec3 a_newPos)
{
	m_objMatrix[3] = glm::vec4(a_newPos, 1);
}

void Shape::Rotate(float a_angle, glm::vec3 a_axis)
{
	m_objMatrix = glm::rotate(m_objMatrix, a_angle, a_axis);
}
