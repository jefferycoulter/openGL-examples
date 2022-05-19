#include "VBO.h"

VBO::VBO(std::vector<Vertex>& vertices)
{
    // create the buffer with a single object
	glGenBuffers(1, &m_VBO);
    // bind the vertex buffer to an openGL array buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    // store the vertex data in the buffer
	glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

void VBO::Bind()
{
    // bind the vertex buffer to an openGL array buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
}


void VBO::Unbind()
{
    // bind both the VBO (and VAO) to 0 so that the VAO and VBO that have been created aren't
    // accidentally changed by some function later
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete()
{
	glDeleteBuffers(1, &m_VBO);
}