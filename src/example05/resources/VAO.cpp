#include "VAO.h"

VAO::VAO()
{
    // create the VAO with a single object (must be done before create the VBO)
	glGenVertexArrays(1, &m_VAO);
}


void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint n_components, GLenum type, GLsizeiptr stride, void *offset)
{
	VBO.Bind();
    // tell openGL how to interpret the vertex data
	glVertexAttribPointer(layout, n_components, type, GL_FALSE, stride, offset);
    // enable the vertex attribute so that openGL knows to use it
	glEnableVertexAttribArray(layout);
    // bind both the VBO and VAO to 0 so that the VAO and VBO that have been created aren't
    // accidentally changed by some function later
	VBO.Unbind();
}

void VAO::Bind()
{
	glBindVertexArray(m_VAO);
}

void VAO::Unbind()
{
    // bind both the VBO and VAO to 0 so that the VAO and VBO that have been created aren't
    // accidentally changed by some function later
	glBindVertexArray(0);
}

void VAO::Delete()
{
	glDeleteVertexArrays(1, &m_VAO);
}