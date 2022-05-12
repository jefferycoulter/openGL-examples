#ifndef TEXTURE_INCL
#define TEXTURE_INCL

#include<glad/glad.h>
#include<stb_image.h>

#include"shader.h"

class Texture
{
    public:
        GLuint m_tex;
        GLenum type;

        Texture(const char* image, GLenum texType, GLenum slot, GLenum format, GLenum pixel_type);

        // assigns a texture unit to a texture
        void Unit(Shader& shader, const char* uniform, GLuint unit);
        // binds a texture
        void Bind();
        // unbinds a texture
        void Unbind();
        // deletes a texture
        void Delete();
};

#endif // TEXTURE_INCL