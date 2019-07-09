#ifndef _SHAPE_H__
#define _SHAPE_H__

class Shape
{
public:

	~Shape();
	virtual void Draw() = 0;

protected:

	unsigned int m_vbo, m_vao, m_ibo;
};

#endif // !_SHAPE_H__
