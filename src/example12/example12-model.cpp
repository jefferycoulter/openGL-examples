#include <iostream>
#include <math.h>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "resources/mesh.h"

// vertices coordinates
Vertex vertices[] =
{ //               coordinates           /            colors          /           textures coords         /       normals  
	Vertex{glm::vec3(-1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 0.0f)},
	Vertex{glm::vec3(-1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
	Vertex{glm::vec3( 1.0f, 0.0f,  1.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec2(1.0f, 0.0f)}
};

// indices for vertices order
GLuint indices[] =
{
	0, 1, 2,
	0, 2, 3
};

Vertex light_vertices[] =
{ //     coordinates     
	Vertex{glm::vec3(-0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f, -0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f,  0.1f)},
	Vertex{glm::vec3(-0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f, -0.1f)},
	Vertex{glm::vec3(0.1f,  0.1f,  0.1f)}
};

GLuint light_indices[] =
{
	0, 1, 2,
	0, 2, 3,
	0, 4, 7,
	0, 7, 3,
	3, 7, 6,
	3, 6, 2,
	2, 6, 5,
	2, 5, 1,
	1, 5, 4,
	1, 4, 0,
	4, 5, 6,
	4, 6, 7
};

int main()
{
    // initialize glfw context
    glfwInit();

    // specify the minimum version of openGL to be used
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

    // specify the "profile" or functions to use
    // the "core profile" (GLFW_OPENGL_CORE_PROFILE) contains only the modern functions.
    // "compatibility profile" (GLFW_OPENGL_COMPAT_PROFILE) contains both modern and 
    // outdated functions as well
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // specify the width and height of the window
    int window_width = 800;
    int window_height = 800;

    // create a glfw window and check that the window was successfully created
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGL Example 12", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "failed to create glfw window" << "\n";
        glfwTerminate(); // make sure to end glfw context before exiting
        return -1;
    }

    // although the window has been created, you have to tell glfw to use it in the current context
    glfwMakeContextCurrent(window);

    // load glad
    gladLoadGL();

    // specify viewport, or where openGL should render within the window (here, the entire window)
    glViewport(0, 0, window_width, window_height);

    // texture data
	Texture textures[]
	{
		Texture("../src/example12/textures/planks.png", GL_TEXTURE_2D, "diffuse", 0, GL_RGBA, GL_UNSIGNED_BYTE),
		Texture("../src/example12/textures/planksSpec.png", GL_TEXTURE_2D, "specular", 1, GL_RED, GL_UNSIGNED_BYTE)
	};

    // generates shader object using shaders defualt.vert and default.frag
	Shader shader("../src/example12/resources/shaders/vert.glsl", "../src/example12/resources/shaders/frag.glsl");

    // store mesh data in vectors for the mesh
	std::vector <Vertex> verts(vertices, vertices + sizeof(vertices) / sizeof(Vertex));
	std::vector <GLuint> ind(indices, indices + sizeof(indices) / sizeof(GLuint));
	std::vector <Texture> tex(textures, textures + sizeof(textures) / sizeof(Texture));
    // create floor mesh
	Mesh floor(verts, ind, tex);

    // create shader for light source
    Shader light_shader("../src/example12/resources/shaders/lvert.glsl", "../src/example12/resources/shaders/lfrag.glsl");
    // store mesh data in vectors for the mesh
	std::vector <Vertex> light_verts(light_vertices, light_vertices + sizeof(light_vertices) / sizeof(Vertex));
	std::vector <GLuint> light_ind(light_indices, light_indices + sizeof(light_indices) / sizeof(GLuint));
	// create light mesh
	Mesh light(light_verts, light_ind, tex);

    glm::vec4 light_color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	glm::vec3 light_pos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 light_model = glm::mat4(1.0f);
	light_model = glm::translate(light_model, light_pos);

	glm::vec3 pyramid_pos = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::mat4 pyramid_model = glm::mat4(1.0f);
	pyramid_model = glm::translate(pyramid_model, pyramid_pos);

    light_shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(light_shader.shader_program, "model"), 1, GL_FALSE, glm::value_ptr(light_model));
	glUniform4f(glGetUniformLocation(light_shader.shader_program, "light_color"), light_color.x, light_color.y, light_color.z, light_color.w);
	shader.Use();
	glUniformMatrix4fv(glGetUniformLocation(shader.shader_program, "model"), 1, GL_FALSE, glm::value_ptr(pyramid_model));
	glUniform4f(glGetUniformLocation(shader.shader_program, "light_color"), light_color.x, light_color.y, light_color.z, light_color.w);
	glUniform3f(glGetUniformLocation(shader.shader_program, "light_pos"), light_pos.x, light_pos.y, light_pos.z);
    glEnable(GL_DEPTH_TEST);

    // create camera object
    Camera camera(window_width, window_height, glm::vec3(0.0f, 0.0f, 4.0f));

    // variables for rotating the pyramid
	float rotation = 0.0f;
	double old_time = glfwGetTime();
    
    // now do stuff in the window until it is told to close
    while (!glfwWindowShouldClose(window))
    {
        // specify the background color in RGBa format
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // specify the new color to the back buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // get input from user
        camera.Inputs(window);
        // update the matrices with the input
        camera.Update(45.0f, 0.1f, 100.0f);
        // tell openGL to use the shader program created above
        shader.Use();
        // get time for rotating the pyramid
        double time = glfwGetTime();
        if (time - old_time >= 1 / 60)
		{
			rotation += 0.5f;
			old_time = time;
		}
        // initialize model matrix for rotations
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 1.0f));
        
        // output the matrices into the vertex shader
		int model_loc = glGetUniformLocation(shader.shader_program, "model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));

        // draw the meshes
        floor.Draw(shader, camera);
        light.Draw(shader, camera);

        // swap the buffers so that the color appears
        glfwSwapBuffers(window);
        // tell glfw to poll events (e.g. window resizing, window close, etc.)
        glfwPollEvents();
    }
    
    // delete the shader objects and program once the window is closed
	shader.Delete();
    light_shader.Delete();

    // since the glfw window is a pointer, you have to manually delete it when finished
    glfwDestroyWindow(window);

    // end the glfw context
    glfwTerminate();

    return 0;
}