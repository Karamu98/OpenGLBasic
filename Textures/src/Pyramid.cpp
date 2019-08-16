#include "Pyramid.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

const float Pyramid::vertexData[] =
{
	// Positions			// Uv's
	-1.0f, -1.0f,  1.0f,	1.0f, 0.0f, // Far base left
	 1.0f, -1.0f,  1.0f,	0.0f, 0.0f, // Far base right
	-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, // Close base left
	 1.0f, -1.0f, -1.0f,	1.0f, 0.0f, // Close base right
	 0.0f,  1.0f,  0.0f,	0.5f, 1.0f  // Pyramid peak
};

const unsigned int Pyramid::indiciesData[] =
{
	4, 3, 2,    // Front face
	4, 1, 3,    // Right face
	4, 0, 1,    // Back face
	4, 2, 0,    // Left face
	3, 0, 2,    // Bottom right
	0, 3, 1     // Bottom right
};

Pyramid::Pyramid()
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
};

void Pyramid::DrawShape()
{
	glBindVertexArray(m_vao);
	glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}