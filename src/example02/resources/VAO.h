#ifndef VAO_INCL
#define VAO_INCL

#include <glad/glad.h>
#include "VBO.h"

class VAO
{
public:

	GLuint m_VAO;

	VAO();

    // links a VBO to the VAO
	void LinkVBO(VBO& VBO, GLuint layout);
    // bind the VAO
	void Bind();
    // unbind the VAO
	void Unbind();
    // delete the VAO
	void Delete();
};
#endif // VAO_INCL