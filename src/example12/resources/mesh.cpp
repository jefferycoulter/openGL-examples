#include "mesh.h"

Mesh::Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures)
{
    Mesh::vertices = vertices;
	Mesh::indices = indices;
	Mesh::textures = textures;

	VAO.Bind();
    // generate vertex buffer object and link it to vertices
	VBO VBO(vertices);
	// generate element buffer object and link it to indices
	EBO EBO(indices);
	// links VBO to VAO
	VAO.LinkAttrib(VBO, 0, 3, GL_FLOAT, sizeof(Vertex), (void*)0); // coords
    VAO.LinkAttrib(VBO, 1, 3, GL_FLOAT, sizeof(Vertex), (void*)(3*sizeof(float))); // color
    VAO.LinkAttrib(VBO, 2, 3, GL_FLOAT, sizeof(Vertex), (void*)(6*sizeof(float))); // normals
	// unbind all to prevent accidentally modifying them
	VAO.Unbind();
	VBO.Unbind();
	EBO.Unbind();
}

void Mesh::Draw(Shader& shader, Camera& camera)
{
    shader.Use();
    VAO.Bind();

    // keep track of the number of diffuse and specular textures
    // since at run time the number will be unknown most likely
    unsigned int n_diff = 0;
    unsigned int n_spec = 0;

    // loop through the textures
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        //          temporary strings to store:
        std::string num;                        // number of textures of a certain type
        std::string type = textures[i].str_type;    // the type of texture

        if (type == "diffuse")
        { // if the type of texture[i] is diffuse, increment the number of diffuse textures
            num = std::to_string(n_diff++);
        }
        else if (type == "specular")
        { // if the type of texture[i] is specular, increment the number of specular textures
            num = std::to_string(n_spec++);
        }

        // find the uniform of the name type+num in the shader program
        textures[i].Unit(shader, (type + num).c_str(), i);
        textures[i].Bind();
    }
    glUniform3f(glGetUniformLocation(shader.shader_program, "cam_pos"), camera.position.x, camera.position.y, camera.position.z);
    camera.Matrix(shader, "camera_matrix");
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
}