#version 330 core

layout (location = 0) in vec3 aPos; // specifies coordinates
layout (location = 1) in vec3 aColor; // specifies color
layout (location = 2) in vec2 aTex; // specifies texture coordinates

out vec3 color; // output to fragment shader
out vec2 coord;

void main()
{
   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
   color = aColor;
   coord = aTex;
}