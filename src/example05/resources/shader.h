#ifndef SHADER_INCL
#define SHADER_INCL

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>

std::string get_file_content(const char *filename);

class Shader
{
    public:
        GLuint shader_program;
        Shader(const char *vertex_file, const char *fragment_file);

        void Use();
        void Delete();
};

#endif // SHADER_INCL