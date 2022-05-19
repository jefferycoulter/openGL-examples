#include "shader.h"

// reads a text file and outputs a string with everything in the text file
std::string load_shader(const char *filename)
{	
	std::string content;
	std::ifstream file_stream(filename, std::ios::in);

	if (!file_stream.is_open())
	{
		std::cerr << "failed to read file " << filename << "\n";
		return "";
	}

	std::string line = "";
    while(!file_stream.eof()) {
        std::getline(file_stream, line);
        content.append(line + "\n");
    }

    file_stream.close();
    return content;
}

// constructor to build the shader program
Shader::Shader(const char* vertex_file, const char* fragment_file)
{
	// read vertex file and fragment file and store the strings
	std::string vertex_code = load_shader(vertex_file);
	std::string fragment_code = load_shader(fragment_file);

	// convert the shader source code strings into character arrays
	const char* vertex_source = vertex_code.c_str();
	const char* fragment_source = fragment_code.c_str();

	// create and instance of an openGL shader object for the vertex and fragment shaders
	GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

	// give the shader source code to the shaders
	glShaderSource(vertex_shader, 1, &vertex_source, NULL);
    glShaderSource(fragment_shader, 1, &fragment_source, NULL);

	// have to compile the shader source code into machine code for the shaders to understand it
	glCompileShader(vertex_shader);
	glCompileShader(fragment_shader);

	// now create a shader program to use the shaders
	shader_program = glCreateProgram();

	// attach the vertex and fragment shaders to the program
	glAttachShader(shader_program, vertex_shader);
	glAttachShader(shader_program, fragment_shader);

	// link the shaders to the program
	glLinkProgram(shader_program);

	// since the shaders have been compiled and linked to the program, they can be deleted
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

}

// activates the shader program
void Shader::Use()
{
	glUseProgram(shader_program);
}

// delete the shader program
void Shader::Delete()
{
	glDeleteProgram(shader_program);
}


// checks if the shaders have compiled properly
void Shader::compileErrors(unsigned int shader, const char* type)
{
	// Stores status of compilation
	GLint hasCompiled;
	// Character array to store error message in
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << infoLog << std::endl;
		}
	}
} 