#include <iostream>
#include <math.h>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "resources/shader.h"
#include "resources/VAO.h"
#include "resources/VBO.h"
#include "resources/EBO.h"
#include "resources/texture.h"

// vertices coordinates
GLfloat vertices[] =
{ //     coordinates     /        colors      
	-0.5f, 0.0f,  0.5f,     0.0f, 0.20f, 0.44f, 
	-0.5f, 0.0f, -0.5f,     0.83f, 0.70f, 0.0, 
	 0.5f, 0.0f, -0.5f,     0.20f, 0.0, 0.44f, 
	 0.5f, 0.0f,  0.5f,     0.0f, 0.8f, 0.44f,

	 -0.5f, 1.0f,  0.5f,     0.0f, 0.20f, 0.44f, 
	-0.5f, 1.0f, -0.5f,     0.83f, 0.70f, 0.0, 
	 0.5f, 1.0f, -0.5f,     0.20f, 0.0, 0.44f, 
	 0.5f, 1.0f,  0.5f,     0.0f, 0.8f, 0.44f, 

};

// indices for vertices order
GLuint indices[] =
{
	0, 3, 2, 
	0, 2, 1,

	0, 1, 5,
	0, 5, 4,

    1, 2, 5,
    2, 5, 6,

    7, 3, 2,
    7, 2, 6,

    0, 4, 7,
    0, 7, 3,

    4, 6, 5,
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGL Example 8", NULL, NULL);
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
	Shader shader("../src/example08/resources/shaders/vert.glsl", "../src/example08/resources/shaders/frag.glsl");

    // Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

    // generate vertex buffer object and link it to vertices
	VBO VBO1(vertices, sizeof(vertices));

	// generate element buffer object and link it to indices
	EBO EBO1(indices, sizeof(indices));

	// links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6*sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6*sizeof(float), (void*)(3*sizeof(float)));
    
	// unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

    // variables for rotating the pyramid
	float rotation = 0.0f;
	double old_time = glfwGetTime();

    glEnable(GL_DEPTH_TEST);
    
    // now do stuff in the window until it is told to close
    while (!glfwWindowShouldClose(window))
    {
        // specify the background color in RGBa format
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // specify the new color to the back buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        // tell openGL to use the shader program created above
        shader.Use();
        // get time for rotating the pyramid
        double time = glfwGetTime();
        if (time - old_time >= 1 / 60)
		{
			rotation += 0.5f;
			old_time = time;
		}
        // initialize model, projection, and view matrices
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 proj = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        // set up the matrices
        model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 1.0f));
        view = glm::translate(view,glm::vec3(0.0f, -0.5f, -4.0f));
        proj = glm::perspective(glm::radians(45.0f), (float)(window_width/window_height), 0.1f, 100.0f);
        // output the matrices into the vertex shader
		int model_loc = glGetUniformLocation(shader.shader_program, "model");
		glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
		int view_loc = glGetUniformLocation(shader.shader_program, "view");
		glUniformMatrix4fv(view_loc, 1, GL_FALSE, glm::value_ptr(view));
		int proj_loc = glGetUniformLocation(shader.shader_program, "proj");
		glUniformMatrix4fv(proj_loc, 1, GL_FALSE, glm::value_ptr(proj));
        // bind the VAO so openGL knows to use it
        VAO1.Bind();
        // specify the primatives that should be used to draw the elements
        glDrawElements(GL_TRIANGLES, sizeof(indices)/sizeof(int), GL_UNSIGNED_INT, 0);
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