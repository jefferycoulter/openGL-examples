#version 330 core

layout (location = 0) in vec3 aPos; // specifies coordinates
layout (location = 1) in vec3 aColor; // specifies color

out vec3 color; // output to fragment shader

uniform mat4 camera_matrix;

void main()
{
   gl_Position = camera_matrix * vec4(aPos, 1.0);
   color = aColor;
}