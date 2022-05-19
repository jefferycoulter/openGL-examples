#include <iostream>
#include <math.h>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include "resources/shader.h"
#include "resources/VAO.h"
#include "resources/VBO.h"
#include "resources/EBO.h"
#include "resources/texture.h"

// vertex coordinates
GLfloat vertices[] =
{ //     coordinates     /        colors      /   texture  //
	-0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 0.0f,	0.0f, 0.0f, // lower left corner
	-0.5f,  0.5f, 0.0f,     0.0f, 1.0f, 0.0f,	0.0f, 1.0f, // upper left corner
	 0.5f,  0.5f, 0.0f,     0.0f, 0.0f, 1.0f,	1.0f, 1.0f, // upper right corner
	 0.5f, -0.5f, 0.0f,     1.0f, 0.0f, 1.0f,	1.0f, 0.0f  // lower right corner
};

// indices
GLuint indices[] =
{
	0, 2, 1, // upper triangle
	0, 3, 2 // lower triangle
};

int main()
{
    std::cout << std::filesystem::current_path() << "\n";

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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGL Example 6", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "failed to create glfw window" << "\n";
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
	Shader shader("../src/example06/resources/shaders/vert.glsl", "../src/example06/resources/shaders/frag.glsl");

    // Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// generate vertex buffer object and link it to vertices
	VBO VBO1(vertices, sizeof(vertices));

	// generate element buffer object and link it to indices
	EBO EBO1(indices, sizeof(indices));

	// links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8*sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8*sizeof(float), (void*)(3*sizeof(float)));
    VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8*sizeof(float), (void*)(6*sizeof(float)));
    
	// unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

    // create texture object
    Texture trump("../src/example06/resources/textures/trump.png", GL_TEXTURE_2D, GL_TEXTURE0, GL_RGB, GL_UNSIGNED_BYTE);
    trump.Unit(shader, "tex0", 0);
    
    //shader.compileErrors(shader.shader_program, "no");

    // now do stuff in the window until it is told to close
    while (!glfwWindowShouldClose(window))
    {
        // specify the background color in RGBa format
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // specify the new color to the back buffer
        glClear(GL_COLOR_BUFFER_BIT);
        // tell openGL to use the shader program created above
        shader.Use();
        // bind texture
        trump.Bind();
        // bind the VAO so openGL knows to use it
        VAO1.Bind();
        // specify the primatives that should be used to draw the elements
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // swap the buffers so that the color appears
        glfwSwapBuffers(window);
        // tell glfw to poll events (e.g. window resizing, window close, etc.)
        glfwPollEvents();
    }
    
    // delete the shader objects and program once the window is closed
    VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
    trump.Delete();
	shader.Delete();

    // since the glfw window is a pointer, you have to manually delete it when finished
    glfwDestroyWindow(window);

    // end the glfw context
    glfwTerminate();

    return 0;
}