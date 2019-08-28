#ifndef __Camera_H_
#define __Camera_H_

#include <glm/ext.hpp>

class Camera
{
public:
	Camera();

	void Draw(unsigned int a_program);
	void Update(float a_deltaTime);
	inline glm::vec3 GetPos() { return glm::vec3(m_camMatrix[3]); }

private:

	glm::mat4 m_projMatrix;
	glm::mat4 m_camMatrix;
	glm::mat4 m_projViewMatrix;
	
	float m_camSpeed = 2;

};


#endif