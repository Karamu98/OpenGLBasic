#ifndef _SHAPE_H__
#define _SHAPE_H__

#include <glm/glm.hpp>

class Shape
{
public:
	Shape();
	~Shape();

	void Draw(unsigned int a_program);
	void SetPosition(glm::vec3 a_newPos);
	void Rotate(float a_angle, glm::vec3 a_axis);

protected:

	virtual void DrawShape() = 0;

	unsigned int m_vbo, m_vao, m_ibo;
	glm::mat4x4 m_objMatrix;
};

#endif // !_SHAPE_H__
