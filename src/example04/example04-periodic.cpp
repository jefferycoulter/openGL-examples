#include <iostream>
#include <math.h>
#include <filesystem>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "resources/shader.h"
#include "resources/VAO.h"
#include "resources/VBO.h"
#include "resources/EBO.h"

// specify vertices of the triangle with an inner triangle removed
GLfloat vertices[] = 
    {//             // coordinates                          // color //
        -0.5f, -0.5f * float(sqrt(3)) * 1/3, 0.0f,   0.0f, 1.0f, 0.2f, // lower left point
		 0.5f, -0.5f * float(sqrt(3)) * 1/3, 0.0f,   0.0f, 0.3f, 1.0f, // Lower right point
		 0.0f,  0.5f * float(sqrt(3)) * 2/3, 0.0f,   1.0f, 0.3f, 0.3f, // upper point
		-0.25f, 0.5f * float(sqrt(3)) * 1/6, 0.0f,   0.6f, 0.8f, 0.17f, // middle left point
		 0.25f, 0.5f * float(sqrt(3)) * 1/6, 0.0f,   0.0f, 0.45f, 0.17f, // middle right point
		 0.0f, -0.5f * float(sqrt(3)) * 1/3, 0.0f,   0.0f, 0.4f, 0.3f // middle down point
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
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGL Example 4", NULL, NULL);
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
	Shader shader("desktop/openGL_examples/src/example04/resources/shaders/vert.glsl", "desktop/openGL_examples/src/example04/resources/shaders/frag.glsl");

    // Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// generate vertex buffer object and link it to vertices
	VBO VBO1(vertices, sizeof(vertices));

	// links VBO to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6*sizeof(float), (void*)0);
    VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6*sizeof(float), (void*)(3*sizeof(float)));
    
	// unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();

    // now do stuff in the window until it is told to close
    while (!glfwWindowShouldClose(window))
    {
        // specify the background color in RGBa format
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // specify the new color to the back buffer
        glClear(GL_COLOR_BUFFER_BIT);

        // setup color change with time
        float time = glfwGetTime();
        float value = (sin(time) / 2.0f) + 0.5f;
        // get the location of the shader variable that will change
        GLuint vertex_color_location = glGetUniformLocation(shader.shader_program, "color");
        // tell openGL to use the shader program created above
        shader.Use();
        // set the shader variable
        glUniform4f(vertex_color_location, 0.0f, value, 0.0f, 1.0f);
        // bind the VAO so openGL knows to use it
        VAO1.Bind();
        // specify the primatives that should be used to draw the vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);
        VAO1.Unbind();
        // swap the buffers so that the color appears
        glfwSwapBuffers(window);
        // tell glfw to poll events (e.g. window resizing, window close, etc.)
        glfwPollEvents();
    }
    
    // delete the shader objects and program once the window is closed
    VAO1.Delete();
	VBO1.Delete();
	shader.Delete();

    // since the glfw window is a pointer, you have to manually delete it when finished
    glfwDestroyWindow(window);

    // end the glfw context
    glfwTerminate();

    return 0;
}