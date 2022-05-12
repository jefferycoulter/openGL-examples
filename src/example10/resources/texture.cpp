#include "texture.h"

Texture::Texture(const char* image, GLenum textype, GLenum slot, GLenum format, GLenum pixel_type)
{
    // assign texture type to Texture object
    type = textype;

    // variables to hold image width, height and color channels
    int img_width, img_height, n_channels;

    // read texture image and flip vertically
    // since stb and openGL read in images differently
    stbi_set_flip_vertically_on_load(true);
    unsigned char *img = stbi_load(image, &img_width, &img_height, &n_channels, 0);
    
    // generate openGL texture object
    glGenTextures(1, &m_tex);

    // assign the texture to a texture unit
    glActiveTexture(slot);
    glBindTexture(GL_TEXTURE_2D, m_tex);
    
    // specify algorithm that is used to make the image smaller or bigger when resized
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   
    // specify the way the texture repeats in both x and y directions
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // assign the image to the openGL Texture object
    glTexImage2D(type, 0, GL_RGBA, img_width, img_height, 0, format, pixel_type, img);

    // generate mipmap
    glGenerateMipmap(type);

    // since image data has been loaded into openGL texture, free the stb image
    stbi_image_free(img);

    // unbind openGL Texture object so it won't be modified
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Unit(Shader& shader, const char* uniform, GLuint unit)
{
    // get uniform from the shader program
    GLuint tex0_location = glGetUniformLocation(shader.shader_program, uniform);

    // call use before changing the uniform
    shader.Use();

    // set the value of the uniform
    glUniform1i(tex0_location, unit);
}

void Texture::Bind()
{
	glBindTexture(type, m_tex);
}

void Texture::Unbind()
{
	glBindTexture(type, 0);
}

void Texture::Delete()
{
	glDeleteTextures(1, &m_tex);
}