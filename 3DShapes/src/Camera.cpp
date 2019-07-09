#include "Camera.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

Camera::Camera()
{
	glm::vec3 pos = glm::vec3(5);

	m_camMatrix = glm::inverse(glm::lookAt(pos, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)));
	m_projMatrix = glm::perspective(glm::radians(50.0f), 1920.0f / 1080.0f, 0.1f, 5000.0f);
}

void Camera::Draw(unsigned int a_program)
{
	m_projViewMatrix = m_projMatrix * glm::inverse(m_camMatrix);

	unsigned int loc = glGetUniformLocation(a_program, "camProjView");
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m_projViewMatrix[0][0]);
}
