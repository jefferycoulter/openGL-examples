#include "texture.h"

Texture::Texture(const char* image, GLenum tex_type, const char* s_type, GLuint slot, GLenum format, GLenum pixel_type)
{
    // assign texture type to Texture object
    type = tex_type;
    str_type = s_type;

    // variables to hold image width, height and color channels
    int img_width, img_height, n_channels;

    // read texture image and flip vertically
    // since stb and openGL read in images differently
    stbi_set_flip_vertically_on_load(true);
    unsigned char *img = stbi_load(image, &img_width, &img_height, &n_channels, 0);
    if (!img) {
        std::cerr << "failed to load image" << "\n";
    }
    
    // generate openGL texture object
    glGenTextures(1, &m_tex);

    // assign the texture to a texture unit
    glActiveTexture(GL_TEXTURE0+slot);
    unit = slot;
    glBindTexture(tex_type, m_tex);
    
    // specify algorithm that is used to make the image smaller or bigger when resized
    glTexParameteri(tex_type, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(tex_type, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
    // specify the way the texture repeats in both x and y directions
    glTexParameteri(tex_type, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(tex_type, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // assign the image to the openGL Texture object
    glTexImage2D(tex_type, 0, GL_RGBA, img_width, img_height, 0, format, pixel_type, img);

    // generate mipmap
    glGenerateMipmap(tex_type);

    // since image data has been loaded into openGL texture, free the stb image
    stbi_image_free(img);

    // unbind openGL Texture object so it won't be modified
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Unit(Shader& shader, const char* uniform, GLuint unit)
{
    // get uniform from the shader program
    GLuint location = glGetUniformLocation(shader.shader_program, uniform);

    // call use before changing the uniform
    shader.Use();

    // set the value of the uniform
    glUniform1i(location, unit);
}

void Texture::Bind()
{
    glActiveTexture(GL_TEXTURE0+unit);
	glBindTexture(type, m_tex);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &m_tex);
}