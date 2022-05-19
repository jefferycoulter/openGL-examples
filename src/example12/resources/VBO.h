#ifndef VBO_INCL
#define VBO_INCL

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <vector>

struct Vertex
{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 texture;
};

class VBO
{
public:
    // reference of vertex buffer object
	GLuint m_VBO;

    // constructor that generates a vertex vuffer object and links it to vertices
	VBO(std::vector<Vertex>& vertices);

    // bind the VBO
	void Bind();
    // unbind the VBO
	void Unbind();
    // delete the VBO
	void Delete();
};

#endif // VBO_INCL