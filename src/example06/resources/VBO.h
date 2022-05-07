#ifndef VBO_INCL
#define VBO_INCL

#include<glad/glad.h>

class VBO
{
public:
    // reference of vertex buffer object
	GLuint m_VBO;

    // constructor that generates a vertex vuffer object and links it to vertices
	VBO(GLfloat* vertices, GLsizeiptr size);

    // bind the VBO
	void Bind();
    // unbind the VBO
	void Unbind();
    // delete the VBO
	void Delete();
};

#endif // VBO_INCL