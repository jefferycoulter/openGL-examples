#version 330 core

out vec4 FragColor; // output the colors
in vec3 color; // input from vertex shader

void main()
{
   FragColor = vec4(color, 1.0f);
}