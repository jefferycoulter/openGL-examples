#version 330 core

layout (location = 0) in vec3 aPos; // specifies coordinates
layout (location = 1) in vec3 aNormal; // specify normals
layout (location = 2) in vec3 aColor; // specifies color
layout (location = 3) in vec2 aTex; // texture coordinates

out vec3 current_position; // output current position to fragment shader
out vec3 Normal; // output normal to fragment shader
out vec3 color; // output to fragment shader
out vec2 coords; // output to fragment shader

uniform mat4 camera_matrix;
uniform mat4 model; // imports the model matrix from the main function

void main()
{
   current_position = vec3(model*vec4(aPos, 1.0f));
   Normal = aNormal;
   color = aColor;
   coords = aTex;
   gl_Position = camera_matrix * vec4(current_position, 1.0);
}