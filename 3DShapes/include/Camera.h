#ifndef __Camera_H_
#define __Camera_H_

#include <glm/ext.hpp>

class Camera
{
public:
	Camera();

	void Draw(unsigned int a_program);

private:

	glm::mat4 m_projMatrix;
	glm::mat4 m_camMatrix;

	glm::mat4 m_projViewMatrix;
};


#endif