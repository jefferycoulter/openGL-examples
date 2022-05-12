#include <iostream>
#include <math.h>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "resources/shader.h"
#include "resources/VAO.h"
#include "resources/VBO.h"
#include "resources/EBO.h"
#include "resources/texture.h"
#include "resources/camera.h"

GLfloat vertices[] =
{ //     coordinates     /        colors          /    normals       
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,     0.0f, -1.0f, 0.0f, // bottom side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, -1.0f, 0.0f, // bottom side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, -1.0f, 0.0f, // bottom side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, -1.0f, 0.0f, // bottom side

	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 -0.8f, 0.5f,  0.0f, // left Side
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 -0.8f, 0.5f,  0.0f, // left Side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 -0.8f, 0.5f,  0.0f, // left Side

	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.5f, -0.8f, // non-facing side
	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.5f, -0.8f, // non-facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.0f, 0.5f, -0.8f, // non-facing side

	 0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.44f,	 0.8f, 0.5f,  0.0f, // right side
	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 0.8f, 0.5f,  0.0f, // right side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 0.8f, 0.5f,  0.0f, // right side

	 0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f,	 0.0f, 0.5f,  0.8f, // facing side
	-0.5f, 0.0f,  0.5f,     0.83f, 0.70f, 0.44f, 	 0.0f, 0.5f,  0.8f, // facing side
	 0.0f, 0.8f,  0.0f,     0.92f, 0.86f, 0.76f,	 .0f, 0.5f,  0.8f  // facing side
};

// indices for vertices order
GLuint indices[] =
{
	0, 1, 2, // bottom side
	0, 2, 3, // bottom side
	4, 6, 5, // left side
	7, 9, 8, // non-facing side
	10, 12, 11, // right side
	13, 15, 14 // facing side
};

GLfloat light_vertices[] =
{ //     coordinates     //
	-0.1f, -0.1f,  0.1f,
	-0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f, -0.1f,
	 0.1f, -0.1f,  0.1f,
	-0.1f,  0.1f,  0.1f,
	-0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f, -0.1f,
	 0.1f,  0.1f,  0.1f
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGL Example 10", NULL, NULL);
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

    // generates shader object using shaders defualt.vert and default.frag
	Shader shader("../src/example10/resources/shaders/vert.glsl", "../src/example10/resources/shaders/frag.glsl");
    // Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();
    // generate vertex buffer object and link it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// generate element buffer object and link it to indices
	EBO EBO1(indices, sizeof(indices));
	// links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 9*sizeof(float), (void*)0); // coords
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 9*sizeof(float), (void*)(3*sizeof(float))); // color
    VAO1.LinkAttrib(VBO1, 2, 3, GL_FLOAT, 9*sizeof(float), (void*)(6*sizeof(float))); // normals
	// unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

    // create shader for light source
    Shader light_shader("../src/example10/resources/shaders/lvert.glsl", "../src/example10/resources/shaders/lfrag.glsl");
    // generate light VAO and bind
    VAO light_VAO;
    light_VAO.Bind();
    // generate light VBO
    VBO light_VBO(light_vertices, sizeof(light_vertices));
    // generate light EBO
    EBO light_EBO(light_indices, sizeof(light_indices));
    // link light VBO to light VAO
    light_VAO.LinkAttrib(light_VBO, 0, 3, GL_FLOAT, 3*sizeof(float), (void*)0);
    // unbind
    light_VAO.Unbind();
    light_VBO.Unbind();
    light_EBO.Unbind();

    glm::vec4 light_color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
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
        glUniform3f(glGetUniformLocation(shader.shader_program, "cam_pos"), camera.position.x, camera.position.y, camera.position.z);
        camera.Matrix(shader, "camera_matrix");
        // bind the VAO so openGL knows to use it
        VAO1.Bind();
        // specify the primatives that should be used to draw the elements
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        // tell openGL to use light shader program
		light_shader.Use();
		// export the camer matrix to the Vertex Shader of the light cube
		camera.Matrix(light_shader, "camera_matrix");
		// bind the VAO so openGL knows to use it
		light_VAO.Bind();
		// draw primitives, number of indices, datatype of indices, index of indices
		glDrawElements(GL_TRIANGLES, sizeof(light_indices)/sizeof(int), GL_UNSIGNED_INT, 0);
        // swap the buffers so that the color appears
        glfwSwapBuffers(window);
        // tell glfw to poll events (e.g. window resizing, window close, etc.)
        glfwPollEvents();
    }
    
    // delete the shader objects and program once the window is closed
    VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shader.Delete();

    // since the glfw window is a pointer, you have to manually delete it when finished
    glfwDestroyWindow(window);

    // end the glfw context
    glfwTerminate();

    return 0;
}