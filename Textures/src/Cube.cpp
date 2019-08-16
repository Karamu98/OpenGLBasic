#include "Cube.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const float Cube::vertexData[] = 
{
	// Reference dir: Facing the cube from the front 

	// Positions			Uv's
	-1.0f, -1.0f,  1.0f,	0.0f, 0.0f, // Front bottom left	0
	 1.0f, -1.0f,  1.0f,	1.0f, 0.0f, // Front bottom right	1
	-1.0f,  1.0f,  1.0f,	0.0f, 1.0f, // Front top left		2
	 1.0f,  1.0f,  1.0f,	1.0f, 1.0f, // Front top right		3
	-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, // Back bottom left		4
	 1.0f, -1.0f, -1.0f,	0.0f, 1.0f, // Back bottom right	5
	-1.0f,  1.0f, -1.0f,	1.0f, 0.0f, // Back top left		6
	 1.0f,  1.0f, -1.0f,	0.0f, 0.0f	// back top right		7
};

const unsigned int Cube::indiciesData[] =
{
	// Front
	0, 1, 2,
	3, 2, 1,
	// Back
	5, 4, 7,
	6, 7, 4,
	// Left
	4, 0, 6,
	2, 6, 0,
	// Right
	1, 5, 3,
	7, 3, 5,
	// Top
	2, 3, 6,
	7, 6, 3,
	// Bottom
	4, 5, 0,
	1, 0, 5

	//0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
};

Cube::Cube()
{
	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ibo);

	glBindVertexArray(m_vao);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), nullptr);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indiciesData), indiciesData, GL_STATIC_DRAW);
}

void Cube::DrawShape()
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLE_STRIP, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}