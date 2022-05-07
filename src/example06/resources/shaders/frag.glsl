#version 330 core

out vec4 FragColor; // output the colors
in vec3 color; // input from vertex shader
in vec2 coord; // input from vertex shader

uniform sampler2D tex0;

void main()
{
      FragColor = texture(tex0[0], coord);
}