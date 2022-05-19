#ifndef MESH_INCL
#define MESH_INCL

#include <string>

#include "VAO.h"
#include "EBO.h"
#include "texture.h"
#include "camera.h"

class Mesh
{
    /**
     * @brief meshes in openGL consist of vertices, indices, and textures.  this class combines
     * all of the previously created classes (VAO, EBO, Texture, etc.) into a single mesh object
     * 
     */
public:
    // use dynamic vectors to store properties of mesh
	std::vector <Vertex> vertices;
	std::vector <GLuint> indices;
	std::vector <Texture> textures;

	// the Draw function will use VAO, so store it publicly
	VAO VAO;

	// initialize the mesh
	Mesh(std::vector <Vertex>& vertices, std::vector <GLuint>& indices, std::vector <Texture>& textures);

	// draw the mesh
	void Draw(Shader& shader, Camera& camera);
};
#endif // MESH_INCL