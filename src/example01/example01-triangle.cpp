#include <iostream>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// vertex shader source code
const char* vertex_shader_source = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
// fragment shader source code
const char* fragment_shader_source = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";

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

    // specify vertices of the triangle
    GLfloat vertices[] = 
    {
        -0.5f, -0.5f * float(sqrt(3)) * 1/3, 0.0f, // lower left point
		 0.5f, -0.5f * float(sqrt(3)) * 1/3, 0.0f, // lower right point
		 0.0f,  0.5f * float(sqrt(3)) * 2/3, 0.0f // upper point
    };
    
    // specify the width and height of the window
    int window_width = 800;
    int window_height = 800;

    // create a glfw window and check that the window was successfully created
    GLFWwindow* window = glfwCreateWindow(window_width, window_height, "OpenGL Example 1", NULL, NULL);
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

    // create and instance of an openGL shader object for the vertex and fragment shaders
    GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    // give the shader source code to the shaders
    // first argument is shader id, second is number of source strings, 
    // third is reference to source strings, fourth is length of source strings.
    // here, each shader only has one source string.
    //
    // see: 
    //    https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glShaderSource.xhtml
    //    https://stackoverflow.com/questions/22100408/what-is-the-meaning-of-the-parameters-to-glshadersource
    // for more details
    glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
    glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);

    // have to compile the shader source code into machine code for the shaders to understand it
    glCompileShader(vertex_shader);
    glCompileShader(fragment_shader);

    // now create a shader program to use the shaders
    GLuint shader_program = glCreateProgram();

    // attach the shaders to the program
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);

    // link the program
    glLinkProgram(shader_program);

    // since the shaders have been compiled and linked to the program, they can be deleted
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    // transferring data between the cpu and gpu can be slow, so transfer it in large chunks
    // using buffers.  openGL uses vertex buffer objects (VBO) and vertex array objects (VAO)
    // to do this.  VBO stores the data and VAO act as pointers and can quickly switch between
    // different VBO.
    
    // initialize vertex array and buffer objects
    GLuint VAO, VBO;

    // create the VAO with a single object (must be done before create the VBO).
    // this attaches the id of the vertex array object to the variable VAO for later use
    glGenVertexArrays(1, &VAO);

    // create the buffer with a single object.
    // similarly, this attaches the id of the vertex buffer object to the variable VBO
    glGenBuffers(1, &VBO);

    // make the vertex array object saved in VAO the current vertex array object by binding it.
    // binding in openGL makes the binded object the current object to be used, so whenever a 
    // function is called to modify a certain object, the binded object is modified.  
    // here, whenever a function uses or modifies a vertex buffer
    // object, it will use the object referenced by the VBO variable.
	glBindVertexArray(VAO);

    // bind the vertex buffer referenced by VBO to an openGL array buffer. binding here means
    // the same as described above
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // store the vertex data in the buffer.  since VBO was binded to GL_ARRAY_BUFFER above,
    // our data will be stored in the buffer referenced by the VBO variable.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // tell openGL how to interpret the vertex data
    // first parameter is location in shader, second is the number of values per vertex,
    // third is the type of value, the fourth is whether the type is an integer value,
    // the fifth is the size of each vertex, and the last is a pointer offset, i.e. where the
    // data begins in the array
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

    // enable the vertex attribute so that openGL knows to use it
    glEnableVertexAttribArray(0);

    // bind both the buffer object and array object to 0 so that the VAO and VBO that have 
    // been created aren't accidentally changed by some function later
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

    // now do stuff in the window until it is told to close
    while (!glfwWindowShouldClose(window))
    {
        // specify the background color in RGBa format
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        // specify the new color to the back buffer
        glClear(GL_COLOR_BUFFER_BIT);
        // tell openGL to use the shader program created above
        glUseProgram(shader_program);
        // bind the VAO created above so openGL knows to use it
        glBindVertexArray(VAO);
        // specify the primatives that should be used to draw the vertices
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // swap the buffers so that the color appears
        glfwSwapBuffers(window);

        // tell glfw to poll events (e.g. window resizing, window close, etc.)
        glfwPollEvents();
    }
    
    // delete the shader objects and program once the window is closed
    glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteProgram(shader_program);

    // since the glfw window is a pointer, you have to manually delete it when finished
    glfwDestroyWindow(window);

    // end the glfw context
    glfwTerminate();

    return 0;
}