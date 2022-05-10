#version 330 core

layout (location = 0) in vec3 aPos; // specifies coordinates
layout (location = 1) in vec3 aColor; // specifies color

out vec3 color; // output to fragment shader

uniform mat4 model;
uniform mat4 proj;
uniform mat4 view;

void main()
{
   gl_Position = proj * view * model * vec4(aPos, 1.0);
   color = aColor;
}