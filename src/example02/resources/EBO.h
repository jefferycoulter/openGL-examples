#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include <glad/glad.h>

class EBO
{
public:
	// reference of element buffer object
	GLuint m_EBO;

	// constructor that generates an element buffer objectand links it to indices
	EBO(GLuint* indices, GLsizeiptr size);

	// bind the EBO
	void Bind();
	// unbind the EBO
	void Unbind();
	// delete the EBO
	void Delete();
};

#endif