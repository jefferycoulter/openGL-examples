#version 330 core

out vec4 FragColor; // output the colors
uniform vec4 color; // this color will change

void main()
{
   FragColor = color;
}